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
 * Revision 1.3  1995/03/31  22:51:25  craig
 * Don't Know
 *
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

/* Function to Initialize the lookup tables for data reduction */

#define TILT 18    /* just to get in the ballpark */

void reduce_init()
{
int j, i, dist_to_end[7], cells_to_end[7];
int total_dist, num_cells, total_cells;
float test, beam_dist, afloat;

/* Calculate the lookup table */ 

total_cells = 0;
total_dist = cs->distToFirst;
dist_to_end[0] = 0;
cells_to_end[0] = 0;
for(i=1; i<=cs->num_segments; i++)
  {
      cells_to_end[i] = total_cells + cs->num_cells[i-1];
      dist_to_end[i] = total_dist + cs->num_cells[i-1] * cs->spacing[i-1];
      total_cells = cells_to_end[i];
      total_dist = dist_to_end[i];
  }

for(i=0; i<361; i++)
  {
      afloat = i;
      beam_dist = total_dist;
      if( i >= 0 && i < 86) /*record all gatess closest 8 beams to horizontal*/
	beam_dist = 1000.0 * rad_dscr->data_red_parm0 / 
	  cos((double)(afloat * DEG_TO_RAD));
      else if (i > 94 && i <=  180)
	beam_dist = 1000.0 * (-1.0) * rad_dscr->data_red_parm1 /
	  cos((double)((180.0 - afloat) * DEG_TO_RAD));
      else if (i > 180 && i < 266)
	beam_dist = 1000.0 * (-1.0) * rad_dscr->data_red_parm1 /
	  cos((double)((afloat - 180.0) * DEG_TO_RAD));
      else if( i > 274 && i <= 360)
	beam_dist = 1000.0 * rad_dscr->data_red_parm0 / 
	  cos((double)((360.0 - afloat) * DEG_TO_RAD));

      num_cells = 1;
      for(j=1; j<=cs->num_segments; j++)
	{
	    if(beam_dist>dist_to_end[j-1] &&
	       beam_dist < dist_to_end[j]) break;
	    num_cells += cells_to_end[j];
	}

      num_cells = num_cells + (beam_dist - dist_to_end[j-1])/
	cs->spacing[j-1];

      if(num_cells > total_cells) num_cells = total_cells;

      reduce_gate[i] = num_cells;

      if(i < 90 || i > 270)
	alt_fact[i] = -800. / (float)cs->spacing[j-1] * 
	  sqrt(1 + tan(i * DEG_TO_RAD) *tan(i * DEG_TO_RAD) +
	       tan(TILT * DEG_TO_RAD) * tan(TILT * DEG_TO_RAD));
      else if(i > 90 && i < 270)
	alt_fact[i] = 1200. / (float)cs->spacing[j-1] *
	  sqrt(1 + tan(i * DEG_TO_RAD) *tan(i * DEG_TO_RAD) +
	       tan(TILT * DEG_TO_RAD) * tan(TILT * DEG_TO_RAD));

      if(num_cells == total_cells) alt_fact[i] = 0;

  }

}

/**************************************************/
/* function that actually does the data reduction */
/**************************************************/

int reduce_data(short type,unsigned int *rec_addr)
{

struct data_beam *db;
int keep;
unsigned int *copy_from_address, i;
unsigned int *copy_to_address;
int total_cells, angle;

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

      /* If aircraft is above maximum, or below minimum, the below
	 algorithm blows up, just return in these cases
         and record the entire data ray */

      if(db->pi.altitude_msl > rad_dscr->data_red_parm0 ||
         db->pi.altitude_msl < rad_dscr->data_red_parm1) return(keep);

      angle = db->pi.rotation_angle + db->pi.roll;

      if(angle < 0)
	{
	    angle += 360;
	    if(angle < 0) return(keep);
	}
      if(angle > 360)
	{
	    angle -= 360;
	    if(angle > 360) return(keep);
	}

      total_cells = reduce_gate[angle] + 
	db->pi.altitude_msl * alt_fact[angle];

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

    default:  /* if data reduction is not on, leave keep equal to 1 and first
                 recorded gate to zero, last recorded gate to max */
      break;
  }

return(keep);

}
