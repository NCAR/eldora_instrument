/*
 *	$Id$
 *
 *	Module:	CALL_LG	 
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
 
 Purpose:       Call load_gatesb with dummy arguments.
                Diagnostic for debugging load_gatesb module
 Date Written:  09/14/92
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ELDRP7.h"

void call_lg()
{
    short num_gates[6],gate_dist1[2],gate_dist2[2],gate_dist3[3];
    short gate_dist4[2],gate_dist5[2];

    /* set up 512 gates on freq 1-5 */
    num_gates[0] = 512;
    num_gates[1] = 512;
    num_gates[2] = 512;
    num_gates[3] = 512;
    num_gates[4] = 512;
    num_gates[5] = 0;   /* meaningless, but it's in the header */
    
    gate_dist1[0] = 120;
    gate_dist1[1] = 60;
    gate_dist2[0] = 120;
    gate_dist2[1] = 60;
    gate_dist3[0] = 120;
    gate_dist3[1] = 60;
    gate_dist4[0] = 120;
    gate_dist4[1] = 60;
    gate_dist5[0] = 120;
    gate_dist5[1] = 60;
    
    load_gatesb(num_gates,gate_dist1,gate_dist2,gate_dist3,gate_dist4,gate_dist5);
}
