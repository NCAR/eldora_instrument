/*
 *	$Id$
 *
 *	Module:	hskp	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  17:08:42  craig
 * Initial revision
 *
 *
 * description: This module provides executive control of the ELDORA 
 *              Housekeeping processor by reading a header passed
 *              by the Control Processor and following command passed
 *              by the control processor. 
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define SCOPE
#define int_vec0 255
#define int_vec1 254
#define int_vec2 253
#define int_vec3 252
#define DATAX    0x0            /* Offset to X axis Data Byte */
#define STATUSX  0x2            /* Offset to X axis status byte */
#define POS_OFF  0              /* Offset from "true" North in counts */
#define ENC_CNTS 2048           /* maximum number of encoder counts */  

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "Parameter.h"
#include "RadarDesc.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "RadarDesc.h"

extern HeaderPtr inHeader;
CELLSPACING *cs;
RADARDESC *rdsc;
WAVEFORM *wvfm;
PARAMETER *prm;
int radar_fore_aft;
hskp()
{


long           i, j, num_coll_int, num_dspq_int;

unsigned char  clr_pdf, *bim_cr0, *bim_cr1, *bim_cr2, 
               *bim_cr3, *bim_vr0, *bim_vr1, *bim_vr2,
               *bim_vr3, clr_pif, stat, *coll_sync;

int            *samps, data_samps, buff_cnt, prf, gates, num1, gate_sp,
               first_gate, tst_pls, sem_status, sampl, keybd,
               position, chips[6], mcpl_stat, prt_flag, f1_flag, f2_flag, 
               f3_flag, pick_gate, f_flag;

float          duty_cycle, rpt_seq, frq1, frq2, frq3, fake_posit;
char           ch;
unsigned long  *data_loc, *long_pntr, *tdpl_status;

void galil();
void bim_int0_isr();
void bim_int1_isr();
void bim_int2_isr();
void bim_int3_isr();
void mysetcal(void);
int posit();


/* Define needed tape structures */

struct radar_d this_radar;
struct cell_spacing_d cellspace;
struct parameter_d param[8];

/* Define some general purpose variables */
short proc, timeout, status, mad;
long current_mailbox_add, current_ray_add;


/* Set up BIM interrupt vector table in host CPU */

      if (intConnect((VOIDFUNCPTR *)(int_vec0 * 4), (VOIDFUNCPTR)bim_int0_isr,0         ) == ERROR)
	{
	    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n"                   ,int_vec0);
	    exit(1);
	}
      if (intConnect((VOIDFUNCPTR *)(int_vec1 * 4), (VOIDFUNCPTR)bim_int1_isr,0         ) == ERROR)
	{
	    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n"                   ,int_vec1);
	    exit(1);
	}
      if (intConnect((VOIDFUNCPTR *)(int_vec2 * 4), (VOIDFUNCPTR)bim_int2_isr,0         ) == ERROR)
	{
	    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n"                   ,int_vec2);
	    exit(1);
	}
      if (intConnect((VOIDFUNCPTR *)(int_vec3 * 4), (VOIDFUNCPTR)bim_int3_isr,0         ) == ERROR)
	{
	    fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n"                   ,int_vec3);
	    exit(1);
	}

/* Create the required semaphores for each BIM ISR */

bim_int0_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int1_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int2_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);
bim_int3_sem = semBCreate(SEM_Q_FIFO,SEM_EMPTY);

/* Initialize the Control and Vector Registers in the Collator's BIM */

bim_cr3 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg3);
stat = *bim_cr3;

bim_cr0 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg0);
*bim_cr0 = 0xdc;  /* set INT0* for interrupt level IRQ4* */
                  /* INT0* is end of beam interrupt */
                  /* NOTE: must write to register in interrupt handler */
                  /*       to re-enable interrupt again                */
stat = *bim_cr0;
printf("BIM control register 0 = %x \n",stat);

bim_cr1 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg1);
*bim_cr1 = 0xdb;  /* set INT1* for interrupt level IRQ3* */
                  /* INT1* is mid-beam interrupt */
                  /* NOTE: must write to register in interrupt handler */
                  /*       to re-enable interrupt again                */
stat = *bim_cr1;
printf("BIM control register 1 = %x \n",stat);

bim_cr2 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg2);
*bim_cr2 = 0xda;  /* set INT2* for interrupt level IRQ2* */
                  /* (INT2* is DP out of sync interrupt) */
                  /* NOTE: must write to register in interrupt handler */
                  /*       to re-enable interrupt again                */
stat = *bim_cr2;
printf("BIM control register 2 = %x \n",stat);

bim_cr3 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg3);
*bim_cr3 = 0xd9;  /* set INT3* for interrupt level IRQ1* */
                  /* INT3* is Collator out of sync interrupt) */
                  /* NOTE: must write to register in interrupt handler */
                  /*       to re-enable interrupt again                */
stat = *bim_cr3;
printf("BIM control register 3 = %x \n",stat);

bim_vr0 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg0);
*bim_vr0 = int_vec0;
stat = *bim_vr0;
printf("BIM vector register 0 = %x \n",stat);

bim_vr1 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg1);
*bim_vr1 = int_vec1;
stat = *bim_vr1;
printf("BIM vector register 1 = %x \n",stat);

bim_vr2 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg2);
*bim_vr2 = int_vec2;
stat = *bim_vr2;
printf("BIM vector register 2 = %x \n",stat);

bim_vr3 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg3);
*bim_vr3 = int_vec3;
stat = *bim_vr3;
printf("BIM vector register 3 = %x \n",stat);

printf("Samples = %d; PRF = %d; GATES = %d; GATE SPACING = %d; \n",sampl,prf,gates,gate_sp); 
printf("First Gate = %d; Duty Cycle = %f; Test Pulse = %d \n",gate_sp,duty_cycle,tst_pls);

/* Enable 68040 Interrupts */

sysIntEnable(1);
sysIntEnable(2);
sysIntEnable(3);
if(!radar_fore_aft)
  slow(rpm);
taskDelay(300); /* Add 3 seconds of delay to allow ant to rotate to 0deg */
}
