/*
 *	$Id$
 *
 *	Module:		 TimeSeries.h
 *	Original Author: Richard E. Neitzel
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
#ifndef INCTimeSeriesh
#define INCTimeSeriesh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct time_series {
    char  time_series_id[4];	/* Identifier = TIME. */
    long  time_series_len;	/* Block size in bytes. */
};

#ifdef OK_RPC

typedef struct time_series time_series;
typedef struct time_series TIME_SERIES;

extern bool_t xdr_time_series(XDR *, time_series *);

#endif /* OK_RPC */
#endif /* INCTimeSeriesh */

