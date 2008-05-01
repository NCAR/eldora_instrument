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
 * Revision 1.1  2003/03/14 15:31:01  vanandel
 * Initial revision
 *
 * Revision 1.2  1991/10/15  17:54:55  thor
 * Fixed to meet latest version of tape spec.
 *
 * Revision 1.2  1991/10/15  17:54:55  thor
 * Fixed to meet latest version of tape spec.
 *
 * Revision 1.1  1991/08/30  18:39:21  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCFieldParamh
#define INCFieldParamh

struct field_parameter_data {
    char  field_param_data[4];	/* Field parameter data identifier */
				/* (ascii characters FRAD) */
    long field_param_data_len;	/* Length of the field parameter */
				/* data block in bytes */
    long data_sys_status;	/* Status word, bits will be assigned */
                                /*  particular status when needed */
    char  radar_name[8];	/* Name of radar from which this data ray  */
				/* came from */
    float test_pulse_level;	/* Test pulse power level as measured by the */
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

typedef struct field_parameter_data field_parameter_data;
typedef struct field_parameter_data FIELDPARAMDATA;

#endif /* INCFieldParamh */
