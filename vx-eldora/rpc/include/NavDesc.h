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
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCNavDesch
#define INCNavDesch

struct nav_descript {
    char  nav_descript_id[4];	/* Identifier = NDDS. */
    long  nav_descript_len;	/* Block size in bytes. */
    short ins_flag;		/* 0 = no INS data, 1 = data recorded. */
    short gps_flag;		/* 0 = no GPS data, 1 = data recorded. */
    short minirims_flag;	/* 0 = no MiniRIMS data, 1 = data recorded. */
    short kalman_flag;		/* 0 = no kalman data, 1 = data recorded. */
};

typedef struct nav_descript nav_descript;
typedef struct nav_descript NAVDESC;

#endif /* INCNavDesch */

