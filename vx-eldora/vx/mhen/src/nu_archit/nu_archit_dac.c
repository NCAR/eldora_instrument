/*
 *      $Id$
 *
 *      Module: NU_ARCHIT_DAC
 *      Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.6  1996/02/23  18:04:25  eric
 * modified code to support test pulse (slow cal).
 *
 * Revision 1.5  1995/01/25  17:38:01  eric
 * Initialized global Header pointers in this routine only to eliminate
 * conflicts.
 *
 * Revision 1.4  1995/01/20  14:52:02  eric
 * Put in patch to fix bug in PPP scale_fac in aft processor.
 *
 * Revision 1.3  1994/11/14  17:25:55  eric
 * Updated code to run Staggered PRT mode.
 *
 * Revision 1.2  1994/07/14  20:33:17  eric
 * Simplified task interaction by doing End of Beam error
 * detection in nu_arch_data_xfer.c
 *
 *
 *
 * description: This module provides executive control of the "NEW" ELDORA 
 *              Architecture RP7 Data Acquisition function for up to 5 
 *              frequencies. Control is established by reading a header passed
 *              by the Control Processor. This module controls the modified
 *              ELDORA processing slice for variable frequency operation. 
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
#define int_vec0 255
#define int_vec1 254
#define int_vec2 253
#define int_vec3 252
#define DATAX    0x0            /* Offset to X axis Data Byte */
#define STATUSX  0x2            /* Offset to X axis status byte */
#define POS_OFF  0              /* Offset from "true" North in counts */
#define ENC_CNTS 2048           /* maximum number of encoder counts */  
#define FORE_MASK 0x1

#define ELDORA 0
#define NEW 0
#define OLD 1
#define MB  0x1
#define EOB 0x2
#define DP  0x4
#define COL 0x8

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
                      *dspint19,*dspq0_dsp0_pcr, *coll0_dsp0_pcr, clr_pif,
                      clr_pdf, *mdbm_int;

volatile static    char   *data = (char *)(DMC_BASE + DATAX);
volatile static    char   *gal_status = (char *)(DMC_BASE + STATUSX);
static    latchpos = 0;
static    unsigned char table[]={0x0,0x1,0x2,0x3,0x4,0x5,0x6,0x7,
				 0x8,0x9,0x0,0x0,0x0,0x0,0x0,0x0,
                                 0x0,0xA,0xB,0xC,0xD,0xE,0xF};

volatile static int  f1_flag, f2_flag, f3_flag, f4_flag, f5_flag;
static char *mb_clr;

dac()
{


long           h, i, j, k, l, q, p, r, num_coll_int, num_dspq_int, tst_pls, tp_width;


volatile unsigned char   *bim_vr0, *bim_vr1, *bim_vr2, *led, *pio_acc,
               *bim_vr3, stat, *coll_sync;

int            *samps, data_samps, buff_cnt, prf, gates, num1, gate_sp, shortcnt,
               first_gate, sem_status, sampl, load_stat, dp_stat[2], coll_stat,
               chips[6], mcpl_stat, prt_flag, ppp_stat, dc_flag, d_sampl,
               pick_gate, f_flag, taskid, dc_stat, temp_stat[5], pcpcnt[10];

unsigned int   num_int1, num_int2, num_int3, num_int4, chip_avg1, chip_avg2,
               chip_avg3, chip_avg4, coll_pick_gate;
float          duty_cycle, rpt_seq, frq1, frq2, frq3, fake_posit,scale_fac,
               dwell_time, proc_const, short_prt, pcp, repeat_seq, Pnoise,
               conv_gain, avg_scale;

short          num_param;


unsigned short          total_pcp;

volatile unsigned long  *tdpl_status, freq, board;

volatile short ray_hndshk_stat, *digif_gates;

unsigned long e_time;

double         n_frq, Pn;


void galil();
void bim_int0_isr();
void bim_int1_isr();
void bim_int2_isr();
void bim_int3_isr();
int posit();

vme2_pntr = (struct vmevmehndshk *)(VMEMEM_BASE + STD_BASE);

#ifdef PIO_CARD

pio_acc = (unsigned char *)(PIO_BASE + IO_CONTROL);
*pio_acc = 0x1;                  /* Enable Port A Output */
pio_acc = (unsigned char *)(PIO_BASE + IO_PORTA);
*pio_acc = 0x0;                  /* Initialize Port A to Zero */
*pio_acc = 0x80;                 /* Set bit A7 to one */

#endif

for(;;)
  {

      sem_status = semTake(exec_sem,WAIT_FOREVER); /* wait for start semaphore  */
      if(sem_status == OK)
	{

      /* Clear out Stop Flag */

	    stop = 0;
	    int_cnt = 0;  /* reset interrupt counter */
	    ppp_stat = 0;
	    proc_stat = 0;
	    err_cnt = 0;
	    max_time = 0;
            tickSet(0);   /* reset tick counter */

      /* Read 68040 status register to determine if FORE or AFT radar */

	    tdpl_status = (unsigned long *)(STAT_REG);
	    radar_fore_aft = *tdpl_status >> 24;
            if (!(radar_fore_aft & FORE_MASK))
              radar_fore_aft = 0;
            else
              radar_fore_aft = 1;
	    printf("radar_fore_aft = %x \n",radar_fore_aft);

     /* Initialize mcpl hanshake indeces to zero */

	    l = 0;
	    q = 0;
	    p = 0;
	    r = 0;

     /* Reset send_ray, send_nav, send_ads handshake flags */

	    send_ads = 0;
	    send_nav = 0;
	    for(i = 0; i < 3; i++)
	      {
		  for(j = 0; j < 2; j++)
		    {
			send_ray[i][j] = 0;
		    }
	      }

     /* Initialize status block to good status */

	    currStatus->rp7 = 0;
	    currStatus->mcpl = 0;
	    currStatus->status = 0;
	    currStatus->mailbox = 0;

      /* Initialize Global Header Pointers */

            wvfm = GetWaveform(inHeader);
	    if(!radar_fore_aft)
                {
                    rdsc = GetRadar(inHeader,1);
                    cs = GetCellSpacing(inHeader,1);
                    fldrdr = GetFieldRadar(inHeader,1);
                }
	    if(radar_fore_aft)
                {
                    rdsc = GetRadar(inHeader,2);
                    cs = GetCellSpacing(inHeader,2);
                    fldrdr = GetFieldRadar(inHeader,2);
                }
       
      /* Send Start Semaphore to Data Transfer Task */

            semGive(real_sem);
            
      /* Parse Header for required system parameters */

	    gates = wvfm -> num_gates[0];
	    num1 = gates - 3;
	    gate_sp = wvfm -> gate_dist1[1];
	    gate_sp = (gate_sp * 2.5) + 0.5; /* convert 60 mhz counts to m and round */
            first_gate = cs -> distToFirst; /* Assumes same first gate spacing for all frequencies */
	    chips[0] = wvfm -> num_chips[0];
	    f1_flag = wvfm -> num_chips[0];
	    prt_flag = 0;    /* default is single prt */
	    if(f1_flag > 1)
	      {
		  f1_flag = 1;
		  prt_flag = 1;
	      }
	    f2_flag = wvfm -> num_chips[1];
	    if(f2_flag > 1)
		  f2_flag = 1;
	    f3_flag = wvfm -> num_chips[2];
	    if(f3_flag > 1)
		  f3_flag = 1;
	    f4_flag = wvfm -> num_chips[3];
	    if(f4_flag > 1)
		  f4_flag = 1;
	    f5_flag = wvfm -> num_chips[4];
	    if(f5_flag > 1)
		  f5_flag = 1;
	    f_flag = f1_flag + f2_flag + f3_flag + f4_flag + f5_flag;
	    n_frq = (double)f_flag;
	    repeat_seq = wvfm -> repeat_seq;
	    total_pcp = (unsigned short)(wvfm -> total_pcp);
	    pcp = repeat_seq * 1.0E-3 / total_pcp;
	    h = 0;
	    pcpcnt[h] = 0;
	    for(i=0; i<total_pcp; i++)
	      {
		  if((wvfm -> blank_chip[i] & 0x01) == 0x01)
		    {
			h++;
			if(h > 9 ) h = 9;
			pcpcnt[h] = 1;
		    }
		  else
		    pcpcnt[h]++;
	      }
	    pcpcnt[h] = pcpcnt[h] + pcpcnt[0];
	    shortcnt = pcpcnt[1];
	    if(h >= 2)
	      {
		  for(i=2; i<(h+1); i++)
		    if(pcpcnt[i] < shortcnt) shortcnt = pcpcnt[i];
	      }
	    short_prt = pcp * shortcnt;
	    prf = 1.0 / short_prt;
            sampl = wvfm -> repeat_seq_dwel;

	    frq1 = rdsc -> freq1;
	    frq2 = rdsc -> freq2;
	    frq3 = rdsc -> freq3;

	    num_param = rdsc -> num_parameter_des;

            num_int1 = cs -> num_cells[0];
            num_int2 = cs -> num_cells[1];
            num_int3 = cs -> num_cells[2];
            num_int4 = cs -> num_cells[3];
	    chip_avg1 = (float)(cs -> spacing[0])/(float)(gate_sp) + 0.5;
	    chip_avg2 = (float)(cs -> spacing[1])/(float)(gate_sp) + 0.5;
	    chip_avg3 = (float)(cs -> spacing[2])/(float)(gate_sp) + 0.5;
	    chip_avg4 = (float)(cs -> spacing[3])/(float)(gate_sp) + 0.5;
	    pick_gate = fldrdr -> time_series_gate; /* must be divisible by 8 !!! */
	    if(!Silent)
	      printf("pick_gate = %d \n",pick_gate);

	    coll_pick_gate = fldrdr -> indep_freq_gate;

/* Scale Noise Power Appropriately for Doppler Processor */
	    
	    avg_scale = 0.0;
	    Pnoise = rdsc -> noise_power;
	    Pnoise -= rdsc -> radar_const;
	    Pn = pow(10.0,((double)(Pnoise)/10.0));
            Pnoise = Pn;
	    printf("Pnoise = %g \n",Pnoise);

/* Calculate antenna r.p.m. based on values in header */

	    rpm = (float )((prf/sampl)*(0.9*60.0/360.0)); /* scan @ 1 ray per beamwidth */
#ifdef NEW_TM
#ifndef T_PULSE
	    tst_pls = ((gates - 2) * gate_sp) + gate_sp/2; /* ensure test pulse spans gates - 1 range gate (last gate for processor 2) */

	    tp_width = 2 * gate_sp; /* can change width factor from 2 if req'd */

	    testpulseb(prf,1,0,tst_pls,tp_width); /* freq1 testpulse */
#endif
#endif            
#ifndef NEW_TM
	    tst_pls = gate_sp * 10;   /* for old timing module */
#endif
	    mdbm_int = (unsigned char*)(SHORT_BASE + VMEVME_BASE + CMD_REG);

/*	    if(!dc_removal)
              {
*/
                  mcpl_stat = mcpl_init();
                  if(mcpl_stat == 0 || mcpl_stat == 2)
                    printf("mcpl initialization failed \n");

                  /* Transfer MAD */
            
                  mcpl_stat = mad_xfer();
                  if(mcpl_stat == 2 || mcpl_stat == 5)
                    printf("mcpl MAD transfer failed \n");
            
            /* Initialize MCPL Data Transfer */
                  
                  init_data_xfer();
/*              }  */

	    if(dc_removal)
	      dc_flag = 1;
	    else
	      dc_flag = 0;

      /* Make sure Timing Module is off */

#ifdef NEW_TM	    
	    timoffb();
#endif
#ifndef NEW_TM
	    timoff();
#endif
	    if(load)
	      {
      
      /* Load Collator DSP32C's with appropriate Code */


#ifndef NEW_DSP_CODE      
		  load_stat = nu_lddsp(1,4,0,0,"col.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
                  if(Param != 6)
                    load_stat = nu_lddsp(1,4,0,0,"dlprt_col.dmp");
                  else
                    load_stat = nu_lddsp(1,4,0,0,"dlprt_col6v.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif      
      /* Load Doppler Processor with appropriate code */


#ifndef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,1,3,0,"dp_m.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,3,1,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,3,2,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,3,3,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,3,0,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,3,1,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,3,2,"dp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,3,3,"dp_d7.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
                  if(Param != 6)
                    load_stat = nu_lddsp(1,1,3,0,"dlprt_dp_m.dmp");
                  else
                    load_stat = nu_lddsp(1,1,3,0,"dlprt_dp6v_m.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,1,3,1,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,1,3,1,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,1,3,2,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,1,3,2,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,1,3,3,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,1,3,3,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,2,3,0,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,2,3,0,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)
                    load_stat = nu_lddsp(1,2,3,1,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,2,3,1,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,2,3,2,"dlprt_dp_s.dmp");
                  else
                    load_stat = nu_lddsp(1,2,3,2,"dlprt_dp6v_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
                  if(Param != 6)                  
                    load_stat = nu_lddsp(1,2,3,3,"dlprt_dp_d7.dmp");
                  else
                    load_stat = nu_lddsp(1,2,3,3,"dlprt_dp6v_d7.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif      


	    /* Load Pulse Pair Processor for F1 with appropriate code */

#ifndef NEW_DSP_CODE

		  load_stat = nu_lddsp(1,1,1,0,"ppp_m.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,1,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,2,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,3,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,1,1,0,"dlprt_ppp_m.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,1,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,2,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,1,1,3,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
			

/* Load Pulse Pair Processor for F2 with appropriate code */


#ifndef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,2,1,0,"ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,1,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,2,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,3,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,2,1,0,"dlprt_ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,1,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,2,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,2,1,3,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif

/* Load Pulse Pair Processor for F3 with appropriate code */

#ifdef FREQ_3
#ifndef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,3,1,0,"ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,1,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,2,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,3,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,3,1,0,"dlprt_ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,1,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,2,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,3,1,3,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#endif /* FREQ_3 */

/* Load Pulse Pair Processor for F4 with appropriate code */

#ifdef FREQ_4
#ifndef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,4,1,0,"ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,1,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,2,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,3,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,4,1,0,"dlprt_ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,1,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,2,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,4,1,3,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#endif /* FREQ_4 */

/* Load Pulse Pair Processor for F5 with appropriate code */

#ifdef FREQ_5
#ifndef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,5,1,0,"ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,1,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,2,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,3,"ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#ifdef NEW_DSP_CODE
		  load_stat = nu_lddsp(1,5,1,0,"dlprt_ppp_d0.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,1,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,2,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
		  load_stat = nu_lddsp(1,5,1,3,"dlprt_ppp_s.dmp");
		  if(load_stat)
		    {
			/* Update Status */
			
			currStatus->rp7 |= DSP_LOAD;			
		    }
#endif
#endif /* FREQ_5 */
	      }

	    load = 0;     /* Reset load flag */
/*	    printf("load = %d \n",load); */
	    n_frq = (double)(fldrdr -> scale_factor[0]);
	    scale_fac = n_frq/1.0;
	    printf("scale_fac = %f\n",scale_fac);
	    n_frq = (double)(fldrdr -> processor_const);
	    proc_const = n_frq/1.0;
	    printf("proc_const = %f\n",proc_const);
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

		  scale_fac = fldrdr -> scale_factor[1];
		  ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,2,1);

           /* Turn on PPP LED */
                  led = (unsigned char *)(PPP_F2_BASE + DLEDON);
		  *led = 0x0;
	      }
	    else
	      {
		  scale_fac = fldrdr -> scale_factor[1];
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

		  scale_fac = fldrdr -> scale_factor[2];
		  ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,3,1);

           /* Turn on PPP LED */
                  led = (unsigned char *)(PPP_F3_BASE + DLEDON);
		  *led = 0x0;
	      }
	    else
	      {
		  scale_fac = fldrdr -> scale_factor[2];
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

		  scale_fac = fldrdr -> scale_factor[3];
		  ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,4,1);

           /* Turn on PPP LED */
                  led = (unsigned char *)(PPP_F4_BASE + DLEDON);
		  *led = 0x0;
	      }
	    else
	      {
		  scale_fac = fldrdr -> scale_factor[3];
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

		  scale_fac = fldrdr -> scale_factor[4];
		  ppp_opflags(pick_gate,FREQ_ON,prt_flag,scale_fac,dc_flag,5,1);

           /* Turn on PPP LED */
                  led = (unsigned char *)(PPP_F5_BASE + DLEDON);
		  *led = 0x0;
	      }
	    else
	      {
		  scale_fac = fldrdr -> scale_factor[4];
		  ppp_opflags(pick_gate,FREQ_OFF,prt_flag,scale_fac,dc_flag,5,1);

	   /* Turn off PPP LED */
                  led = (unsigned char *)(PPP_F5_BASE + DLEDOFF);
		  *led = 0x0;

	      }

            gate_avg(num_int1,num_int2,num_int3,num_int4,chip_avg1,chip_avg2,chip_avg3,chip_avg4);

	    /* Set Collator operational flags */

       	    coll_opflags(prt_flag,f1_flag,f2_flag,f3_flag,f4_flag,f5_flag,coll_pick_gate);

      /* Initialize values for Sun3 Display */

	    position = 0;
	    k = 0;

      /* General Initialization */

	    fake_posit = 0.0;

/* Define PIF flags in PPP and Collator */

	    collint = (unsigned char *)(COL0BASE + DSP0SEL + PIRH);
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

	    
/* Initialize the Control and Vector Registers in the Collator's BIM */

	    bim_cr3 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg3);
	    stat = *bim_cr3;
	    
	    bim_cr0 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg0);
	    *bim_cr0 = 0xdc;  /* set INT0* for interrupt level IRQ4* */
	    /* INT0* is end of beam interrupt */
	    /* NOTE: must write to register in interrupt handler */
	    /* to re-enable interrupt again                */
	    stat = *bim_cr0;
	    if(!Silent)
	      printf("BIM control register 0 = %x \n",stat);
	    
	    bim_cr1 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg1);
	    *bim_cr1 = 0xdb;  /* set INT1* for interrupt level IRQ3* */
	    /* INT1* is mid-beam interrupt */
	    /* NOTE: must write to register in interrupt handler */
	    /*       to re-enable interrupt again                */
	    stat = *bim_cr1;
	    if(!Silent)
	      printf("BIM control register 1 = %x \n",stat);
	    
	    bim_cr2 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg2);
	    *bim_cr2 = 0xda;  /* set INT2* for interrupt level IRQ2* */
	    /* (INT2* is DP out of sync interrupt) */
	    /* NOTE: must write to register in interrupt handler */
      /*       to re-enable interrupt again                */
	    stat = *bim_cr2;
	    if(!Silent)
	      printf("BIM control register 2 = %x \n",stat);
	    
	    bim_cr3 = (unsigned char *)(COL0BASE + BIMSEL + cntrl_reg3);
	    *bim_cr3 = 0xd9;  /* set INT3* for interrupt level IRQ1* */
	    /* INT3* is Collator out of sync interrupt) */
	    /* NOTE: must write to register in interrupt handler */
	    /*       to re-enable interrupt again                */
	    stat = *bim_cr3;
	    if(!Silent)
	      printf("BIM control register 3 = %x \n",stat);
	    
	    bim_vr0 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg0);
	    *bim_vr0 = int_vec0;
	    stat = *bim_vr0;
	    if(!Silent)
	      printf("BIM vector register 0 = %x \n",stat);
	    
	    bim_vr1 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg1);
	    *bim_vr1 = int_vec1;
	    stat = *bim_vr1;
	    if(!Silent)
	      printf("BIM vector register 1 = %x \n",stat);
	    
	    bim_vr2 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg2);
	    *bim_vr2 = int_vec2;
	    stat = *bim_vr2;
	    if(!Silent)
	      printf("BIM vector register 2 = %x \n",stat);

	    bim_vr3 = (unsigned char *)(COL0BASE + BIMSEL + vec_reg3);
	    *bim_vr3 = int_vec3;
	    stat = *bim_vr3;
	    if(!Silent)
	      printf("BIM vector register 3 = %x \n",stat);

/********************************************************************
 *  Program Timing Module Here                                      *
 ********************************************************************/
#ifdef NEW_TM

	    ld_timmod(ELDORA,NEW);
	    write_coll(first_gate,gate_sp);

#endif
#ifndef NEW_TM
/* Old timing module Calls */ 

	    ld_timmod(ELDORA,OLD);
	    write_coll(first_gate,gate_sp);

#endif
/********************************************************************
 *  Program Digital IF's Here                                                *
 ********************************************************************/
/*  single channel digif in Freq 1
	    digif_gates = (short *)(ATOD1BASE);
	    *digif_gates = gates - 1;
*/
	    d_sampl = sampl * wvfm -> num_chips[0];
	    for(i=1;i<=f_flag;i++)
	      prog_digif(i,gates,gate_sp,d_sampl); /* program new digif */
	    taskDelay(1);                          /* delay */
/********************************************************************
 *  Start Collator and DSPQ DSP32C's HERE                           *
 ********************************************************************/

/*	    sysIntEnable(4);  try moving to different location */
	    dspgo(1,4,0,0);

            taskDelay(1);
            
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


/********************************************************************
 * Start azimuth scan                                               *
 ********************************************************************/

#ifdef LAB      
	    if(!radar_fore_aft)      
	      slow(rpm);
	    taskDelay(300); /* Add 3 seconds of delay to allow ant to rotate to 0deg */
#endif
/********************************************************************
 *  Start Timing Module Here                                        *
 ********************************************************************/
     /* Try a task delay here to ensure all processors are done executing
        initializations
     */
	    taskDelay(10);
            /*
              Wait for Housekeeping Processor to initialize structure
              in buffer memory
            */

            j = 0;
/*          taskid = taskIdSelf(); */
/*          if(taskPrioritySet(taskid,175)!= OK) */ /* lower task's priority so
                                                          that tight loop doesn't impede other tasks */
/*          printf("invalid task id \n"); */
            vme2_pntr -> start_hndshk = 1;  /* handshake w/ housekeeper for synchronized start */

            while(strncmp(vme2_pntr -> salute,"HI RADAR",8)!= 0)
                    {
                        j++; 
                    }

/*                if(taskPrioritySet(taskid,100)!= OK) *//* return task to origi
nal priority */
/*                  printf("invalid task id \n"); */


#ifdef NEW_TM
	    timonb();
#endif
#ifndef NEW_TM
	    timon();
#endif

	    semGive(exec_int0_sem);

/* Enable 68040 Interrupts */

	    sysIntEnable(4);
	    sysIntEnable(1);
	    sysIntEnable(2);
	    sysIntEnable(3);

	    while(!(stop || reboot))
	      {
      /* Wait 0.3 seconds maximum for MID-BEAM INTERRUPT */

		  sem_status = semTake(bim_int1_sem, 30); /* wait 30 ticks for ISR to pass sem */
		  if (sem_status == OK)
		    {
#ifdef T_PULSE
                  /* Update Testpulse Parameters -- if required */
     
                        if(vme2_pntr->tpulse_flg == 1)
                          {
                              testpulseb(prf,!radar_fore_aft,vme2_pntr->tpulse_freq_num-1,vme2_pntr->tpulse_dist,vme2_pntr->tpulse_width);
                              vme2_pntr->tpulse_flg = 0;
                          }
#endif
                        
                  /* Send data across mcpl at mid-beam */

                        /* check for valid NAV data */

			if(send_nav)
			  {
			      send_nav = 0;
			      broadcast_data(curr_nav_add,curr_nav_mailbox_add,vme2_pntr->nav_length);
			  }
                                    
                              /* check for valid ADS data */
                              
			else if(send_ads)
			  {
			      send_ads = 0;
			      broadcast_data(curr_ads_add,curr_ads_mailbox_add,vme2_pntr->ads_length);
			  }
			else
			  {

			      if(vme2_pntr -> radar_hndshk[l] == 1)
				{
				    for(i=0;i<3;i++)
				      {
					  if(vme2_pntr -> radar_hndshk[l] == 1)
					    {
						curr_ray_add[p][r] = (long)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (l * DATA_RAY_OFFSET));
						curr_mailbox_add[p][r] = (long)(&vme2_pntr -> radar_hndshk[l]); 
						broadcast_data(curr_ray_add[p][r],curr_mailbox_add[p][r],logical_length);
#ifdef PIO_CARD
                                               /* Toggle bit 7 of PIO port A */
						
						*pio_acc = 0x80; 
						*pio_acc = 0x0;
#endif
                                                 
/*						ray->fielddata.ray_count = l + (q * 27); */
						p = p<2?p+1:0;
						l = l<26?l+1:0;
					    }
					  else
					    i = 3; /* exit loop if fail any test */
				      }

				    /*  r = r<1?r+1:0; */
				}

			  }

		    }

		  else
		    {
		  
		  /*  printf("ERROR:  NO MID-BEAM INTERRUPT RECEIVED \n"); */
			int_cnt++;
			/* Update Status */
			
			currStatus->rp7 |= MID_BEAM;
			proc_stat |= MB;
	            }
		    
      /* Wait 0.3 seconds for End of Beam Interrupt */

/*		  sem_status = semTake(exec_int0_sem, 30); *//* wait 30 ticks for ISR to pass sem */
/*
		  if (sem_status != OK)
		    {
*/
/*			printf("ERROR:  NO END OF BEAM INTERRUPT RECEIVED \n"); */
/*			int_cnt++;			*/
			/* Update Status */
/*			
			currStatus->rp7 |= END_OF_BEAM;
			proc_stat |= EOB;
		    }
*/
      /* Test for DP Out of Sync Interrupt */

		  sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

		  if (sem_status == OK)
		    {

/* 
Determine which DP went out of sync by reading sync flag status back from all operating DP DSP32C's 
*/
			/* Update Status */
			
			currStatus->rp7 |= DP_SYNC;
			proc_stat |= DP;
			int_cnt++;
/*			printf("Doppler Processor Out of Sync Error \n");  */
			taskDelay(2);     /* delay until all DP's have interrupted */
			*bim_cr2 = 0xda;  /* re-enable interrupt INT2* */            	
		    }
      /* Test for Collator Out of Sync Interrupt */

		  sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
		  if (sem_status == OK)
		    {
			*bim_cr3 = 0xd9;  /* re-enable interrupt INT3* */            
			/* Update Status */

			currStatus->rp7 |= COLL_SYNC;
			proc_stat |= COL;
			int_cnt++;
/*			printf("Collator Out of Sync Error \n"); */
		    }
		  if(int_cnt >= 1 && int_cnt <= 4 && !stop)
		    {		    
			int_cnt = 5;
			switch(proc_stat)
			  {
			    case 1:
			      printf("ERROR:  NO MID-BEAM INTERRUPT RECEIVED \n");
			      printf("CHECK FREQ1 DIGIF. \n");
			      break;
			    case 2:
			    case 3:
			    case 4:
			    case 5:
			    case 6:
			    case 7:
			    case 8:
			    case 9:
			    case 10:
			    case 11:
			    case 12:
			    case 13:
			    case 14:
			      if(proc_stat & MB)
				printf("ERROR:  NO MID-BEAM INTERRUPT RECEIVED \n");
			      if(proc_stat & EOB)
				printf("ERROR:  NO END OF BEAM INTERRUPT RECEIVED \n");
			      if(proc_stat & DP)
				printf("ERROR:  DOPPLER PROCESSOR OUT OF SYNC \n");
			      if(proc_stat & COL)
				printf("ERROR:  COLLATOR OUT OF SYNC \n");
			      for(i=1;i<=f_flag;i++)
				{
				    temp_stat[i-1] = ppp_sync(i,1);
				    ppp_stat |= temp_stat[i-1];
				}
			      dp_stat[0] = dp_sync(1,3);
			      dp_stat[1] = dp_sync(2,3);
			      coll_stat = col_sync();
			      if(coll_stat && !ppp_stat && !dp_stat[0] && !dp_stat[1])
				printf("CHECK COLLATOR. \n");
			      if(!ppp_stat && dp_stat[0])
				printf("CHECK DOPPLER PROCESSOR #1. \n");
			      if(!ppp_stat && dp_stat[1])
				printf("CHECK DOPPLER PROCESSOR #2. \n");
			      if(ppp_stat)
				{
				    for(i=1;i<=f_flag;i++)
				      {
					  if(temp_stat[i-1])
					    printf("CHECK FREQ %d PULSE PAIR PROCESSOR \n",i);
				      }
				}
			      if(proc_stat & MB)
				{
				    printf("CHECK FREQ 1 DIGIF. \n");
				    if(proc_stat & EOB)
				      {
					  printf("CHECK 60MHz. \n");
					  printf("CHECK COLLATOR BIM. \n");
					  printf("CHECK TIMING MODULE. \n");
				      }
				}
			      break;
			    case 15:
			      printf("PUNT. \n");
			      break;

			  }	
                        e_time = tickGet()/6000;
                        printf("SYSTEM FAILED AFTER %d MINUTES \n",e_time);

		    }

	      }
     
/* Stop Radar Data Processor */
#ifdef NEW_TM
	    timoffb();
#endif
#ifndef NEW_TM
	    timoff();
#endif
	    dspstop(1,4,0,0);

	    dspstop(1,1,3,-1);
	    dspstop(1,2,3,-1);

	    dspstop(1,1,1,-1);
	    
	    dspstop(1,2,1,-1);

	    dspstop(1,3,1,-1);

	    dspstop(1,4,1,-1);

	    dspstop(1,5,1,-1);

/* Reset DC Offset Flags */
	    
	    dc_flag = 0;
	    dc_removal = 0;

      /* Clear DP Out of Sync Interrupt that may occur as a result of the 
	 stoppage  */

	    sem_status = semTake(bim_int2_sem, NO_WAIT); /* don't wait for ISR to pass sem */

	    if (sem_status == OK)
	      {
/*		  *bim_cr2 = 0xda;   re-enable interrupt INT2* */            
	      }
      /* Clear Collator Out of Sync Interrupt that may occur as a result of 
         the stoppage */

	    sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */
	    if (sem_status == OK)
	      {
/*		  *bim_cr3 = 0xd9;  re-enable interrupt INT3* */            
	      }
/* 
    Try taking mid and end of beam semaphores in case they
    were given prior to Stop command and not taken
*/

/*	    sem_status = semTake(exec_int0_sem,NO_WAIT); */
	    sem_status = semTake(bim_int1_sem,30);
	    if(sem_status == OK)
	      /* Write to Freq 1 DIGIF to clear Mid-Beam interrupt */
	      {
		  mb_clr = (char *)(DIGIF1_BASE + MDBM_CLR);
		  *mb_clr = 0;
		  *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */
	      }
/*	    taskDelay(7); this ensures real time task has enough time to stop */

/* Disable all interrupts */

	    sysIntDisable(1);
	    sysIntDisable(2);
	    sysIntDisable(3);
	    sysIntDisable(4);

/* reset dc removal handshake flag */

/*          vme2_pntr -> dc_remove_hndshk = 1; */

/* Check Operational Flag Status and clear any flags that are set */

	    if(reboot)
	      reboot = 0;
	    if(stop)
                {
/*                    stop = 0; */
                    printf("Executive Task Stopped \n");
                }
	}
      else
	{
	    printf("PANIC: BAD REAL TIME SEMAPHORE \n");
	    exit(3);
	}
  }
}
void bim_int0_isr()
  {
      unsigned char  clr_pif, pcr_status;

      clr_pif = *collint;
      semGive(bim_int0_sem);
/*      semGive(exec_int0_sem); */
  }
void bim_int1_isr()
  {

/* Write to Freq 1 DIGIF to clear Mid-Beam interrupt */

      mb_clr = (char *)(DIGIF1_BASE + MDBM_CLR);
      *mb_clr = 0;
      
      *bim_cr1 = 0xdb;  /* re-enable interrupt INT1* */

     /* Interrupt House Keeping Processor via polled interrupts */
      vme2_pntr -> polled++; /* for polled interrupts */      

      /* Interrupt House Keeping Processor via */
      /* VME to VME interface if FORE Processor  */
      if(!radar_fore_aft)
	*mdbm_int = 0x20;  

      semGive(bim_int1_sem);

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
void bim_int3_isr()
  {
      unsigned char  clr_pdf, *coll_sync;
      coll_sync = (unsigned char *)(COL0BASE + DSP0SEL + PDRH);
      clr_pdf = *coll_sync;
      semGive(bim_int3_sem);
      
  }


void  galil()
{
  char   c;
  int   ang,pos,tp();
  
  /* get the latch position */
  while(!(*gal_status & 2));    *data = 'T';
  while(!(*gal_status & 2));    *data = 'S';
  while(!(*gal_status & 2));    *data = ';';
  
  for(latchpos=0; 1;)          /* read latched position */
    {
      while(*gal_status & 1);      /* wait for a character in buffer */
      c = *data;
      if(c == '?' || c == ':')    break;
      if(c != '\r' && c != '\n')  /* ignore line feed and carriage return */
	latchpos = (latchpos<<4) | *(table + (c - '0'));
    }
  
  /* enable latch for next revolution */
  while(!(*gal_status & 2));      *data = 'L';
  while(!(*gal_status & 2));      *data = 'T';
  while(!(*gal_status & 2));      *data = '1';
  while(!(*gal_status & 2));      *data = ';';
  while(*gal_status & 1);         c = *data;   /* wait for response */
  
  /*  get current pointing angle */
  while(!(*gal_status & 2));   *data = 'T';
  while(!(*gal_status & 2));   *data = 'P';
  while(!(*gal_status & 2));   *data = ';';
  
  for(ang=0; 1;)
    {
      while(*gal_status & 1);      /* wait for a character to appear in buffer */
      c = *data;
      if(c == '?' || c == ':')    break;
      if(c != '\r' && c != '\n')  /* ignore line feed and carriage return */
	ang = (ang<<4) | *(table + (c - '0'));
    }
  
  pos = (ang - latchpos) & 0xFFFFFF;          /* subtract true north */
  
  pos |= (pos & 0x800000)?0xFF000000:0;  /* sign extend negative numbers */
  
  if(pos > ENC_CNTS-1)        /* if antenna has spun around alot lately  */
    {
      pos %= ENC_CNTS;      /* make sure the position is within max encoder counts */
      latchpos = ang - pos;   /* approximate a good location for latchpos */
    }
  else if(pos < 0)        /* if antenna has spun around alot lately  */
    {
      pos = (pos%ENC_CNTS) + ENC_CNTS; /* make sure the position is within max encoder counts */
      latchpos = ang - pos; /* approximate a good location for latchpos */
    }
}

int  posit()
{
  char   c;
  int    ang,garbage;
  /*
    garbage = 0;
    while(!(*status & 1)) garbage = *data;  remove any spurious read data */
  
  while(!(*gal_status & 2));   *data = 'T';
  while(!(*gal_status & 2));   *data = 'P';
  while(!(*gal_status & 2));   *data = ';';
  
  for(ang=0; 1;)
    {
      while(*gal_status & 1);      /* wait for a character to appear in buffer */
      c = *data;
      if(c == '?' || c == ':')    break;
      if(c != '\r' && c != '\n')  /* ignore line feed and carriage return */
	ang = (ang<<4) | *(table + (c - '0'));
    }
  
  ang = (ang - latchpos) & 0xFFFFFF;          /* subtract true north */
  
  ang |= (ang & 0x800000)?0xFF000000:0;  /* sign extend negative numbers */
  
  if(ang > ENC_CNTS-1)    /* if just past 0 */
    {
      ang -= ENC_CNTS;   /* do modulus so  0 <= ang < max encoder counts  */
      
      /* get new latch position */
      while(!(*gal_status & 2));    *data = 'T';
      while(!(*gal_status & 2));    *data = 'S';
      while(!(*gal_status & 2));    *data = ';';
      
      for(latchpos=0; 1;)          /* read latched position */
	{
	  while(*gal_status & 1);      /* wait for a character in buffer */
	  c = *data;
	  if(c == '?' || c == ':')    break;
	  if(c != '\r' && c != '\n')  /* ignore line feed and carriage return */
            latchpos = (latchpos<<4) | *(table + (c - '0'));
	}
      
      /* enable latch for next revolution */
      while(!(*gal_status & 2));      *data = 'L';
      while(!(*gal_status & 2));      *data = 'T';
      while(!(*gal_status & 2));      *data = '1';
      while(!(*gal_status & 2));      *data = ';';
      while(*gal_status & 1);         c = *data;   /* wait for response */
    }
  else if(ang < 0)    /* if just past 0 the other direction */
    {
      ang += ENC_CNTS;   /* do modulus so  0 <= ang < max encoder counts  */
      
      /* get new latch position */
      while(!(*gal_status & 2));    *data = 'T';
      while(!(*gal_status & 2));    *data = 'S';
      while(!(*gal_status & 2));    *data = ';';
      
      for(latchpos=0; 1;)          /* read latched position */
	{
	  while(*gal_status & 1);      /* wait for a character in buffer */
	  c = *data;
	  if(c == '?' || c == ':')    break;
	  if(c != '\r' && c != '\n')  /* ignore line feed and carriage return */
            latchpos = (latchpos<<4) | *(table + (c - '0'));
	}
      
      latchpos = (latchpos - ENC_CNTS) & 0xFFFFFF;  /* as if going clockwise */
      
      /* enable latch for next revolution */
      while(!(*gal_status & 2));      *data = 'L';
      while(!(*gal_status & 2));      *data = 'T';
      while(!(*gal_status & 2));      *data = '1';
      while(!(*gal_status & 2));      *data = ';';
      while(*gal_status & 1);         c = *data;   /* wait for response */
    }
  ang -= POS_OFF; 
  return(ang);
}
