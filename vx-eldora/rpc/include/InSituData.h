/*
 *	$Id$
 *
 *	Module:		 InsituData.h
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
#ifndef INCInsituDatah
#define INCInsituDatah

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct insitu_data {
    char  insitu_data_id[4];	/* Identifier = ISIT. */
    long  insitu_data_len;		/* Block size in bytes. */
    short julian_day;
    short hours;
    short minutes;
    short seconds;
};

#ifdef OK_RPC

typedef struct insitu_data insitu_data;
typedef struct insitu_data INSITU_DATA;

extern bool_t xdr_insitu_data(XDR *, insitu_data *);

#endif /* OK_RPC */
#endif /* INCInsituDatah */

