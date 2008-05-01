/*
 *	$Id$
 *
 *	Module:		 TimeSeries.h
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
#ifndef INCTimeSeriesh
#define INCTimeSeriesh

struct time_series {
    char  time_series_id[4];	/* Identifier = TIME. */
    long  time_series_len;	/* Block size in bytes. */
};

typedef struct time_series time_series;
typedef struct time_series TIME_SERIES;

#endif /* INCTimeSeriesh */

