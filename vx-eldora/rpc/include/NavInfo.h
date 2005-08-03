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
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCNavInfoh
#define INCNavInfoh

struct nav_info {
    char  nav_info_id[4];	/* Identifier = NAVD. */
    long  nav_info_len;		/* Block size in bytes. */
    short julian_day;		/* Julian day. */
    short hours;
    short minutes;
    short seconds;
};

typedef struct nav_info nav_info;
typedef struct nav_info NAV_INFO;

#endif /* INCNavInfoh */

