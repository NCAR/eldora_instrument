/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1993/09/22  15:27:29  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/25  17:08:02  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:40  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:11  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>

/*********** INITIALIZE MODE SELECT PARAMETER BLOCK ********/

void md_sel_init()
{
/******** PARAMETER LIST HEADER *********/

mod_sel.res1 = 0x00;
mod_sel.res2 = 0x00;
mod_sel.buf_mode = 0x10; /* BUFFERED MODE */
mod_sel.blk_desc_len = 0x08; /* FULL BLOCK DESCRIPTOR LENGTH */

/********** BLOCK DESCRIPTOR ***********/

mod_sel.density = 0x15; /* EXB 8500 FORMAT FULL ERROR CHECKING */
mod_sel.num_blks_msb = 0x00; /* SINCE EXB 8500 ONLY SUPPORTS ONE */
mod_sel.num_blks_mid = 0x00; /* DENSITY, 0 IS CORRECT FOR THESE  */
mod_sel.num_blks_lsb = 0x00; /* FIELDS */
mod_sel.res3 = 0x00;
mod_sel.blk_len_msb = 0x00; /* SELECTED VARIABLE LENGTH */
mod_sel.blk_len_mid = 0x00; /* LOGICAL BLOCKS WITH THESE */
mod_sel.blk_len_lsb = 0x00; /* FIELDS */

/*********** VENDOR UNIQUE PARAMETERS *********/

mod_sel.vend_parms =0x02; 
mod_sel.res4 = 0x00;
mod_sel.motion_thr = 0x30; /* 500 KBYTEs BEFORE TAPE MOTION */
mod_sel.recon_thr = 0xD0; /* 500 KBYTES BEFORE RECONNECT */
mod_sel.gap_thr = 0x00; /* GAP BLOCKS WRITTEN BEFORE TAPE STOPS */

/* THESE FIELD DESCRIPTIONS CAN BE FOUND BEGINNING ON PAGE */
/* 79 of EXB 8500 MANUAL UNDER MODE SELECT */
}


