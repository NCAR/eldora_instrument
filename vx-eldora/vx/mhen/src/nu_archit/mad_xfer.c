/*
 *	$Id$
 *
 *	Module:	MAD_XFER	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1995/01/25  17:40:54  eric
 * Initialized global Header pointers from nu_archit_dac.c to eliminate
 * conflicts.
 *
 * Revision 1.3  1995/01/20  14:54:32  eric
 * Fixed bug that caused Time Series name field to be
 * overwritten.
 *
 * Revision 1.2  1994/11/14  20:09:05  craig
 * Expand to possible 3000 rays in a MAD area
 *
 * Revision 1.1  1994/07/14  20:33:03  eric
 * Initial revision
 *
 * Revision 1.2  1993/09/30  15:27:09  eric
 * modified calculation for number of areas to use logical length
 * rather than fixed size of 0x8000 bytes.
 *
 * Revision 1.1  1992/11/09  16:07:33  eric
 * Initial revision
 *
 *
 * description: This module initializes the Multi-Crate Pipeline (MCPL)
 *              for the ELDORA RP7 RADAR Processor.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE
#define F_FACTOR 0x40
#define NAV_LENGTH 0x1154

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

#include "RadarGbls.h"
#include "HeaderRpc.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "RDPGlobals.h"
#include "raw_data.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "LclCommands.h"
#include "ELDRP7.h"

#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

extern float rpm;

/* make the following variables available throughout this
   module only !                                        
*/

mad_xfer()
{

long           i, j, data_length, data_len, bytes_per_cell, num_cells,
               indep_freq_len, time_series_len, ts_len;
/* The following parameters need to be defined for the mcpl code to work */

/* Define needed tape structures */

struct radar_d this_radar;
struct cell_spacing_d cellspace;
struct parameter_d param[8];

/* Define some general purpose variables */
short proc, timeout, status, mad;

/***************************************************************************/
/* Setup Generic parts of TPB's to send a MAD structures to the processors */
/***************************************************************************/

/* Parse Header for required TPB info */

cellspace.num_segments = cs -> num_segments;
for (i=0; i<cellspace.num_segments; i++)
  cellspace.num_cells[i] = cs -> num_cells[i];
this_radar.num_parameter_des = rdsc -> num_parameter_des;
for(i=0;i<this_radar.num_parameter_des;i++)
  {
      prm = GetParameter(inHeader,i);
      param[i].binary_format = prm -> binary_format;
  }
/* Calculate data length, independent frequency length, and time_series length in bytes */

num_cells = 0;
for (j=0; j < cs -> num_segments; j++)
  num_cells += cs -> num_cells[j];
printf("num_cells = %d \n", num_cells);
bytes_per_cell = 0;
for (j=0; j < rdsc -> num_parameter_des; j++)
  {
      prm = GetParameter(inHeader,j);
      bytes_per_cell += prm -> binary_format;
  }
printf("bytes_per_cell = %d \n",bytes_per_cell);
data_len = num_cells * bytes_per_cell;
printf("data_len = %d \n",data_len);

if(fldrdr -> indepf_times_flg > 0)
  indep_freq_len = rdsc -> num_freq_trans * 8 * rdsc -> num_ipps_trans + sizeof(INDEP_FREQ);
else
  indep_freq_len = 0;
printf("indep_freq_len = %d \n",indep_freq_len);

if(fldrdr -> indepf_times_flg == 3)
  time_series_len = rdsc -> num_freq_trans * 2 * 4 * wvfm -> repeat_seq_dwel * wvfm -> num_chips[0] + sizeof(TIME_SERIES);
else
  time_series_len = 0;
printf("time_series_len = %d \n",time_series_len);

for(j=0;j<27;j++)
  {
/* Setup necessary info for 'tape header' */
      /* Ray Info Block */

      ray = (struct DATA_RAY *)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (j * DATA_RAY_OFFSET));
      ray->rayinfo.ray_info[0] = 'R';
      ray->rayinfo.ray_info[1] = 'Y';
      ray->rayinfo.ray_info[2] = 'I';
      ray->rayinfo.ray_info[3] = 'B';
      ray->rayinfo.ray_info_length = sizeof(ray_i);
      ray->rayinfo.azimuth = 0.0;
      ray->rayinfo.elevation = 0.0;
      ray->rayinfo.true_scan_rate = rpm * 6.0;  

      /* Platform Info Block */
      
      ray->pltfrminfo.platform_info[0] = 'A';
      ray->pltfrminfo.platform_info[1] = 'S';
      ray->pltfrminfo.platform_info[2] = 'I';
      ray->pltfrminfo.platform_info[3] = 'B';
      ray->pltfrminfo.platform_info_length = sizeof(platform_i);
      ray->pltfrminfo.rotation_angle = 0.0;
      
      /* Field Parameter Data Block */

      ray->fielddata.field_param_data[0] = 'F';
      ray->fielddata.field_param_data[1] = 'R';
      ray->fielddata.field_param_data[2] = 'A';
      ray->fielddata.field_param_data[3] = 'D';
      ray->fielddata.field_param_data_len = 0;
      for (i=0; i<cellspace.num_segments; i++)
	ray->fielddata.field_param_data_len += cellspace.num_cells[i];
      ray->fielddata.field_param_data_len *= this_radar.num_parameter_des * 2;
      ray->fielddata.field_param_data_len += sizeof(field_parameter_data);
      if(radar_fore_aft)
	{
	    ray->fielddata.radar_name[0] = 'A';
	    ray->fielddata.radar_name[1] = 'F';
	    ray->fielddata.radar_name[2] = 'T';
	    ray->fielddata.radar_name[3] = ' ';
	}
      else
	{
	    ray->fielddata.radar_name[0] = 'F';
	    ray->fielddata.radar_name[1] = 'O';
	    ray->fielddata.radar_name[2] = 'R';
	    ray->fielddata.radar_name[3] = 'E';
	}

      ray->fielddata.ray_count = 0;

      /* Independent Frequency Block */
      
      i_f = (INDEP_FREQ *)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (j * DATA_RAY_OFFSET) + data_len + sizeof(ray_i) + sizeof(platform_i) + sizeof(field_parameter_data));
/*      printf("i_f = %X \n", i_f); */
      i_f -> indep_freq_id[0] = 'I';
      i_f -> indep_freq_id[1] = 'N';
      i_f -> indep_freq_id[2] = 'D';
      i_f -> indep_freq_id[3] = 'F';
      i_f -> indep_freq_len = indep_freq_len;

      /* Time Series Block */

      t_s = (TIME_SERIES *)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (j * DATA_RAY_OFFSET) + data_len + indep_freq_len + sizeof(ray_i) + sizeof(platform_i) + sizeof(field_parameter_data));
/*      printf("t_s = %X \n", t_s); */
      t_s -> time_series_id[0] = 'T';
      t_s -> time_series_id[1] = 'I';
      t_s -> time_series_id[2] = 'M';
      t_s -> time_series_id[3] = 'E';
      t_s -> time_series_len = time_series_len;

      /* Raw Data Block */

      if((fldrdr -> indepf_times_flg == 3))
	ts_len = time_series_len;
      else
	ts_len = sizeof(TIME_SERIES);
      raw_d = (struct RAW_D*)(VMEMEM_BASE + STD_BASE + DATA_RAY_BASE + (j * DATA_RAY_OFFSET) + data_len + indep_freq_len + sizeof(ray_i) + sizeof(platform_i) + sizeof(field_parameter_data) + ts_len);

      raw_d -> rd.raw_des[0] = 'R';
      raw_d -> rd.raw_des[1] = 'A';
      raw_d -> rd.raw_des[2] = 'W';
      raw_d -> rd.raw_des[3] = 'D';
      raw_d -> rd.numPoints = prm->num_samples; /* Samples as default */
  }


/* The tpb_to_proc TPB sends the receive_gmad_tpb to a processor's dispatch
   table entry one.  All of the general purpose parmeters in the TPB will be
   set now and processor specific ones will be set later */

tpb_to_proc->source_start_address = (long)receive_gmad - STD_BASE;
                                               /* Data to be retrieved from
                                                  receive_gmad structure */
tpb_to_proc->sink_start_address = 0;           /* Going to an mcpl node so
                                                 no address is necessary */
tpb_to_proc->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                          modifier $09, disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
tpb_to_proc->sink_attribute = 0x0001;     /* Going to a MCPL Node */
tpb_to_proc->transfer_length = 7;         /* Eight Long words in a TPB */
tpb_to_proc->completion_status = 0x0006; /* Disable interrupt Mailbox A32/D16*/
tpb_to_proc->nack_count = 0;              /* Do not retry */
tpb_to_proc->link_address = (long)gmad_to_proc - STD_BASE;
                                     /* link to gmad_to_proc TPB structure */
tpb_to_proc->link_attribute = 0x0963;/* Normal VMEBus Addressing,VME link after
                                        trigger, address modifier $09, disable
                                        BLT, 256 Xfers/burst, disable
                                        pipelining, link to VME */
tpb_to_proc->mcpl_control_word = 0x0045;  /* Generic MCPL Node address,
                                            dispatch table entry #1, remain
                                            connected, dispatch table load,
                                            burst mode, compelled */
tpb_to_proc->mailbox_address = (long)mailbox - STD_BASE;

/* The gmad_to_proc TPB sends a MAD to an MCPL node as determined by 
    its dispatch table entry one.  All of the general purpose parmeters in this
    TPB will be set now and processor specific ones will be set later */

gmad_to_proc->source_start_address = (long)gmad - STD_BASE;
                               /* Data to be retrieved from gmad structure */
gmad_to_proc->sink_start_address = 0;      /* Going to an mcpl node so
                                           no address is necessary */
gmad_to_proc->source_attribute = 0x0963; /* Normal VMEBus Addressing, address
                                           modifier $09, disable pipelining,
                                           disable BLT,256 transfers per burst,
                                           32 bit transfers,VMEbus */
gmad_to_proc->sink_attribute = 0x0001;     /* Going to a MCPL Node */
gmad_to_proc->transfer_length = (sizeof(struct MAD) + 4)/4;
                                         /* Size of MAD in Long words */
gmad_to_proc->completion_status = 0x0006;/* Disable interrupt Mailbox A32/D16*/
gmad_to_proc->nack_count = 0;             /* Do not retry */
gmad_to_proc->link_address = (long)tpb_to_proc - STD_BASE;
                            /* link back to tpb_to_proc structure */ 
gmad_to_proc->link_attribute = 0x0963; /* Normal VMEBus Addressing,VME link
                                         after trigger, address modifier $09,
                                         disable BLT, 256 Xfers/burst,
                                         disable pipelining,link to VME */
gmad_to_proc->mcpl_control_word = 0x004B; /* Generic MCPL Node address,
                                            dispatch table entry #1,DTX
                                            Dispatch Table execute,locked,
                                            noncompelled, remain connected */
gmad_to_proc->mailbox_address = (long)mailbox - STD_BASE;

/* The receive_gmad tpb loads data off of the MCPL and into VMEBus memory
   this TPB is loaded in dispatch table entry one, completion of transfer is
   not signaled */

receive_gmad->source_start_address = 0; /* Data to be retrieved from mcpl */
receive_gmad->sink_start_address = 0x0000000; /* Generic Start Address of the
                                                MAD Memory */
receive_gmad->source_attribute = 0x0001;     /* Comming From an MCPL Node */
receive_gmad->sink_attribute = 0x0963;  /* Normal VMEBus Addressing, address
                                          modifier $09,disable pipelining,
                                          disable BLT, 256 transfers per burst,
                                          32 bit transfers,VMEbus */
receive_gmad->transfer_length = (sizeof(struct MAD) + 4)/4;
                                       /*  Long words in a MAD structure */
receive_gmad->completion_status = 0;    /*  No completion Status */
receive_gmad->nack_count = 0;           /* Do not retry */
receive_gmad->link_address = 0;        /* Linking is not performed by target */
receive_gmad->link_attribute = 0;      /* Linking is not performed by target */
receive_gmad->mcpl_control_word = 0;    /* MCPL to VMEBus transfer only */
receive_gmad->mailbox_address = 0;      /* No mail Box Being used */

/***********************************************************************/
/* Calculate the position and size of the data areas in all processors */
/***********************************************************************/


number_procs = 0;
for (proc=0; proc<4; proc++)
  {
    if(number_of_mads[proc] != 0) number_procs++;
    for (mad=0; mad<number_of_mads[proc]; mad++)
      {
/*    madinfo[proc][mad][SIZE_AREAS] = 0x8000; */
      logical_length = data_ray_length();
      if(logical_length > NAV_LENGTH)
	data_length = logical_length + F_FACTOR;
      else
	data_length = NAV_LENGTH + F_FACTOR;

      madinfo[proc][mad][SIZE_AREAS] = data_length;

      madinfo[proc][mad][NUM_AREAS] = 
	(madinfo[proc][mad][MAX_MEM] - STARTOF_MAD_DATA) /
	  madinfo[proc][mad][SIZE_AREAS];
          if(madinfo[proc][mad][NUM_AREAS] % 2 != 0)
            madinfo[proc][mad][NUM_AREAS] -= 1;
     if(madinfo[proc][mad][NUM_AREAS] > 3000)
	madinfo[proc][mad][NUM_AREAS] = 3000;
      printf("\nlogical length = %d\n",logical_length);
      printf("area size = %ld\n",madinfo[proc][mad][SIZE_AREAS]);
      printf("max mem = %ld\n",madinfo[proc][mad][MAX_MEM]);
      printf("number of areas = %ld\n",madinfo[proc][mad][NUM_AREAS]);
      printf("MAD structure start address = %lx\n",gmad);

/* fill the general MAD structure with the proper information */

      gmad->sync_word = 0x55555555;
      gmad->num_before_repeat = madinfo[proc][mad][NUM_AREAS];
      for(i=0; i<madinfo[proc][mad][NUM_AREAS]; i++)
	{
         gmad->mailboxes[i] = 0;
	 gmad->addresses[i] = madinfo[proc][mad][MAD_BASE] + STARTOF_MAD_DATA +
                             i * madinfo[proc][mad][SIZE_AREAS];
        }

/* Fill Generic holes in TPB's to send this MAD structure to the
   proper processor */
     tpb_to_proc->mcpl_control_word = (tpb_to_proc->mcpl_control_word & 0x07FF)
                                       | (proc_nodes[proc]<<11);
     gmad_to_proc->mcpl_control_word= (gmad_to_proc->mcpl_control_word & 0x07FF)
                                      | (proc_nodes[proc]<<11);
     receive_gmad->sink_start_address = madinfo[proc][mad][MAD_BASE];

/* Send the  receieve_gmad tpb to the proper processor */

/* Only the FORE radar processor will send out the MADS */
    if( !radar_fore_aft)
     {

      printf("triggering the mcpl node to transfer TPB to processor: %d\n"
              ,proc);

      timeout = 0;
      *mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  taskDelay(1);
	  timeout++;
	} while ((*mailbox == 0) && (timeout < 200));

      status = *status_reg;
      if(*mailbox == 0x5FFF)
	printf("TPB transfer to processor %d successful status is: %x\n",
                proc,status);
      else
	{
	    printf("#1 TPB transfer unsuccessful proc: %d mailbox: %x status: %x\n",proc,*mailbox,status);

	    currStatus->mcpl |= MAD_XFER;
	    switch(proc)
	      {
		case 0:
		  currStatus->mcpl |= DP1;
		  break;
		case 1:
		  currStatus->mcpl |= DP2;
		  break;
		case 2:
		  currStatus->mcpl |= RP;
		  break;
		case 3:
		  currStatus->mcpl |= GP;
		  break;
	      }
	    currStatus->mailbox = *mailbox;
	    currStatus->status = status;
	     
	    return(2); 
	}

/* Transfer MAD structure to remote node */

      printf("\nTransferring MAD: %d to processor: %d\n",mad,proc);

      timeout = 0;
      *mailbox = 0;
      *control_reg = TRIGGER;
      *start_reg = 0x0001;
      do
	{
	  timeout++;
	  taskDelay(1);
	} while ((*mailbox != 0x5FFF) && (timeout < 200)); 

      status = *status_reg;
      if(timeout < 200)
      printf("Successful Xfer of MAD: %d to proc: %d status is: %x\n",
              mad,proc,status);
      else
	{
	printf("Unsuccessful xfer MAD: %d, proc: %d mailbox: %x status: %x\n",
                mad,proc,*mailbox,status);
        return(5); 
        }
      } /* test for being the aft processor */


    } /* For loop for each MAD */
   } /* For loop for each processor */
return(-1);
}



































