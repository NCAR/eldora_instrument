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
 * Revision 1.1  1994/01/06  21:31:49  craig
 * Initial revision
 *
 *
 * description: This module calculates the length in bytes
 *              of a radar data ray.
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>
int offset_to_last;
int longs_to_copy;
int bytes_per_cell;
int bytes_at_end;
int cells_per_ray;

long data_ray_length()
{

FIELDRADAR *fldrdr;
PARAMETER *prm;

/* define some general purpose variables */

int i;
long indep_freq_length, time_series_length;
long number_of_cells, total_length;


/****************************************************************/
/* First job is to calculate the number of bytes in a data cell */
/* Assume both fore and aft radars are identical, so only needs */
/* To be done for one of the radars                             */
/****************************************************************/

rad_dscr = GetRadar(Hdr,1);

bytes_per_cell = 0;
for(i=0; i < rad_dscr->num_parameter_des; i++)
    {
    prm = GetParameter(Hdr,i);
    bytes_per_cell += prm -> binary_format;
    }

/* Calculate the number of cells */

number_of_cells = 0;
for (i=0; i<cs->num_segments; i++)
  number_of_cells += cs -> num_cells[i];

/* Calculate the size of the independent frequency data */

fldrdr = GetFieldRadar(Hdr,1);
 
if(fldrdr->indepf_times_flg > 0)
  indep_freq_length = rad_dscr->num_freq_trans * bytes_per_cell *
                       rad_dscr->num_ipps_trans + 8;
else
   indep_freq_length = 0; /* Until switch to new header */

/* Calculate the size of the time series data */

if(fldrdr->indepf_times_flg == 3)
  time_series_length = rad_dscr->num_freq_trans * 2 * 4 * 
    wave->repeat_seq_dwel * wave->num_chips[0] + 8;
else
   time_series_length = 0; /* Until switch to new header */

total_length = sizeof(ray_i) + sizeof(platform_i) + 
                 sizeof(field_parameter_data) + number_of_cells *
                 bytes_per_cell + indep_freq_length + time_series_length;

offset_to_last=sizeof(ray_i)+sizeof(platform_i)+sizeof(field_parameter_data)+(number_of_cells-1)*bytes_per_cell;
longs_to_copy=(bytes_per_cell+indep_freq_length+time_series_length+4)/4;
bytes_at_end=bytes_per_cell+indep_freq_length+time_series_length;
cells_per_ray=number_of_cells;
min_to_record =  sizeof(ray_i) + sizeof(platform_i) + 
                   sizeof(field_parameter_data) + 2 * bytes_per_cell +
		   indep_freq_length + time_series_length;
return(total_length);
}



