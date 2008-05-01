/*
 

 Replacement file (init_info_blocks.c) for mad_xfer.c for hskp to initialize 
 hskp data info block for Ray, Platform, and Field Parameter Data



 * $Id: mad_xfer.c,v 1.6 2005/08/09 19:44:49 granger Exp $
 *
 * Module: MAD_XFER  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research 
 * Date:   $Date: 2005/08/09 19:44:49 $
 *
 * revision history
 * ----------------
 * $Log: mad_xfer.c,v $
 * Revision 1.6  2005/08/09 19:44:49  granger
 * commit versions as copied from /net/eldora/eldora
 *
 * Revision 1.5  1996/06/28  21:33:31  eric
 * Added raw data field for ascope diagnostics.
 *
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

static char rcsid[] = "$Date: 2005/08/09 19:44:49 $ $RCSfile: mad_xfer.c,v $ $Revision: 1.6 $";

#define OK_RPC
#define scope extern
#include "hskpAll.h"
#include "RadarGbls.h"

//#define NO_RP7_SCOPE
//#define NO_RDP_SCOPE
//#define F_FACTOR 0x40
//#define NAV_LENGTH 0x1154

//#include "vxWorks.h"
//#include "math.h"
//#include "stdioLib.h"
//#include "intLib.h"
//#include "memLib.h"
//#include "semLib.h"
//#include "taskLib.h"
//#include "tyLib.h"
//#include "ioLib.h"
//#include "in.h"
//#include "systime.h"
//#include "sysLib.h"


//#include "HeaderRpc.h"
//#include "Parameter.h"
//#include "RadarDesc.h"
//#include "Header.h"
//#include "CellSpacing.h"
//#include "Ray.h"
//#include "Platform.h"
//#include "FieldParam.h"
//#include "Waveform.h"
//#include "IndFreq.h"
//#include "TimeSeries.h"
//#include "RDPGlobals.h"
//#include "raw_data.h"
//#include "mcpl_def.h"
//#include "mcpl_gbl.h"
//#include "LclCommands.h"
//#include "ELDRP7.h"

//#include "coll_dpramdefs.h"
//#include "colquadsubs.h"
//#include "dspdefs.h"
//#include "dspaddr.h"
//#include "rp7.h"


extern float rpm;
CELLSPACING *cs;
RADARDESC *rdsc;

/* make the following variables available throughout this
   module only !                                        
*/

void init_info_blocks()
{

long           i, j, data_length, data_len, bytes_per_cell, num_cells,
               indep_freq_len, time_series_len, ts_len;
/* The following parameters need to be defined for the mcpl code to work */

/* Define needed tape structures */

struct radar_d this_radar_fore;
struct radar_d this_radar_aft;
struct cell_spacing_d cellspace_fore;
struct cell_spacing_d cellspace_aft;

//struct parameter_d param[8];

/* Define some general purpose variables */
short proc, timeout, status, mad;

/***************************************************************************/
/* Setup Generic parts of TPB's to send a MAD structures to the processors */
/***************************************************************************/

//Parse FORE Header

rdsc = GetRadar(inHeader, 1);
cs = GetCellSpacing(inHeader, 1);

cellspace_fore.num_segments = cs -> num_segments;
for (i=0; i<cellspace_fore.num_segments; i++)
  cellspace_fore.num_cells[i] = cs -> num_cells[i];
this_radar_fore.num_parameter_des = rdsc -> num_parameter_des;

//Parse AFT Header

rdsc = GetRadar(inHeader, 2);
cs = GetCellSpacing(inHeader, 2);

cellspace_aft.num_segments = cs -> num_segments;
for (i=0; i<cellspace_aft.num_segments; i++)
  cellspace_aft.num_cells[i] = cs -> num_cells[i];
this_radar_aft.num_parameter_des = rdsc -> num_parameter_des;


/*
for(i=0;i<this_radar.num_parameter_des;i++)
  {
      prm = GetParameter(inHeader,i);
      param[i].binary_format = prm -> binary_format;
  }
*/


/* Calculate data length in bytes 

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
*/


for(j=0;j<27;j++)
  {
      /* Setup necessary info for 'tape header' */

      /* Ray Info Block */

      fore_ray_pntr = (struct DATARAY *)(STANDARD_BASE + FORE_STAND_START + FIRST_RADAR_OFFSET + (j * RADAR_SIZE_INCR));
      aft_ray_pntr = (struct DATARAY *)(STANDARD_BASE + AFT_STAND_START + FIRST_RADAR_OFFSET + (j * RADAR_SIZE_INCR));

      fore_ray_pntr->this_rayi.ray_info[0] = 'R';
      fore_ray_pntr->this_rayi.ray_info[1] = 'Y';
      fore_ray_pntr->this_rayi.ray_info[2] = 'I';
      fore_ray_pntr->this_rayi.ray_info[3] = 'B';
      fore_ray_pntr->this_rayi.ray_info_length = sizeof(ray_i);
      fore_ray_pntr->this_rayi.azimuth = 0.0;
      fore_ray_pntr->this_rayi.elevation = 0.0;
      fore_ray_pntr->this_rayi.true_scan_rate = rpm * 6.0;  

      aft_ray_pntr->this_rayi.ray_info[0] = 'R';
      aft_ray_pntr->this_rayi.ray_info[1] = 'Y';
      aft_ray_pntr->this_rayi.ray_info[2] = 'I';
      aft_ray_pntr->this_rayi.ray_info[3] = 'B';
      aft_ray_pntr->this_rayi.ray_info_length = sizeof(ray_i);
      aft_ray_pntr->this_rayi.azimuth = 0.0;
      aft_ray_pntr->this_rayi.elevation = 0.0;
      aft_ray_pntr->this_rayi.true_scan_rate = rpm * 6.0;  

      /* Platform Info Block */
      
      fore_ray_pntr->this_plat.platform_info[0] = 'A';
      fore_ray_pntr->this_plat.platform_info[1] = 'S';
      fore_ray_pntr->this_plat.platform_info[2] = 'I';
      fore_ray_pntr->this_plat.platform_info[3] = 'B';
      fore_ray_pntr->this_plat.platform_info_length = sizeof(platform_i);
      fore_ray_pntr->this_plat.rotation_angle = 0.0;

      aft_ray_pntr->this_plat.platform_info[0] = 'A';
      aft_ray_pntr->this_plat.platform_info[1] = 'S';
      aft_ray_pntr->this_plat.platform_info[2] = 'I';
      aft_ray_pntr->this_plat.platform_info[3] = 'B';
      aft_ray_pntr->this_plat.platform_info_length = sizeof(platform_i);
      aft_ray_pntr->this_plat.rotation_angle = 0.0;
      
      /* Field Parameter Data Block */

      fore_ray_pntr->this_fdata.field_param_data[0] = 'F';
      fore_ray_pntr->this_fdata.field_param_data[1] = 'R';
      fore_ray_pntr->this_fdata.field_param_data[2] = 'A';
      fore_ray_pntr->this_fdata.field_param_data[3] = 'D';
      fore_ray_pntr->this_fdata.field_param_data_len = 0;
      for (i=0; i<cellspace_fore.num_segments; i++)
         fore_ray_pntr->this_fdata.field_param_data_len += cellspace_fore.num_cells[i];
      fore_ray_pntr->this_fdata.field_param_data_len *= this_radar_fore.num_parameter_des * 2;
      fore_ray_pntr->this_fdata.field_param_data_len += sizeof(field_parameter_data);
      
      fore_ray_pntr->this_fdata.radar_name[0] = 'F';
      fore_ray_pntr->this_fdata.radar_name[1] = 'O';
      fore_ray_pntr->this_fdata.radar_name[2] = 'R';
      fore_ray_pntr->this_fdata.radar_name[3] = 'E';     

      fore_ray_pntr->this_fdata.ray_count = 0;

      aft_ray_pntr->this_fdata.field_param_data[0] = 'F';
      aft_ray_pntr->this_fdata.field_param_data[1] = 'R';
      aft_ray_pntr->this_fdata.field_param_data[2] = 'A';
      aft_ray_pntr->this_fdata.field_param_data[3] = 'D';
      aft_ray_pntr->this_fdata.field_param_data_len = 0;
      for (i=0; i<cellspace_aft.num_segments; i++)
         aft_ray_pntr->this_fdata.field_param_data_len += cellspace_aft.num_cells[i];
      aft_ray_pntr->this_fdata.field_param_data_len *= this_radar_aft.num_parameter_des * 2;
      aft_ray_pntr->this_fdata.field_param_data_len += sizeof(field_parameter_data);

      aft_ray_pntr->this_fdata.radar_name[0] = 'A';
      aft_ray_pntr->this_fdata.radar_name[1] = 'F';
      aft_ray_pntr->this_fdata.radar_name[2] = 'T';
      aft_ray_pntr->this_fdata.radar_name[3] = ' ';

      aft_ray_pntr->this_fdata.ray_count = 0;

  }

return(-1);
}



































