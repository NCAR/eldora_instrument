/*
 *	$Id$
 *
 *	Module:		 Parameter.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/08/30  18:39:32  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCParameterh
#define INCParameterh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct parameter_d {
    char  parameter_des[4];	/* Parameter Descriptor identifier */
				/* (ascii characters "PARM"). */
    long parameter_des_length;	/* Parameter Descriptor length in */
				/* bytes.*/
    char  parameter_name[8];	/* Name of parameter being described. */
    char  param_description[40]; /* Detailed description of this parameter. */
    char  param_units[8];	/* Units parameter is written in. */
    float interpulse_time;	/* Effective inter-pulse period of */
				/* this parameter in us. */
    float xmitted_freq;		/* Average transmitted frequency of */
				/* this  parameter in GHz. */
    float recvr_bandwidth;	/* Effective receiver bandwidth for */
				/* this parameter in MHz.*/
    float unambiguous_velocity; /* Unambiguous velocity after */
				/* unfolding in m/s. */
    float unambiguous_rng;	/* Unambiguous range after unfolding in km.  */
    short pulse_width;		/* Effective pulse width of parameter */
				/* in m. */
    short polarization;		/* Polarization of the radar beam for */
				/* this parameter (0 Horizontal,1 */
				/* Vertical,2 Circular,3 Elliptical) in na. */
    short num_samples;		/* Number of samples used in estimate */
				/* for this parameter. */
    short binary_format;	/* Binary format of radar data. */
    char  threshold_field[8];	/* Name of parameter upon which this */
				/* parameter is thresholded (ascii */
				/* characters NONE if not */
				/* thresholded). */
    float threshold_value;	/* Value of threshold in ? */
    float parameter_scale;	/* Scale factor for parameter. */
    float parameter_bias;	/* Bias factor for parameter. */
    long  bad_data;		/* Bad data flag (Length is same as */
				/* that of the data). */
}; /* End of Structure */


#ifdef OK_RPC

typedef struct parameter_d parameter_d;
typedef struct parameter_d PARAMETER;

bool_t xdr_parameter_d(XDR *, PARAMETER *);

#endif /* OK_RPC */

#endif /* INCParameterh */

