/*
 *      $Id$
 *
 *      Module:          DispStatus.h
 *      Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.0  1992/11/02  20:44:10  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1991/10/30  19:57:24  thor
 * Added error code.
 *
 * Revision 1.1  1991/09/16  17:39:00  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This is the only status used to indicate things are ok.
 *
 */
#ifndef INCDispStatush
#define INCDispStatush

#include "GblStatus.h"

#define DRAWING 0x4

#define DDP_RCV_ERR 0x1000
#define DDP_RESET_ERR 0x2000

#endif /* INCDispStatush */

