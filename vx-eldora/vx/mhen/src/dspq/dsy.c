/*
 *	$Id$
 *
 *	Module:	DSY	 
 *	Original Author: Grant Gray 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 * Sets the daisy-chain latch.
 *
 * Arguments:
 *
 * 1)       Freq #
 * 2)       Board #
 * 3)       Daisy chain value
 *
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";


#include "vxWorks.h"
#include "stdioLib.h"
#include "ELDRP7.h"

void dsy(freq,board,daisy_chain_val)
    unsigned long board, freq;
    unsigned int daisy_chain_val;
{
    unsigned short *vbd;
 
    freq = (freq << 24) + EXTD_BASE;     /* freq # * 10**6 + EXTD_BASE */
    board = board << 20;                /* board # * 10**5 */
    board += freq;                      /* full base addr of board */
    vbd = (unsigned short *)board;

    *vbd = (unsigned short)daisy_chain_val;
}

