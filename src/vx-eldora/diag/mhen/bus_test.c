/*
 *      $Id$
 *
 *      Module: BUS_TEST.C
 *      Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module provides control over a high speed data bus
 *              test that should pinpoint faulty hardware more quickly.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define SCOPE
#define RP7_SCOPE
#define NO_PIO_CARD
#define NO_RDP_SCOPE
#define NO_LAB
#define FREQ_ON 1
#define FREQ_OFF 0
#define FREQ_3
#define FREQ_4
#define FREQ_5
#define NEW_TM
#define T_PULSE
#define GATES 512
#define int_vec0 175
#define int_vec1 174
#define int_vec2 173
#define int_vec3 172
#define DATAX    0x0            /* Offset to X axis Data Byte */
#define STATUSX  0x2            /* Offset to X axis status byte */
#define POS_OFF  0              /* Offset from "true" North in counts */
#define ENC_CNTS 2048           /* maximum number of encoder counts */  
#define FORE_MASK 0x1

#define NEW 0
#define OLD 1
#define MB  0x1
#define EOB 0x2
#define DP  0x4
#define COL 0x8

#define Fifo_sz 4096

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
#include "string.h"
#include "systime.h"
#include "sysLib.h"
#include "tickLib.h"

#include "HeaderRpc.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "NU_ELDRP7.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "LclCommands.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "pio_def.h"
#include "dspaddr.h"
#include "rp7.h"

#define DMC_BASE 0xFF81 + SHORT_BASE    /* Base Address of DMC-300 Card */

extern int Silent;
int Param = 6;
short task_sync = 0;
float rpm;
int position, err_cnt, int_cnt, proc_stat;
extern long max_time;

/* make the following variables available throughout this
   module only !                                        
*/

volatile static unsigned char  *collint, *dspint0, *dspint1, *dspint2, *dspint3, 
                      *dspint4, *dspint5, *dspint6, *dspint7,*dspint8,
                      *dspint9,*dspint10,*dspint11,*dspint12,*dspint13,
                      *dspint14,*dspint15,*dspint16,*dspint17,*dspint18,
                      *dspint19,*dspq0_dsp0_pcr, clr_pif;
                  

volatile static    char   *data = (char *)(DMC_BASE + DATAX);
volatile static    char   *gal_status = (char *)(DMC_BASE + STATUSX);
static    latchpos = 0;
static    unsigned char table[]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,
				 0x8,0x9,0x0,0x0,0x0,0x0,0x0,0x0,
                                 0x0,0xA,0xB,0xC,0xD,0xE,0xF};

volatile static int  f1_flag, f2_flag, f3_flag, f4_flag, f5_flag;

bus_test()
{


long           i, j;


volatile unsigned char   *bim_vr2, *led, stat;

int            gates, shortcnt, sem_status, sampl, load_stat, dp_stat[2], coll_stat,
               chips[6], mcpl_stat, prt_flag, ppp_stat, dc_flag, choice,
               pick_gate, f_flag, taskid, dc_stat, temp_stat[5];

unsigned int   coll_pick_gate;
float          scale_fac, proc_const, Pnoise, tx_data[4096];

unsigned long e_time;

void dma_data(unsigned long frq, unsigned long bd);
void bim_int2_isr();

/* Initialize Ascope Defaults */

TS_gate = 40;
TS_freq = 1;
TS_off = 0;
SCALE_fac = 1.0;
ASCOPE_mode = TIMESERIES; /* default is Time Series */

for(;;)
  {
    int_cnt = 0;  /* reset interrupt counter */
    ppp_stat = 0;
    proc_stat = 0;
    err_cnt = 0;
    max_time = 0;
    tickSet(0);   /* reset tick counter */
    gates = Fifo_sz;
    sampl=1;
    prt_flag=0;
    Pnoise = 0.0;
    scale_fac = 1.0;
    coll_pick_gate = 10;
    proc_const = 280.0;

/* Make sure Timing Module is off */

#ifdef NEW_TM	    
    timoffb();
#endif
#ifndef NEW_TM
    timoff();
#endif
    printf("HSDBB BUS TEST\n");
    printf("Type 'stop = 1' at any time to stop test\n");
    printf("Enter frequency's HSDBB to test (1 thru 5) or 0 to End\n");
    scanf(" %d",&choice);
    switch (choice)
      {
      case 0:
	printf("Exiting Bus Test\n");
	return;
	break;
      case 1:
	f1_flag = 1;
	f2_flag = 0;
	f3_flag = 0;
	f4_flag = 0;
	f5_flag = 0;
	break;
      case 2:
	f1_flag = 0;
	f2_flag = 1;
	f3_flag = 0;
	f4_flag = 0;
	f5_flag = 0;
	break;
      case 3:
	f1_flag = 0;
	f2_flag = 0;
	f3_flag = 1;
	f4_flag = 0;
	f5_flag = 0;
	break;
      case 4:
	f1_flag = 0;
	f2_flag = 0;
	f3_flag = 0;
	f4_flag = 1;
	f5_flag = 0;
	break;
      case 5:
	f1_flag = 0;
	f2_flag = 0;
	f3_flag = 0;
	f4_flag = 0;
	f5_flag = 1;
	break;
      default:
	f1_flag = 1;
	f2_flag = 0;
	f3_flag = 0;
	f4_flag = 0;
	f5_flag = 0;
	break;
      }


    /* Load Doppler Processor with appropriate code */

    load_stat = nu_lddsp(1,1,3,0,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,1,3,1,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,1,3,2,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,1,3,3,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,2,3,0,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,2,3,1,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,2,3,2,"hsdbb_rx_s.dmp");
    load_stat = nu_lddsp(1,2,3,3,"hsdbb_rx_s.dmp");

    /* Load Pulse Pair Processor for F1 with appropriate code */

    load_stat = nu_lddsp(1,1,1,0,"hsdbb_tx_m.dmp");
    load_stat = nu_lddsp(1,1,1,1,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,1,1,2,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,1,1,3,"hsdbb_tx_s.dmp");

/* Load Pulse Pair Processor for F2 with appropriate code */

    load_stat = nu_lddsp(1,2,1,0,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,2,1,1,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,2,1,2,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,2,1,3,"hsdbb_tx_s.dmp");

/* Load Pulse Pair Processor for F3 with appropriate code */

    load_stat = nu_lddsp(1,3,1,0,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,3,1,1,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,3,1,2,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,3,1,3,"hsdbb_tx_s.dmp");

/* Load Pulse Pair Processor for F4 with appropriate code */

    load_stat = nu_lddsp(1,4,1,0,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,4,1,1,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,4,1,2,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,4,1,3,"hsdbb_tx_s.dmp");

/* Load Pulse Pair Processor for F5 with appropriate code */

    load_stat = nu_lddsp(1,5,1,0,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,5,1,1,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,5,1,2,"hsdbb_tx_s.dmp");
    load_stat = nu_lddsp(1,5,1,3,"hsdbb_tx_s.dmp");

    if(f1_flag)
      {
/* Initialize Daisy Chain for F1 PPP and DP */

	dsy(1,1,1);
	dsy(1,3,1);
	dsy(2,3,0);
	
/* Download desired number of Gates and Samples to PPP's, DP, and Collator for F1 */

	ngates(gates,1,1,1);
	ngates(gates,1,1,3);
	ngates(gates,1,2,3);
	sam4(sampl,1,1,1);
	sam4(sampl,1,1,3);
	sam4(sampl,1,2,3);

/* Set DP operational flags */

	opflags(prt_flag,f1_flag,f2_flag,f3_flag,f4_flag,f5_flag,proc_const,coll_pick_gate,Pnoise,1,3);
	opflags(prt_flag,f1_flag,f2_flag,f3_flag,f4_flag,f5_flag,proc_const,coll_pick_gate,Pnoise,2,3);

   /* Turn on DP LED's */

	led = (unsigned char *)(DP1_BASE + DLEDON);
	*led = 0x0;
	led = (unsigned char *)(DP2_BASE + DLEDON);
	*led = 0x0;

/* Place PPP "pick_gate" coincident with test pulse and enable operation */

	ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,1,1);


    /* Turn on PPP LED */
	led = (unsigned char *)(PPP_F1_BASE + DLEDON);
	*led = 0x0;


    /* DMA fake data to DSP Memory */

	dma_data(1,1);

      }
    else
      {
	ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,1,1);


	/* Turn off PPP LED */
	led = (unsigned char *)(PPP_F1_BASE + DLEDOFF);
	*led = 0x0;
      }

    if(f2_flag)
      {	  
/* Initialize Daisy Chain for F2 */

	dsy(2,1,1);
		  
/* Download desired number of Gates and Samples to PPP's, DP, and Collator for F2 */

	ngates(gates,1,2,1);
	sam4(sampl,1,2,1);

/* Place PPP "pick_gate" coincident with test pulse and enable operation */

	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,2,1);

           /* Turn on PPP LED */
	led = (unsigned char *)(PPP_F2_BASE + DLEDON);
	*led = 0x0;

    /* DMA fake data to DSP Memory */

	dma_data(2,1);

      }
    else
      {
	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,2,1);
	
     /* Turn off PPP LED */
	led = (unsigned char *)(PPP_F2_BASE + DLEDOFF);
	*led = 0x0;
      }

    if(f3_flag)
      {		  
/* Initialize Daisy Chain for F3 */

	dsy(3,1,1);

/* Download desired number of Gates and Samples to PPP's, DP, and Collator for F3 */
	ngates(gates,1,3,1);
	sam4(sampl,1,3,1);

/* Place PPP "pick_gate" coincident with test pulse and enable operation */

	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,3,1);
	
    /* Turn on PPP LED */
	led = (unsigned char *)(PPP_F3_BASE + DLEDON);
	*led = 0x0;

    /* DMA fake data to DSP Memory */

	dma_data(3,1);

      }
    else
      {
	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,3,1);
	
   /* Turn off PPP LED */
	led = (unsigned char *)(PPP_F3_BASE + DLEDOFF);
	*led = 0x0;
      }

    if(f4_flag)
      {		  
/* Initialize Daisy Chain for F4 */

	dsy(4,1,1);

/* Download desired number of Gates and Samples to PPP's, DP, and Collator for F4 */
	ngates(gates,1,4,1);
	sam4(sampl,1,4,1);

/* Place PPP "pick_gate" coincident with test pulse and enable operation */

	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,4,1);
	
    /* Turn on PPP LED */
	led = (unsigned char *)(PPP_F4_BASE + DLEDON);
	*led = 0x0;

    /* DMA fake data to DSP Memory */

	dma_data(4,1);

      }
    else
      {
	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,4,1);

    /* Turn off PPP LED */
	led = (unsigned char *)(PPP_F4_BASE + DLEDOFF);
	*led = 0x0;
      }
    
    if(f5_flag)
      {		  
/* Initialize Daisy Chain for F5 */

	dsy(5,1,1);

/* Download desired number of Gates and Samples to PPP's, DP, and Collator for F5 */
	ngates(gates,1,5,1);
	sam4(sampl,1,5,1);

/* Place PPP "pick_gate" coincident with test pulse and enable operation */

	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,5,1);

    /* Turn on PPP LED */
	led = (unsigned char *)(PPP_F5_BASE + DLEDON);
	*led = 0x0;

    /* DMA fake data to DSP Memory */

	dma_data(5,1);

      }
    else
      {
	scale_fac = 1.0;
	ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,5,1);

    /* Turn off PPP LED */
	led = (unsigned char *)(PPP_F5_BASE + DLEDOFF);
	*led = 0x0;
	
      }

/* Define PIF flags in PPP and DP */

    dspint0 = (unsigned char *)(PPP_F1_BASE + DSP0SEL + PIRH);
    dspint1 = (unsigned char *)(PPP_F1_BASE + DSP1SEL + PIRH);
    dspint2 = (unsigned char *)(PPP_F1_BASE + DSP2SEL + PIRH);
    dspint3 = (unsigned char *)(PPP_F1_BASE + DSP3SEL + PIRH);
    dspint8 = (unsigned char *)(DP1_BASE + DSP0SEL + PIRH);
    dspint9 = (unsigned char *)(DP1_BASE + DSP1SEL + PIRH);
    dspint10 = (unsigned char *)(DP1_BASE + DSP2SEL + PIRH);
    dspint11 = (unsigned char *)(DP1_BASE + DSP3SEL + PIRH);
    dspint12 = (unsigned char *)(DP2_BASE + DSP0SEL + PIRH);
    dspint13 = (unsigned char *)(DP2_BASE + DSP1SEL + PIRH);
    dspint14 = (unsigned char *)(DP2_BASE + DSP2SEL + PIRH);
    dspint15 = (unsigned char *)(DP2_BASE + DSP3SEL + PIRH);
	    
/* Set up BIM interrupt vector table in host CPU */

    if (intConnect((VOIDFUNCPTR *)(int_vec2 * 4), (VOIDFUNCPTR)bim_int2_isr,0         ) == ERROR)
      {
	fprintf(stderr, "failed to connect interrupt vector %d. Exiting.\n"                   ,int_vec2);
	exit(1);
      }
	    
/* Initialize the Control and Vector Registers in the Collator's BIM */

    bim_cr2 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg2);
    *bim_cr2 = 0xda;  /* set INT2* for interrupt level IRQ2* */
    /* (INT2* is DP out of sync interrupt) */
    /* NOTE: must write to register in interrupt handler */
    /*       to re-enable interrupt again                */
    stat = *bim_cr2;
    if(!Silent)
      printf("BIM control register 2 = %x \n",stat);
    
    bim_vr2 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg2);
    *bim_vr2 = int_vec2;
    stat = *bim_vr2;
    if(!Silent)
      printf("BIM vector register 2 = %x \n",stat);

/********************************************************************
 *  Program Digital IF's Here                                                *
 ********************************************************************/

    prog_digif(5,4,150,10);  /* program digifs so they don't free run */

    taskDelay(1);            /* delay */
/********************************************************************
 *  Start Collator and DSPQ DSP32C's HERE                           *
 ********************************************************************/

    dspgo(1,2,3,3);
    dspgo(1,2,3,2);
    dspgo(1,2,3,1);
    dspgo(1,2,3,0);
    dspgo(1,1,3,3);
    dspgo(1,1,3,2);
    dspgo(1,1,3,1);
    dspgo(1,1,3,0);
    
    taskDelay(1);
    
    dspgo(1,5,1,3);
    dspgo(1,5,1,2);
    dspgo(1,5,1,1);
    dspgo(1,5,1,0);
    
    dspgo(1,4,1,3);
    dspgo(1,4,1,2);
    dspgo(1,4,1,1);
    dspgo(1,4,1,0);
	    
    dspgo(1,3,1,3);
    dspgo(1,3,1,2);
    dspgo(1,3,1,1);
    dspgo(1,3,1,0);
    
    dspgo(1,2,1,3);
    dspgo(1,2,1,2);
    dspgo(1,2,1,1);
    dspgo(1,2,1,0);
    
    dspgo(1,1,1,3);
    dspgo(1,1,1,2);
    dspgo(1,1,1,1);
    dspgo(1,1,1,0);
	    
#ifdef INTR
	    
    /* Enable 68040 Interrupts */
	    
    sysIntEnable(2);
    
    while(!stop)
      {
	
    /* Test for DP Out of Sync Interrupt */
	
	sem_status = semTake(bim_int2_sem, 10); /* wait for ISR to pass sem */

	if (sem_status == OK)
	  {
	    
	    /* 
	       Determine which DP went out of sync by reading sync flag status back from all operating DP DSP32C's 
*/
	    /* Update Status */
			
	    /*			printf("Doppler Processor Out of Sync Error \n");  */
	    taskDelay(2);     /* delay until all DP's have interrupted */
	    *bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            	
	    e_time = tickGet()/3600;
	    printf("SYSTEM FAILED AFTER %d MINUTES \n",e_time);
	    for(i=1;i<=f_flag;i++)
	      {
		temp_stat[i-1] = ppp_sync(i,1);
	      }
	    dp_stat[0] = dp_sync(1,3);
	    dp_stat[1] = dp_sync(2,3);
	    
	  }
	
      }
     
/* Stop Test */
#ifdef NEW_TM
    timoffb();
#endif
#ifndef NEW_TM
    timoff();
#endif

    dspstop(1,1,3,-1);
    dspstop(1,2,3,-1);
    
    dspstop(1,1,1,-1);
    
    dspstop(1,2,1,-1);
    
    dspstop(1,3,1,-1);
    
    dspstop(1,4,1,-1);
    
    dspstop(1,5,1,-1);


    /* Clear DP Out of Sync Interrupt that may occur as a result of the 
       stoppage  */

    sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

    if (sem_status == OK)
      {
	*bim_cr2 = 0xda;   /* re-enable interrupt INT2* */
      }
    
/* Disable all interrupts */

    sysIntDisable(2);
    
    /* Check Operational Flag Status and clear any flags that are set */
    
    if(stop)
      {
	stop = 0;
	printf("Bus Test Stopped \n");
      }
#endif 
  }
}
void bim_int2_isr()
  {
      unsigned char  clr_pif;
      
      clr_pif = *dspint11;
      clr_pif = *dspint10;
      clr_pif = *dspint9;
      clr_pif = *dspint8;
      clr_pif = *dspint15;
      clr_pif = *dspint14;
      clr_pif = *dspint13;
      clr_pif = *dspint12;
      semGive(bim_int2_sem);
  }

void dma_data(unsigned long frq, unsigned long bd)

{
  int i,j;
  unsigned char *pio, scr;
  unsigned short *pio16;
  
  frq = (frq << 24) + EXTD_BASE;   /* freq # * 10**6 + EXTD_BASE*/
  bd = bd << 20;                  /* Board # * 10**5 = base addr */
  bd += frq;                      /* Full base addr. of board */
  for(i=0;i<4;i++)
    {
      
/* 
   set up DSPQ PCR and PIO registers for 24 bit addressing,
   32 bit DMA xfers, and a 16 bit Parallel I/O Port (Remember PIO Port
   defaults to 8 bit I/F after reset). NOTE: MSBYTE on VME is LSBYTE on
   DSP32C !!!
*/

      pio = (unsigned char *)(bd + (DSPSEL + i * DSPOFF));
      pio16 = (unsigned short *)(bd + (DSPSEL + i * DSPOFF));
      if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
      *(pio + PCRL) = (REGMAP + DMA + AUTO);
      *(pio + PCRH) = (DMA32 + PIO16);
      *(pio16 + PARE16) = 0x0;
      *(pio16 + PAR16) = 0x1000;

/* write data array into DSP memory starting at 0x1000 */

      for (j = 0; j < Fifo_sz/4; j++)
	{
	  *(pio16 + PDR216) = 0xa5a5;
	  *(pio16 + PDR16) = 0xa5a5;
	  *(pio16 + PDR216) = 0x5a5a;
	  *(pio16 + PDR16) = 0x5a5a;
	  *(pio16 + PDR216) = 0x0000;
	  *(pio16 + PDR16) = 0x0000;
	  *(pio16 + PDR216) = 0xffff;
	  *(pio16 + PDR16) = 0xffff;
	}
	  
    }
}
