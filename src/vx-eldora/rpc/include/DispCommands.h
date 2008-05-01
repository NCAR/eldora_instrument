/*
 *      $Id$
 *
 *      Module:          DispCommands.h
 *      Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.2  1996/03/21  17:17:41  thor
 * Added support for new Raw display.
 *
 * Revision 2.1  1993/07/29  18:57:30  thor
 * Changed RESTART to RESTART_DISP due to conflict with VxWorks.
 *
 * Revision 2.0  1992/11/02  20:44:10  thor
 * First offical ELDORA release!
 *
 * Revision 1.6  1992/09/21  14:56:38  thor
 * Added LOAD_ONLY command.
 *
 * Revision 1.5  1992/02/10  17:00:08  thor
 * Added Dual Display codes.
 *
 * Revision 1.4  1992/01/08  14:53:33  thor
 * Added new command for timeout change.
 *
 * Revision 1.3  1991/12/17  16:01:07  thor
 * Added constants for sides.
 *
 * Revision 1.2  1991/09/18  15:30:50  thor
 * Added RESTART command & inlines.
 *
 * Revision 1.1  1991/09/16  17:38:50  thor
 * Initial revision
 *
 *
 *
 * description:
 *        These are the display specific commands. They specify which
 * display format is to be used.
 *
 */
#ifndef INCDispCommandsh
#define INCDispCommandsh

#include "GblCommands.h"

#define FORWARD_RADIAL 0x10
#define FORWARD_HORIZ  0x20
#define FORWARD_VERT   0x40
#define AFT_RADIAL     0x80
#define AFT_HORIZ      0x100
#define AFT_VERT       0x200
#define RESTART_DISP   0x400
#define TMO_CHANGE     0x800
#define FORWARD_DUAL   0x1000
#define AFT_DUAL       0x2000
#define FORWARD_RAW    0x4000
#define AFT_RAW        0x8000
#define LOAD_ONLY      0x80000

#define LEFT_SIDE      0
#define RIGHT_SIDE     1

#ifndef __GNUC__
#define INLINE
#else
#define INLINE inline
#endif /* INLINE */

/* If this doesn't break anything I'll remove it. */
/* static INLINE int TypeToNum(FAST int t) */
/* { */
/*     switch(t) { */
/*       case FORWARD_RADIAL: return(0); */
/*       case FORWARD_HORIZ:  return(1); */
/*       case FORWARD_VERT:   return(2); */
/*       case AFT_RADIAL: return(3); */
/*       case AFT_HORIZ:  return(4); */
/*       case AFT_VERT:   return(5); */
/*       default: return(-1); */
/*     } */
/* } */

/* static INLINE int NumToType(FAST int n) */
/* { */
/*     switch(n) { */
/*       case 0: return(FORWARD_RADIAL); */
/*       case 1:  return(FORWARD_HORIZ); */
/*       case 2:   return(FORWARD_VERT); */
/*       case 3: return(AFT_RADIAL); */
/*       case 4:  return(AFT_HORIZ); */
/*       case 5:   return(AFT_VERT); */
/*       default: return(-1); */
/*     } */
/* } */

#undef INLINE

#endif /* INCDispCommandsh */

