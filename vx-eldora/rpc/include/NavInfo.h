/*
 *	$Id$
 *
 *	Module:		 NavInfo.h
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
#ifndef INCNavInfoh
#define INCNavInfoh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct nav_info {
    char  nav_info_id[4];	/* Identifier = NAVD. */
    long  nav_info_len;		/* Block size in bytes. */
    short julian_day;		/* Julian day. */
    short hours;
    short minutes;
    short seconds;
};

#ifdef OK_RPC

typedef struct nav_info nav_info;
typedef struct nav_info NAV_INFO;

extern bool_t xdr_nav_info(XDR *, nav_info *);

#endif /* OK_RPC */
#endif /* INCNavInfoh */

