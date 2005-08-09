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
 * Revision 1.2  1995/03/31  22:49:10  craig
 * don't know
 *
 * Revision 1.1  1994/01/06  21:31:29  craig
 * Initial revision
 *
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

mod_sel.res1 = 0;
mod_sel.res2 = 0;
mod_sel.buf_mode_spd = 0x10;  /* select buffered mode, and default speed */
mod_sel.blk_desc_len = 8;     /* block descriptor length */

/* block descriptor */

mod_sel.density = 0x19;       /* 62500 BPI, 64 track pairs */
mod_sel.num_blks_msb = 0;     /* Set to zero because DLT says so */
mod_sel.num_blks_mid = 0;
mod_sel.num_blks_lsb = 0;
mod_sel.res3 = 0;
mod_sel.blk_len_msb = 0;      /* Set to zero for variable length records */
mod_sel.blk_len_mid = 0;
mod_sel.blk_len_lsb = 0;

/* device configuration page */

mod_sel.page_code = 0x10;        /* page is device configuration */
mod_sel.page_len = 0xE;          /* 14 bytes follow in this page */
mod_sel.CAP_CAF_fmt = 0;         /* all must be zero */
mod_sel.active_par = 0;          /* Only partion zero is used */
mod_sel.write_full_ratio = 0;    /* zero: DLT figures this for us */
mod_sel.read_full_ratio = 0;     /* ditto */
mod_sel.write_dly_msb = 0;
mod_sel.write_dly_lsb = 0x32;    /* flush buffer evry 5 seconds */
mod_sel.DBR_other_stuff = 0x40;  /* do as the book says */ 
mod_sel.gap_size = 0;            /* not used */
mod_sel.EOD_EGG_SEW = 0x10;      /* enable end-of-data */
mod_sel.buff_size_msb = 0;       /* leave zero for noe */
mod_sel.buff_size_mid = 0;
mod_sel.buff_size_lsb = 0;
mod_sel.data_compress = 0;       /* disable data compression for now */
mod_sel.res3 = 0;

}


