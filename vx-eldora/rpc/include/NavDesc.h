/*
 *	$Id$
 *
 *	Module:		 NavDesc.h
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
#ifndef INCNavDesch
#define INCNavDesch

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct nav_descript {
    char  nav_descript_id[4];	/* Identifier = NDDS. */
    long  nav_descript_len;	/* Block size in bytes. */
    short ins_flag;		/* 0 = no INS data, 1 = data recorded. */
    short gps_flag;		/* 0 = no GPS data, 1 = data recorded. */
    short minirims_flag;	/* 0 = no MiniRIMS data, 1 = data recorded. */
    short kalman_flag;		/* 0 = no kalman data, 1 = data recorded. */
};

#ifdef OK_RPC

typedef struct nav_descript nav_descript;
typedef struct nav_descript NAVDESC;

extern bool_t xdr_nav_descript(XDR *, nav_descript *);

#endif /* OK_RPC */
#endif /* INCNavDesch */

