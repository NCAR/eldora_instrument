/*
 *	$Id$
 *
 *	Module:		 FieldParam.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCFieldParamh
#define INCFieldParamh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct field_parameter_data {
    char  field_param_data[4];	/* Field parameter data identifier */
				/* (ascii characters FRAD) */
    short field_param_data_len; /* Length of the field parameter */
				/* data block in bytes */
    short data_sys_status;	/* Status word, bits will be assigned
                                /*  particular status when needed */
    char  radar_name[8];	/* Name of radar from which this data ray  */
				/* came from */
    float test_pulse_level;	/* Test pulse power level as measured by the
                                /*  power meter in dbm */
    float test_pulse_dist;	/* Distance from antenna to middle of */
				/* test pulse in km */
    float test_pulse_width;	/* Test pulse width in m  */
    float test_pulse_freq;	/* Test pulse frequency in Ghz */
    short test_pulse_atten;	/* Test pulse attenuation in db */
    short test_pulse_fnum;	/* Frequency number being calibrated */
				/* with the test pulse (what mux on */
				/* timing module is set to) */
    float noise_power;		/* Total estimated noise power in dbm */
    long  ray_count;		/* Data Ray counter For this */
				/* particular type of data ray */
    short first_rec_gate;	/* First recorded gate number (N) */
    short last_rec_gate;	/* Last recorded gate number (M) */
};				/* End of Structure */

#ifdef OK_RPC 

typedef struct field_parameter_data field_parameter_data;
typedef struct field_parameter_data FIELDPARAMDATA;

bool_t xdr_field_parameter_data(XDR *, field_parameter_data *);

#endif /* OK_RPC */

#endif /* INCFieldParamh */
