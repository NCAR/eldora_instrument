/*
 *	$Id$
 *
 *	Module:		 InSitu.h
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
#ifndef INCInSituh
#define INCInSituh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct insitu_parameter {
    char  name[8];
    char  units[8];
};

struct insitu_descript {
    char  insitu_descript_id[4]; /* Identifier = SITU. */
    long  insitu_descript_len;	/* Block size in bytes. */
    long  number_params;	/* Number of paramters. */
    struct insitu_parameter params[256]; /* Is this enough? */
};

#ifdef OK_RPC

typedef struct insitu_descript insitu_descript;
typedef struct insitu_descript INSITUDESC;

extern bool_t xdr_insitu_descript(XDR *, insitu_descript *);

typedef struct insitu_parameter insitu_parameter;
typedef struct insitu_parameter INSITU_PARAMETER;

extern bool_t xdr_insitu_parameter(XDR *, insitu_parameter *);

#endif /* OK_RPC */
#endif /* INCInSituh */

