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

#include "Parameter.h"
#include "RadarDesc.h"
#include "FieldRadar.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"


long data_ray_length()
{

extern CELLSPACING *cs;
extern RADARDESC *rdsc;
extern FIELDRADAR *fldrdr;
extern PARAMETER *prm;
extern HeaderPtr *inHeader;

/* define some general purpose variables */

int i;
long indep_freq_length, time_series_length;
long bytes_per_cell, number_of_cells, total_length;

indep_freq_length = 0;
time_series_length = 0;

/****************************************************************/
/* First job is to calculate the number of bytes in a data cell */
/* Assume both fore and aft radars are identical, so only needs */
/* To be done for one of the radars                             */
/****************************************************************/

rdsc = GetRadar(inHeader,1);

bytes_per_cell = 0;
for(i=0; i < rdsc->num_parameter_des; i++)
    {
    prm = GetParameter(inHeader,i);
    bytes_per_cell += prm -> binary_format;
    }
/* Calculate the number of cells */
cs = GetCellSpacing(inHeader,1);

number_of_cells = 0;
for (i=0; i<cs->num_segments; i++)
  number_of_cells += cs -> num_cells[i];
printf("number_of_cells = %d \n",number_of_cells);
/* Calculate the size of the independent frequency data */

fldrdr = GetFieldRadar(inHeader,1);

if(fldrdr->indepf_times_flg > 0)
  indep_freq_length = rdsc->num_freq_trans * bytes_per_cell * rdsc->num_ipps_trans + 8;
else
   indep_freq_length = 0; 

/* Calculate the size of the time series data */

if(fldrdr->indepf_times_flg == 3)
  time_series_length = rdsc->num_freq_trans * 2 * 4 * prm->num_samples * rdsc->num_ipps_trans + 8;
else
   time_series_length = 0;

total_length = sizeof(ray_i) + sizeof(platform_i) + 
                 sizeof(field_parameter_data) + number_of_cells *
                 bytes_per_cell + indep_freq_length + time_series_length;

return(total_length);
}
