/*
 *	$Id$
 *
 *	Module:	NU_ARCH_DATA_XFER	 
 *	Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1996/06/28  21:35:16  eric
 * Added code to fill raw data structure with appropriate data.
 *
 * Revision 1.7  1996/02/23  18:03:27  eric
 * modified code to support test pulse (slow cal).
 *
 * Revision 1.6  1995/03/01  23:01:20  eric
 * removed clearing out stop flag from initialization.
 *
 * Revision 1.5  1995/01/25  17:39:59  eric
 * Initialized global Header pointers from nu_archit_dac.c to
 * avoid conflicts.
 *
 * Revision 1.4  1995/01/20  14:52:57  eric
 * Added code to handle 10 variable and 6 variable data sorting.
 *
 * Revision 1.3  1994/11/14  17:26:43  eric
 * Updated code to handle Staggered PRT's and
 * variable parameter headers.
 *
 * Revision 1.2  1994/07/14  20:36:17  eric
 * Added End of Beam error detection.
 *
 * Revision 1.1  1994/04/22  19:56:20  eric
 * Initial revision
 *
 * Revision 1.2  1993/05/12  22:03:50  eric
 * Corrected "last gate" data buffer offsets.
 *
 * Revision 1.1  1992/11/09  16:07:45  eric
 * Initial revision
 *
 *
 * description: This module is intended to be spawned as a real time Vx 
 *              Works task upon system startup. It will run on every End 
 *              of Beam Interrupt until a stop command is issued by the
 *              Control Processor. The main function of this module is to
 *              transfer data from the Collator's Dual Port Memory to a
 *              structure set up in VME Buffer Memory. In addition, if 
 *              a time series flag flag is set, time series data for a
 *              given frequency will be placed in an array of dimension N, 
 *              where N is the desired number of time series points.
 *              In either case data will be broadcast out the MCPL.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE
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

#include "HeaderRpc.h"
#include "Parameter.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "RadarDesc.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "ELDRP7.h"
#include "LclCommands.h"
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

#define EOB 0x2
#define AVG
#define FFT_PTS 128
extern short task_sync;
extern int int_cnt, proc_stat;

coll_data_xfer()
{


long           k, l, m, p, q, r, s;

register long  i,j;

int            *samps, data_samps, buff_cnt, prf, gates,sem_status, sampl,
               prt_flag, f1_flag, f2_flag, ts_sampl, n, fta_freq, status, 
               f3_flag, f4_flag, f5_flag, f_flag, ts_off, ts_cnt, incr, 
               offset, lgate_off,indf_off, num_int1, num_int2, num_int3, 
               num_int4, tnum_int, lgate_off2, cnt, raw_d_off, data_len, 
               indep_freq_len, time_series_len, ts_len, bytes_per_cell, 
               num_cells, I, Q;

volatile short indf_ts, ray_hndshk_stat;

short          num_param;

unsigned short lgate[8];

unsigned long  data_loc_base, temp;

volatile register unsigned long *dptr;
volatile register unsigned long   *data_loc;
volatile register float *ts_data_loc,*raw_ptr;

float          a_dummy, t_scale, *array, *av_array, *b_array;

double wp;
volatile register float *ts_loc;
if (!(array = (float *)malloc(4*2000*sizeof(float))))
  printf("680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
if (!(b_array = (float *)malloc(4*2000*sizeof(float))))
  printf("680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
if (!(av_array = (float *)malloc(4*2000*sizeof(float))))
  printf("680X0 OUT OF MEMORY; ALLOCATION FAILED \n");
FFT_pts = FFT_PTS;  /* Initialize FFT size to 128 by default */

for(;;)
  {
      sem_status = semTake(real_sem,WAIT_FOREVER); /* wait for start semaphore  */
      if(sem_status == OK)
	{
	    task_sync = 0;

/* Parse Header for required system parameters */

	    gates = wvfm -> num_gates[0];
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
            lgate_off2 = (f_flag - 1)*8;
	    sampl = wvfm -> repeat_seq_dwel;
	    incr = ((f_flag * 8) - 4)/4;
            num_int1 = cs -> num_cells[0];
            num_int2 = cs -> num_cells[1];
            num_int3 = cs -> num_cells[2];
            num_int4 = cs -> num_cells[3];
	    tnum_int = num_int1 + num_int2 + num_int3 + num_int4;
            num_param = rdsc -> num_parameter_des;
            if(num_param == 10) /* we really have 12 parameters in collator */
              num_param = 12;

/* Calculate data length, independent frequency length, and time_series length i
n bytes */

	    num_cells = 0;
	    for (j=0; j < cs -> num_segments; j++)
	      num_cells += cs -> num_cells[j];
	    bytes_per_cell = 0;
	    for (j=0; j < rdsc -> num_parameter_des; j++)
	      {
		prm = GetParameter(inHeader,j);
		bytes_per_cell += prm -> binary_format;
	      }
	    data_len = num_cells * bytes_per_cell;

	    if(fldrdr -> indepf_times_flg > 0)
	      indep_freq_len = rdsc -> num_freq_trans * 8 * rdsc -> num_ipps_trans + sizeof(INDEP_FREQ);
	    else
	      indep_freq_len = 0;

	    if(fldrdr -> indepf_times_flg == 3)
	      time_series_len = rdsc -> num_freq_trans * 2 * 4 * wvfm -> repeat_seq_dwel * wvfm -> num_chips[0] + sizeof(TIME_SERIES);
	    else
	      time_series_len = 0;
	    if((fldrdr -> indepf_times_flg == 3))
	      ts_len = time_series_len;
	    else
	      ts_len = sizeof(TIME_SERIES);
            raw_d_off = data_len + indep_freq_len + sizeof(ray_i) + sizeof(platform_i) + sizeof(field_parameter_data) + ts_len;
/* Substitute code to process indf_ts here !!! */

	    indf_ts = fldrdr -> indepf_times_flg;

	    /* Calculate offset for Power Spectrum */

	    wp = 10.0 - 20.0 * log10((double)65535*(float)(wvfm->chip_width[0]/7.5));

      /* General Initialization */

	    data_loc = (unsigned long *)(COL0BASE + DB_1);
	    ts_off = (tnum_int * num_param * 2) + (f_flag * 4 * 2 *(prt_flag + 1)) + (f_flag * 4 * 2 * (prt_flag + 1));
	    data_samps = (tnum_int - 1) * num_param * 2 / 4;
	    buff_cnt = 0;
	    ts_cnt = 0;
	    cnt = 0;
	    fill_busy = 0;
	    n = 0;
	    k = 0;
	    l = 0;
	    m = 0;
	    p = 0;
	    r = 0;
	    s = 0;
	    q = 0;
            j = 0;
	    for(i=0; i<2*sampl;i++) /* Initialize av_array to 0 */
	      av_array[i] = 0.0;
	    switch(indf_ts)
	      {
		case 0:
		  data_samps = (tnum_int - 1) * num_param * 2 / 4;
		  break;
		case 1:
		  indf_off = 2 * (num_param * tnum_int + f_flag * 4 * (prt_flag + 1));
		  break;
		case 3:
		  indf_off = 2 * (num_param * tnum_int + f_flag * 4 * (prt_flag + 1));
		  ts_sampl = 2 * sampl * f_flag * (prt_flag + 1);
		  break;
	      }
     
     /* wait for timing module to start */

	    sem_status = semTake(exec_int0_sem,WAIT_FOREVER);

	    while(!(stop || reboot))
	      {
	    /* Wait 30 ticks for End of Beam Interrupt */
		  
		  sem_status = semTake(bim_int0_sem,30); /* wait 30 ticks for ISR to pass sem */
		  
		  if (sem_status == OK)
		    {
			*bim_cr0 = 0xdc; /* re-enable interrupt INT0* */            
			switch(vme2_pntr -> tpulse_freq_num)
			  {
			    case 1:
			      lgate_off = 0x0;
			      break;
			    case 2:
			      lgate_off = 0x2;
			      break;
			    case 3:
			      lgate_off = 0x4;
			      break;
			    case 4:
			      lgate_off = 0x6;
			      break;
			    case 5:
			      lgate_off = 0x8;
			      break;
			    default:
			      lgate_off = 0x0;
			      break;
			  }
			data_loc_base = (unsigned long)data_loc;
			/* Check Handshake Status of Housekeeper */			
			ray_hndshk_stat = vme2_pntr -> radar_hndshk[k];
			      
			if((unsigned short)ray_hndshk_stat != 0xAFFF)
			  {
			      if((((unsigned short)ray_hndshk_stat == 0) || ((unsigned short)ray_hndshk_stat == 1)) && cnt == 0)
				printf("m %4X \n",ray_hndshk_stat);
			      cnt = cnt<1000?cnt+1:0;				    
			      if((unsigned short)ray_hndshk_stat != 0 && (unsigned short)ray_hndshk_stat !=1)
				printf("m %4X \n",ray_hndshk_stat);

			      /* send error status to control processor */
			  }

			ray = (struct DATA_RAY*)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (k * DATA_RAY_OFFSET));
			raw_d = (struct RAW_D*)((char *)ray + raw_d_off);
			ray -> fielddata.ray_count = k + (q * 27);
			dptr = &(ray->data[0]);
			raw_ptr = &(raw_d->data[0]);
			i = data_samps;

			/* Pull data from Collator's Dual Port Ram */
			if(!prt_flag && num_param == 4)
                            {
                                while(i-- != 0)
                                    {
                                        *dptr++ = *data_loc++;
                                    }
                                data_loc++; /* increment past last gate */
                                data_loc++;
                                data_loc += lgate_off;
                                *dptr++ = *data_loc;
                        /* Write testpulse velocity to vme to vme handshake area */        
                                vme2_pntr->tpulse_vel = (*data_loc++ & 0xffff); 
                                *dptr++ = *data_loc;
                        /* Write testpulse power to vme to vme handshake area */
                                vme2_pntr->tpulse_level_proc = (*data_loc++ & 0xffff);			

                            }
                         if(num_param == 6)
                             {
                                 while(i-- != 0)
                                     {
                                         *dptr++ = *data_loc++;
                                     }
                                 data_loc++; /* increment past last gate */
                                 data_loc++;
                                 data_loc++;
                                 data_loc += (lgate_off*2);
                                 temp = *data_loc++;
                                 lgate[0] = temp & 0xffff;         /* Vs */
                                 lgate[1] = (temp >> 16) & 0xffff; /* Ws */
                                 temp = *data_loc++;
                                 lgate[2] = temp & 0xffff;         /* DBZs */
                                 lgate[3] = (temp >> 16) & 0xffff; /* NCPs */
                                /* data_loc += lgate_off2; */
                                 temp = *data_loc++;
                                 lgate[4] = temp & 0xffff;         /* Vl */
                                 lgate[5] = (temp >> 16) & 0xffff; /* Wl */
                                 temp = *data_loc++;
                                 lgate[6] = temp & 0xffff;         /* DBZl */
                                 lgate[7] = (temp >> 16) & 0xffff; /* NCPl */
                                 *dptr++ = lgate[4] | (lgate[0] << 16); /* Vl,Vs */
                                 *dptr++ = (((lgate[1] + lgate[5]) >> 1) << 16) & 0xffff0000; /* W,V=0 */
                                 *dptr++ = ((lgate[2] + lgate[6]) >> 1) | (((lgate[3] + lgate[7]) >> 1) << 16); /* NCP,DBZ */

                        /* Write testpulse velocity and power to vme to vme handshake area */        
                                 vme2_pntr->tpulse_vel = lgate[0];
                                 vme2_pntr->tpulse_level_proc = lgate[2];

                             }
                        if(num_param == 12)     /* 10 parameter header */
                            {
                                j = 0;
                                while(i-- != 0)
                                    {
                                        if(j != 1)
                                            {
                                                *dptr++ = *data_loc++;
                                            }
                                        else
                                            {
                                                data_loc++;
                                            }
                                        j++;
                                        if(j == 6)
                                          j = 0;
                                    }
                                data_loc += lgate_off;
                                temp = *data_loc++;
                                lgate[0] = temp & 0xff;         /* Ws */
                                lgate[1] = (temp >> 16) & 0xff; /* Vs */
                                temp = *data_loc++;
                                lgate[2] = temp & 0xff;         /* NCPs */
                                lgate[3] = (temp >> 16) & 0xff; /* DBZs */
                                data_loc += lgate_off2;
                                temp = *data_loc++;
                                lgate[4] = temp & 0xff;         /* Wl */
                                lgate[5] = (temp >> 16) & 0xff; /* Vl */
                                temp = *data_loc++;
                                lgate[6] = temp & 0xff;         /* NCPl */
                                lgate[7] = (temp >> 16) & 0xff; /* DBZl */
                                *dptr++ = lgate[0] || (lgate[1] << 16); /* Ws,Vs */
                                *dptr++ = lgate[4] || (lgate[5] << 16); /* Wl,Vl */
                                *dptr++ = lgate[6] || (lgate[7] << 16); /* NCPl,DBZl */
                                *dptr++ = (lgate[0] + lgate[4]) >> 2;   /* W,V=0 */
                                *dptr++ = ((lgate[2] + lgate[6]) >> 2) || (((lgate[3] + lgate[7]) >> 2) << 16); /* NCP, DBZ */

                            }
                        if((indf_ts == 1)||(indf_ts == 3))
                            {
			      dptr++;       /* skip over indep freq header info */
			      dptr++;
			      data_loc = (unsigned long *)(data_loc_base + indf_off);
			      i = 2*f_flag * (prt_flag + 1);
			      while(i-- != 0)
				{
				    *dptr++ = *data_loc++;
				}
			  }
			      
			if(indf_ts == 3)
			  {
			      dptr++;       /* skip over time series header info */
			      dptr++;
			      i = ts_sampl;
			      while(i-- != 0)
				{
				    *dptr++ = *data_loc++; /* copy floats over as ints */  
				}
			  }
			
			/* Do Raw Data Processing if Req'd */

			ts_data_loc = (float *)(data_loc_base + ts_off + (TS_freq - 1) * (prt_flag + 1) * sampl * 2 * sizeof(float));
			switch(ASCOPE_mode)
			  {
			  case 0: /* Time Series */
			    i = sampl;
			    if(!TS_off)
			      {
				raw_d ->rd.x_label[0] = 'S';
				raw_d ->rd.x_label[1] = 'A';
				raw_d ->rd.x_label[2] = 'M';
				raw_d ->rd.x_label[3] = 'P';
				raw_d ->rd.x_label[4] = 'L';
				raw_d ->rd.x_label[5] = 'E';
				raw_d ->rd.x_label[6] = ' ';
				raw_d ->rd.x_label[7] = ' ';

				raw_d ->rd.y_label[0] = 'I';
				raw_d ->rd.y_label[1] = 'N';
				raw_d ->rd.y_label[2] = 'P';
				raw_d ->rd.y_label[3] = 'H';
				raw_d ->rd.y_label[4] = 'A';
				raw_d ->rd.y_label[5] = 'S';
				raw_d ->rd.y_label[6] = 'E';
				raw_d ->rd.y_label[7] = ' ';
				raw_d ->rd.xmax = (float)(sampl-1);
				raw_d ->rd.xmin = 0.0;
  				raw_d ->rd.ymax = 65535*(float)(wvfm->chip_width[0]/7.5); 
				raw_d ->rd.ymin = -65536*(float)(wvfm->chip_width[0]/7.5);
				raw_d ->rd.numPoints = sampl;
				while(i-- != 0)
				  {

				    *raw_ptr++ = *ts_data_loc++ * SCALE_fac;  /* Read I */
				    ts_data_loc++; /* skip over Q */

				  }
			      }
			    else
			      {
				raw_d ->rd.x_label[0] = 'S';
				raw_d ->rd.x_label[1] = 'A';
				raw_d ->rd.x_label[2] = 'M';
				raw_d ->rd.x_label[3] = 'P';
				raw_d ->rd.x_label[4] = 'L';
				raw_d ->rd.x_label[5] = 'E';
				raw_d ->rd.x_label[6] = ' ';
				raw_d ->rd.x_label[7] = ' ';

				raw_d ->rd.y_label[0] = 'Q';
				raw_d ->rd.y_label[1] = 'U';
				raw_d ->rd.y_label[2] = 'A';
				raw_d ->rd.y_label[3] = 'D';
				raw_d ->rd.y_label[4] = 'R';
				raw_d ->rd.y_label[5] = 'A';
				raw_d ->rd.y_label[6] = 'T';
				raw_d ->rd.y_label[7] = '.';
				raw_d ->rd.xmax = (float)(sampl-1);
				raw_d ->rd.xmin = 0.0;
				raw_d ->rd.ymax = 65535*(float)(wvfm->chip_width[0]/7.5); 
				raw_d ->rd.ymin = -65536*(float)(wvfm->chip_width[0]/7.5); 
				raw_d ->rd.numPoints = sampl;
				while(i-- != 0)
				  {
				    ts_data_loc++;     /* point to Q */
				    *raw_ptr++ = *ts_data_loc++ * SCALE_fac;  /* Read Q */
				  }
			      }

			    break;
			  case 1: /* Velocity */
			    data_loc = (unsigned long *)(data_loc_base);
			    i = tnum_int;
			    if(prt_flag)
			      prm = GetParameter(inHeader,3); /* velocity */
			    else
			      prm = GetParameter(inHeader,1); /* velocity */
			    t_scale = 1.0/prm->parameter_scale;
				raw_d ->rd.x_label[0] = 'G';
				raw_d ->rd.x_label[1] = 'A';
				raw_d ->rd.x_label[2] = 'T';
				raw_d ->rd.x_label[3] = 'E';
				raw_d ->rd.x_label[4] = ' ';
				raw_d ->rd.x_label[5] = ' ';
				raw_d ->rd.x_label[6] = ' ';
				raw_d ->rd.x_label[7] = ' ';

				raw_d ->rd.y_label[0] = 'V';
				raw_d ->rd.y_label[1] = 'E';
				raw_d ->rd.y_label[2] = 'L';
				raw_d ->rd.y_label[3] = 'O';
				raw_d ->rd.y_label[4] = 'C';
				raw_d ->rd.y_label[5] = 'I';
				raw_d ->rd.y_label[6] = 'T';
				raw_d ->rd.y_label[7] = 'Y';
				raw_d ->rd.xmax = (float)(tnum_int-1);
				raw_d ->rd.xmin = 0.0;
				raw_d ->rd.ymax = prm->parameter_bias * t_scale;
				raw_d ->rd.ymin = -prm->parameter_bias * t_scale;
				raw_d ->rd.numPoints = tnum_int;

			    while(i-- != 0)
			      {
				if(prt_flag) /* staggered PRT */
				  {
				    data_loc++;
				    *raw_ptr++ = ((*data_loc++ & 0xffff) - prm->parameter_bias) * t_scale;
				    data_loc++;
				  }
				else  /* simple PRT */
				  {
				    *raw_ptr++ = ((*data_loc++ & 0xffff) - prm->parameter_bias) * t_scale;
				    data_loc++;
				  }
			      }
			      
			    break;
			  case 2: /* Power (Actually dbZ for now) */
			    data_loc = (unsigned long *)(data_loc_base);
			    i = tnum_int;
			    if(prt_flag)
			      prm = GetParameter(inHeader,5); /* dbZ */
			    else
			      prm = GetParameter(inHeader,3); /* dbZ */
			    t_scale = 1.0/prm->parameter_scale;

				raw_d ->rd.x_label[0] = 'G';
				raw_d ->rd.x_label[1] = 'A';
				raw_d ->rd.x_label[2] = 'T';
				raw_d ->rd.x_label[3] = 'E';
				raw_d ->rd.x_label[4] = ' ';
				raw_d ->rd.x_label[5] = ' ';
				raw_d ->rd.x_label[6] = ' ';
				raw_d ->rd.x_label[7] = ' ';

				raw_d ->rd.y_label[0] = 'P';
				raw_d ->rd.y_label[1] = 'O';
				raw_d ->rd.y_label[2] = 'W';
				raw_d ->rd.y_label[3] = 'E';
				raw_d ->rd.y_label[4] = 'R';
				raw_d ->rd.y_label[5] = ' ';
				raw_d ->rd.y_label[6] = ' ';
				raw_d ->rd.y_label[7] = ' ';
				raw_d ->rd.xmax = (float)(tnum_int-1);
				raw_d ->rd.xmin = 0.0;
				raw_d ->rd.ymax = 70.0;
				raw_d ->rd.ymin = -30.0;
				raw_d ->rd.numPoints = tnum_int;
				
			    while(i-- != 0)
			      {
				if(prt_flag) /* staggered PRT */
				  {
				    data_loc++;
				    data_loc++;
				    *raw_ptr++ = ((*data_loc++ & 0xffff) - prm->parameter_bias) * t_scale;

				  }
				else  /* simple PRT */
				  {
				    data_loc++;
				    *raw_ptr++ = ((*data_loc++ & 0xffff) - prm->parameter_bias) * t_scale;
				  }
			      }
			    break;
			  case 3: /* Power Spectrum */
			    i = 2 * sampl;
			    raw_d ->rd.x_label[0] = 'B';
			    raw_d ->rd.x_label[1] = 'I';
			    raw_d ->rd.x_label[2] = 'N';
			    raw_d ->rd.x_label[3] = ' ';
			    raw_d ->rd.x_label[4] = ' ';
			    raw_d ->rd.x_label[5] = ' ';
			    raw_d ->rd.x_label[6] = ' ';
			    raw_d ->rd.x_label[7] = ' ';

			    raw_d ->rd.y_label[0] = 'F';
			    raw_d ->rd.y_label[1] = 'F';
			    raw_d ->rd.y_label[2] = 'T';
			    raw_d ->rd.y_label[3] = '*';
			    raw_d ->rd.y_label[4] = '*';
			    raw_d ->rd.y_label[5] = '2';
			    raw_d ->rd.y_label[6] = ' ';
			    raw_d ->rd.y_label[7] = ' ';
			    raw_d ->rd.xmax = (float)(sampl-1);
			    raw_d ->rd.xmin = 0.0;
			    raw_d ->rd.ymax = 10.0;
			    raw_d ->rd.ymin = -90.0;
			    raw_d ->rd.numPoints = FFT_pts;
			    for(i=0; i<2*sampl; i+=2)
			      {
				/* Swap I and Q for ELDORA */

				b_array[i+1] = *ts_data_loc++;
				b_array[i] = *ts_data_loc++; 
			      }
			    status = getseries(array,2*FFT_pts,b_array,2*sampl);
			    if(status)
			      {
				hann(array,FFT_pts);
				fft(array,FFT_pts);
				for(i=0; i<2*FFT_pts; i++)
				  {
				    I = i * 2;
				    Q = I + 1;
				    if(array[I] == 0.0 && array[Q] == 0.0)
				      {
					array[i] = 0.0;
					continue;
				      }
				    array[i] = 10.0 * log10(array[I]*array[I] + array[Q]*array[Q]) + wp;				
#ifdef AVG
				    array[i] = 0.9*av_array[i] + 0.1*array[i];
				    av_array[i] = array[i];
#endif /* AVG */
				  }
#ifdef SWAP
				for(i=0; i<sampl; i++)
				  {
				    if(i <= sampl/2-1)
				      *raw_ptr++ = array[i + sampl/2];
				    else
				      *raw_ptr++ = array[i - sampl/2];
				  }
#endif /* SWAP */
#ifndef SWAP
				for(i=0; i<FFT_pts; i++)
				  {
				    *raw_ptr++ = array[i];
				  }
#endif /* SWAP */
			      }
			    else    /* if new fft not ready use previous fft */
			      {
				for(i=0; i<FFT_pts; i++)
				  {
				    *raw_ptr++ = av_array[i];
				  }
			      }
			    break;
			  default:
			    break;
			  }

			vme2_pntr -> radar_hndshk[k] = 0;
			k = k<26?k+1:0;
			if(k == 0)
			  q++;
			      
			if (buff_cnt == 0)
			  {
			      buff_cnt = 1;
			      data_loc = (unsigned long *)(COL0BASE + DB_2);
			  }
			else
			  {
			      buff_cnt = 0;
			      data_loc = (unsigned long *)(COL0BASE + DB_1);
			  }
			

			/* check for valid NAV data */
			      
			if(vme2_pntr -> nav_hndshk[m] == 1)
			  {
			      curr_nav_add = (long)(VMEMEM_BASE + STD_BASE + DATA_NAV_BASE + (m * DATA_NAV_OFFSET));
			      curr_nav_mailbox_add = (long)(&vme2_pntr -> nav_hndshk[m]); 
			      send_nav = 1;
			      m = m<1?m+1:0;
			  }
			      
			      /* check for valid ADS data */
			      
			else if(vme2_pntr -> ads_hndshk[p] == 1)
			  { 
			      curr_ads_add = (long)(VMEMEM_BASE + STD_BASE + DATA_ADS_BASE + (p * DATA_ADS_OFFSET));
			      curr_ads_mailbox_add = (long)(&vme2_pntr -> ads_hndshk[p]); 
			      send_ads = 1;
			      p = p<1?p+1:0;
			  }
			if(fill_ts_array)
			  {
			      if(!fill_busy)
				{
				    if(dc_removal)
				      fta_freq=(int)ad_freq;
				    else
				      fta_freq=ts_freq;
				    switch(fta_freq)
				      {
					case 1:
					  offset = 0;
					  break;
					case 2:
					  offset = 8;
					  break;
					case 3:
					  offset = 16;
					  break;
				      }
			      
				    fill_busy = 1;	
				}		      
			      if (ts_cnt == 0)
				{
				    ts_cnt = 1;
				    ts_loc = (float *)(COL0BASE + DB_1 + ts_off + offset);
				}
			      else
				{
				    ts_cnt = 0;
				    ts_loc = (float *)(COL0BASE + DB_2 + ts_off + offset);
				}
				    for(i=0; i<sampl; i++)
				      {
					  if(n < pts)
					    {
						a_dummy = *ts_loc++;  /* read in i from collator */
						*a_pntr++ = a_dummy;  
						a_dummy = *ts_loc;    /* read in q from collator */
						
						*a_pntr++ = a_dummy;
						ts_loc += incr;       /* point to next i,q pair in */
					  /* collator DPRAM */
						n++;
					    }
					  else
					    {
						i = sampl;
						n = 0;
						fill_ts_array = 0;
						fill_busy = 0;
						semGive(array_full_sem);
					    }
				      }

			  }
              }
            else
                {
/*			printf("ERROR: NO END OF BEAM INTERRUPT \n"); */
			int_cnt++;
			      /* Update Status */
			currStatus -> rp7 |= END_OF_BEAM;
			proc_stat |= EOB;
                }
	   
        }

       /* Make sure that semaphore is empty after last Stop Command */

	    sem_status = semTake(bim_int0_sem,NO_WAIT);
	    if (sem_status == OK)
	      *bim_cr0 = 0xdc;  /* re-enable interrupt INT0* */            
	    printf("Real Time Task Stopped \n");
  }
      else
	printf("PANIC: BAD REAL TIME SEMAPHORE \n");
}
free(array);
}

































