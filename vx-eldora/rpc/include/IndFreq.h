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
 * Revision 1.1  1992/07/28  17:23:46  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCIndFreqh
#define INCIndFreqh

struct indep_freq {
    char  indep_freq_id[4];	/* Identifier = INDF. */
    long  indep_freq_len;	/* Block size in bytes. */
};

typedef struct indep_freq indep_freq;
typedef struct indep_freq INDEP_FREQ;

#endif /* INCIndFreqh */

