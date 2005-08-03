/*
 *	$Id$
 *
 *	Module:		 Tass_hskp.h
 *	Original Author: E. Loew
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 *
 *
 * description: Time series housekeeping
 *        
 */
#ifndef INCTasshksph
#define INCTasshskph

#ifdef OK_RPC

#ifdef UNIX
#include <rpc/rpc.h>
#else
#include "rpc/rpc.h"
#endif /* UNIX */

#include "Tass.h"
#include "Ray.h"

#endif /* OK_RPC */

struct tass_hskp {

    struct ray_i       ray;
    struct tass_data   tass;

};				/* End of Structure */

#ifdef OK_RPC 

typedef struct tass_hskp tass_hskp;
typedef struct tass_hskp TASSHSKP;

bool_t xdr_tass_hskp(XDR *, tass_hskp *);

#endif /* OK_RPC */

#endif /* INCTasshskph */
