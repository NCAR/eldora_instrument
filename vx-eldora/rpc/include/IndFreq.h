/*
 *	$Id$
 *
 *	Module:		 IndFreq.h
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
#ifndef INCIndFreqh
#define INCIndFreqh

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#endif /* OK_RPC */

struct indep_freq {
    char  indep_freq_id[4];	/* Identifier = INDF. */
    long  indep_freq_len;	/* Block size in bytes. */
};

#ifdef OK_RPC

typedef struct indep_freq indep_freq;
typedef struct indep_freq INDEP_FREQ;

extern bool_t xdr_indep_freq(XDR *, indep_freq *);

#endif /* OK_RPC */
#endif /* INCIndFreqh */

