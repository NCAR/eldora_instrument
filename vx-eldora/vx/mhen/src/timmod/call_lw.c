/*
 *	$Id$
 *
 *	Module:	CALL_LW	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/12/04  18:00:40  eric
 * Initial revision
 *
 *
 description:
 
 Purpose:       Call load_waveformb with dummy arguments.
                Diagnostic for debugging load_waveformb module
 Date Written:  09/14/92
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

void call_lw()

{
    short num_chips[6],total_pcp,chip_offset[6],chip_width[6];
    char blank_chip[256];
    float repeat_seq;
    
    /* set up two-frequency stepped chirp, single prf (non-staggerred) */
    num_chips[0] = 1;
    num_chips[1] = 1;
    num_chips[2] = 0;
    num_chips[3] = 0;
    num_chips[4] = 0;
    num_chips[5] = 1;
    
    total_pcp    = 1;
    
    chip_offset[0] = 5;
    chip_width[0]  = 30;
    chip_offset[1] = 35;
    chip_width[1]  = 30;
    chip_offset[2] = 0;
    chip_width[2]  = 0;
    chip_offset[3] = 0;
    chip_width[3]  = 0;
    chip_offset[4] = 0;
    chip_width[4]  = 0;
    chip_offset[5] = 1;
    chip_width[5]  = 60;
    
    blank_chip[0] = 0x23;  /* PK, chip(0), and chip(1) */
    
    repeat_seq = 1;  /* 1kHz prf */
    
    load_waveformb(num_chips,blank_chip,&repeat_seq,&total_pcp,chip_offset,chip_width);
}
