/*
 *	$Id$
 *
 *	Module: reduce.c		 
 *	Original Author:Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1994/11/14  17:51:52  craig
 * Fixed problem that calculated length of FRAD block wrong
 *
 * Revision 1.1  1994/01/06  21:31:53  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/21  22:31:17  reif
 * Initial revision
 *
 * 
 * description: The purpose of reduce.c is to perform the various data
 *              reduction algorithms and return a 1 if we are to keep
 *              the record and 0 if it is to be discarded.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

extern int offset_to_last;
extern int longs_to_copy;
extern int bytes_per_cell;
extern int bytes_at_end;
extern int cells_per_ray;
 
int reduce_data(short type,unsigned int *rec_addr)
{

struct data_beam *db;
int keep;
unsigned int *copy_from_address;
unsigned int *copy_to_address;
int i;
int current_dist;
int total_cells;
int actual_alt;
int beam_dist;
int diff;

/********* Perform Selected Reduction **********/

(unsigned int)db=*rec_addr;

/* Default should be to keep entire record, set first recorded gate to
   zero and last record gate to maximum */

keep=1; /* Return this if we want to keep entire record */
db->fpd.first_rec_gate = 0;
db->fpd.last_rec_gate = cells_per_ray - 1;
db->fpd.field_param_data_len = cells_per_ray * bytes_per_cell +
                               sizeof(field_parameter_data);

switch(type)
  {
    case 1: /* Between two angles - PIE SLICE */
      if((db->pi.rotation_angle+db->pi.roll)>=rad_dscr->data_red_parm0 &&\
	 (db->pi.rotation_angle+db->pi.roll)<=rad_dscr->data_red_parm1)
	keep=1; /* This is a keeper */
      else
	keep=0; /* Get rid of this record */
      break;

    case 2: /* Between concentric circles */
      printf("NOT IMPLEMENTED YET!!\n");
      break;

    case 3:   /* Above and below certain altitudes */

      /* If aircraft is above maximum, or below minimum, or elevation angle
         is zero.  The below algorithm blows up, just return in these cases
         and record the entire data ray */

      if(db->pi.altitude_msl > rad_dscr->data_red_parm0 ||
         db->pi.altitude_msl < rad_dscr->data_red_parm1 ||
         db->ri.elevation == 0.0) return(keep);

      if(db->ri.elevation>0)
	{
	    /* Along beam distance to the maximum altitude line (meters) */
	    beam_dist=1000.0 *(rad_dscr->data_red_parm0 - db->pi.altitude_msl)/
	      sin((double)(db->ri.elevation * DEG_TO_RAD));
	}
      else
	{
	    /* Along beam distance to minimum altitude line (meters) */
	    beam_dist=1000.0 *(rad_dscr->data_red_parm1 - db->pi.altitude_msl)/
	      sin((double)(db->ri.elevation * DEG_TO_RAD));
	}

      current_dist=cs->distToFirst; /* Initialize with distance to
				       first cell */
      total_cells=0;
      for(i=0; i<cs->num_segments; i++)
	{
	    current_dist+=(cs->num_cells[i]*cs->spacing[i]);
	    if(current_dist>=beam_dist)
	      {
		  current_dist-=cs->num_cells[i]*cs->spacing[i];
		  diff=beam_dist-current_dist;
		  total_cells+=diff/cs->spacing[i]+1;

		  if(total_cells >= cells_per_ray)
		    return(keep);

		  (unsigned int)copy_to_address=(unsigned int)db+
		    sizeof(struct data_beam)+(total_cells*bytes_per_cell);
		  (unsigned int)copy_from_address=(unsigned int)db+
		    offset_to_last;
		  for(i=0; i<longs_to_copy; i++)
		    {
			*copy_to_address++ = *copy_from_address++;
		    }

		  db->fpd.last_rec_gate=total_cells;
		  keep=sizeof(struct data_beam)+total_cells*bytes_per_cell
		    + bytes_at_end;
		  db->fpd.field_param_data_len = (total_cells + 1) *
		    bytes_per_cell + sizeof(field_parameter_data);
		  break;
	      }
	    total_cells+=cs->num_cells[i];
	}
      break;

    default:  /* if data reduction is not on, leave keep equal to 1 and first
                 recorded gate to zero, last recorded gate to max */
      break;
  }
return(keep);
}
