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
#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"


extern CELLSPACING *cs;
extern RADARDESC *rdsc;
extern WAVEFORM *wvfm;
extern PARAMETER *prm;
extern FIELDRADAR *fldrdr;
extern task_sync;

coll_data_xfer()
{


long           k, l, m, p, q, r, s;

register long  i;

int            *samps, data_samps, buff_cnt, prf, gates,sem_status, sampl,
               prt_flag, f1_flag, f2_flag, ts_sampl, n, fta_freq, tp_pwr_off,
               f3_flag, f4_flag, f5_flag, f_flag, ts_off, ts_cnt, incr, offset, lgate_off,
               indf_off, num_int1, num_int2, num_int3, num_int4, tnum_int;

volatile short indf_ts, ray_hndshk_stat;

unsigned long  data_loc_base;

volatile register unsigned long *dptr;
volatile register unsigned long   *data_loc;

float          a_dummy, *tp_pwr_loc;

volatile register float *ts_loc;
for(;;)
  {
      sem_status = semTake(real_sem,WAIT_FOREVER); /* wait for start semaphore  */
      if(sem_status == OK)
	{
/* Clear out Stop Flag */

	    stop = 0;
	    task_sync = 0;

/* Parse Header for required system parameters */

	    wvfm = GetWaveform(inHeader);
	    gates = wvfm -> num_gates[0];
	    f1_flag = wvfm -> num_chips[0];
	    f2_flag = wvfm -> num_chips[1];
	    f3_flag = wvfm -> num_chips[2];
	    f4_flag = wvfm -> num_chips[3];
	    f5_flag = wvfm -> num_chips[4];
	    f_flag = f1_flag + f2_flag + f3_flag + f4_flag + f5_flag;
	    prt_flag = 0;
	    sampl = wvfm -> repeat_seq_dwel * wvfm -> num_chips[0];
	    incr = ((f_flag * 8) - 4)/4;
	    if(!radar_fore_aft)
	      cs = GetCellSpacing(inHeader,1);
	    else
	      cs = GetCellSpacing(inHeader,2);
            num_int1 = cs -> num_cells[0];
            num_int2 = cs -> num_cells[1];
            num_int3 = cs -> num_cells[2];
            num_int4 = cs -> num_cells[3];
	    tnum_int = num_int1 + num_int2 + num_int3 + num_int4;

/* Substitute code to process indf_ts here !!! */

	    if(!radar_fore_aft)
	      fldrdr = GetFieldRadar(inHeader,1);
	    else
	      fldrdr = GetFieldRadar(inHeader,2);
	    indf_ts = fldrdr -> indepf_times_flg;

      /* General Initialization */

	    data_loc = (unsigned long *)(COL0BASE + DB_1);
	    ts_off = (tnum_int * 4 * 2) + (2 * f_flag * 4 * 2);
	    tp_pwr_off = ts_off + (2 * f_flag * 4 * sampl);
	    data_samps = (tnum_int - 1) * 2;
	    buff_cnt = 0;
	    ts_cnt = 0;
	    fill_busy = 0;
	    n = 0;
	    k = 0;
	    l = 0;
	    m = 0;
	    p = 0;
	    r = 0;
	    s = 0;
	    q = 0;
	    switch(indf_ts)
	      {
		case 0:
		  data_samps = (tnum_int - 1) * 2;
		  break;
		case 1:
		  indf_off = 2 * 4 * (tnum_int + f_flag);
		  break;
		case 3:
		  indf_off = 2 * 4 * (tnum_int + f_flag);
		  ts_sampl = 2 * sampl * f_flag;
		  break;
	      }
	    while(!(stop || reboot))
	      {
	    /* Wait 5 ticks for End of Beam Interrupt */
		  
		  sem_status = semTake(bim_int0_sem,5); /* wait 5 ticks for ISR to pass sem */
		  
		  if (sem_status == OK)
		    {
			*bim_cr0 = 0xdc; /* re-enable interrupt INT0* */            
			switch(vme2_pntr -> tpulse_freq_num)
			  {
			    case 1:
			      lgate_off = 0x2;
			      break;
			    case 2:
			      lgate_off = 0x4;
			      tp_pwr_off += 0x4;
			      break;
			    case 3:
			      lgate_off = 0x6;
			      tp_pwr_off += 0x8;
			      break;
			    default:
			      lgate_off = 0x2;
			      break;
			  }
			data_loc_base = (unsigned long)data_loc;
			/* Check Handshake Status of Housekeeper */			
			ray_hndshk_stat = vme2_pntr -> radar_hndshk[k];

			if((unsigned short)ray_hndshk_stat != 0xAFFF)
			  {
			      printf("m %4X \n",ray_hndshk_stat);
			      /* send error status to control processor */
			  }

			ray = (struct RAY*)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (k * DATA_RAY_OFFSET));
			ray -> fielddata.ray_count = k + (q * 27);
			dptr = &(ray->data[0]);
			i = data_samps;

			/* Pull data from Collator's Dual Port Ram */

			while(i-- != 0)
			  {
			      *dptr++ = *data_loc++;
			  }
			data_loc += lgate_off;
			*dptr++ = *data_loc++;
			*dptr++ = *data_loc++;

			if((indf_ts == 1)||(indf_ts == 3))
			  {
			      dptr++;       /* skip over indep freq header info */
			      dptr++;
			      data_loc = (unsigned long *)(data_loc_base + indf_off);
			      i = 2*f_flag;
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
			vme2_pntr -> radar_hndshk[k] = 0;
			k = k<26?k+1:0;
			if(k == 0)
			  q++;
			      
	      /* Write testpulse power into vme to vme handshake area */
			
			tp_pwr_loc = (float *)(data_loc_base + tp_pwr_off);
			vme2_pntr -> tpulse_level_proc = *tp_pwr_loc;
			
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
			      else
				{
				    if(vme2_pntr -> radar_hndshk[l] == 1)
				      {
					  for(i=0;i<3;i++)
					    {
						if(vme2_pntr -> radar_hndshk[l] == 1)
						  {
						      curr_ray_add[r][s] = (long)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (l * DATA_RAY_OFFSET));
						      curr_mailbox_add[r][s] = (long)(&vme2_pntr -> radar_hndshk[l]); 
						      send_ray[r][s] = 1;
						      r = r<2?r+1:0;
						      l = l<26?l+1:0;
						  }
						else 
						  i = 3; /* exit loop if any test fails */
					    }
					  s = s<1?s+1:0;
				      }
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
}

































