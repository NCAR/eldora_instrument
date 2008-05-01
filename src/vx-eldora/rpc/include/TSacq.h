/*
 *	$Id$
 *
 *	Module:		 TSacq.h
 *	Original Author: E. Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 *
 *
 * description: Time series control parameters
 *        
 */
#ifndef INCTSacqh
#define INCTSacqh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct ts_acq {

    int st_gate; 	 /* starting gate number (0-1023) */
			
    int num_gates;       /* number of time series gates to record (1-512) */

    float rt_az_limit;   /* rightmost azimuth bound on time series recording */

    float lt_az_limit;   /* leftmost azimuth bound on time series recording */

    float upper_el_limit;  /* upper elevation bound on time series recording */

    float lower_el_limit;  /* lower elevation bound on time series recording */

};				/* End of Structure */

#ifdef OK_RPC 

typedef struct ts_acq ts_acq;
typedef struct ts_acq TSACQ;

bool_t xdr_ts_acq(XDR *, ts_acq *);

#endif /* OK_RPC */

#endif /* INCTSacqh */
