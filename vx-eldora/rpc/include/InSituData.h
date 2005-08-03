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
 * Revision 1.2  1992/08/11  13:47:18  thor
 * Fixed typo.
 *
 * Revision 1.1  1992/07/28  17:25:10  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCInsituDatah
#define INCInsituDatah

struct insitu_data {
    char  insitu_data_id[4];	/* Identifier = ISIT. */
    long  insitu_data_len;	/* Block size in bytes. */
    short julian_day;
    short hours;
    short minutes;
    short seconds;
};

typedef struct insitu_data insitu_data;
typedef struct insitu_data INSITU_DATA;

#endif /* INCInsituDatah */

