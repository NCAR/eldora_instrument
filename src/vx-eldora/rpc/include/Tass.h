/*
 *	$Id$
 *
 *	Module:		 Tass.h
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 *
 *
 * description: Time series data format for the tass tests
 *        
 */
#ifndef INCTassh
#define INCTassh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct tass_data {
    char  tass_data[4]; 	/* tass data identifier */
				/* (ascii characters TASS) */
    long tass_data_len;	        /* Length of the tass time series */
				/* data block in bytes */
    long  ray_count;		/* Data Ray counter For this */
				/* particular type of data ray */
    short num_samples;          /* Number of samples taken during
                                   the dwell time */
    short waveform_type;        /* Identifier for the type of
                                   waveform being used */
    short first_rec_gate;	/* First recorded gate number (N) */
    short last_rec_gate;	/* Last recorded gate number (M) */
};				/* End of Structure */

#ifdef OK_RPC 

typedef struct tass_data tass_data;
typedef struct tass_data TASSDATA;

bool_t xdr_tass_data(XDR *, tass_data *);

#endif /* OK_RPC */

#endif /* INCTassh */
