/*
 *	$Id$
 *
 *	Module:	data_ray_length	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1996/06/21  18:48:55  thor
 * Fixed raw data length calc.
 *
 * Revision 1.3  1995/01/25  17:42:15  eric
 * Initialized global Header pointers from nu_archit_dac.c to avoid
 * conflicts.
 *
 * Revision 1.2  1994/11/14  17:27:23  eric
 * Added include files to keep compatible with new
 * Control Processor code.
 *
 * Revision 1.1  1994/07/14  20:37:28  eric
 * Initial revision
 *
 * Revision 1.1  1992/11/09  16:07:59  eric
 * Initial revision
 *
 *
 * description: This module calculates the length in bytes
 *              of a radar data ray.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"

#include "HeaderRpc.h"
#include "RadarGbls.h"
#include "RDPGlobals.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "raw_data.h"

#define MAX_FFT_SIZE 1024
long data_ray_length()
{

/* define some general purpose variables */

int i;
long indep_freq_length, time_series_length, raw_data_length;
long bytes_per_cell, number_of_cells, total_length;

indep_freq_length = 0;
time_series_length = 0;
raw_data_length = 0;

/****************************************************************/
/* First job is to calculate the number of bytes in a data cell */
/* Assume both fore and aft radars are identical, so only needs */
/* To be done for one of the radars                             */
/****************************************************************/


bytes_per_cell = 0;
for(i=0; i < rdsc->num_parameter_des; i++)
    {
    prm = GetParameter(inHeader,i);
    bytes_per_cell += prm -> binary_format;
    }
/* Calculate the number of cells */

number_of_cells = 0;
for (i=0; i<cs->num_segments; i++)
  number_of_cells += cs -> num_cells[i];
printf("number_of_cells = %d \n",number_of_cells);
/* Calculate the size of the independent frequency data */


if(fldrdr->indepf_times_flg > 0)
  indep_freq_length = rdsc->num_freq_trans * bytes_per_cell * rdsc->num_ipps_trans + 8;
else
   indep_freq_length = 0; 

/* Calculate the size of the time series data */

if(fldrdr->indepf_times_flg == 3)
  time_series_length = rdsc->num_freq_trans * 2 * 4 * prm->num_samples * rdsc->num_ipps_trans + 8;
else
   time_series_length = 0;

/* Calculate the raw data length */

if(number_of_cells > prm->num_samples && number_of_cells > MAX_FFT_SIZE)
  raw_data_length = sizeof(RAW_DATA) + number_of_cells * sizeof(float);
else if(prm->num_samples > number_of_cells && prm->num_samples > MAX_FFT_SIZE)
  raw_data_length = sizeof(RAW_DATA) + prm->num_samples * sizeof(float);
else
  raw_data_length = sizeof(RAW_DATA) + MAX_FFT_SIZE * sizeof(float);
total_length = sizeof(ray_i) + sizeof(platform_i) + 
  sizeof(field_parameter_data) + number_of_cells *
  bytes_per_cell + indep_freq_length + time_series_length + raw_data_length;

return(total_length);
}
