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
 * Revision 1.1  1994/01/06  21:31:30  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:04  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:43  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:12  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/**************INITIALIZE MODE SENSE BLOCK ****************/

void md_sns_init()
{

/* parameter list header */

mod_sen.res1 = 0x00;
mod_sen.res2 = 0x00;
mod_sen.buf_mode_spd = 0x00;
mod_sen.blk_desc_len = 0x00;

/* Block descriptor */

mod_sen.density = 0x00;
mod_sen.num_blks_msb = 0x00;
mod_sen.num_blks_mid = 0x00;
mod_sen.num_blks_lsb = 0x00;
mod_sen.res3 = 0x00;
mod_sen.blk_len_msb = 0x00;
mod_sen.blk_len_mid = 0x00;
mod_sen.blk_len_lsb = 0x00;

/* device configuration page */

mod_sen.page_code = 0x00;
mod_sen.page_len = 0x00;
mod_sen.CAP_CAF_fmt = 0x00;
mod_sen.active_par = 0x00;
mod_sen.write_full_ratio = 0x00;
mod_sen.read_full_ratio = 0x00;
mod_sen.write_dly_msb = 0x00;
mod_sen.write_dly_lsb = 0x00;
mod_sen.DBR_other_stuff = 0x00;
mod_sen.gap_size = 0x00;
mod_sen.EOD_EGG_SEW = 0x00;
mod_sen.buff_size_msb = 0x00;
mod_sen.buff_size_mid = 0x00;
mod_sen.buff_size_lsb = 0x00;
mod_sen.data_compress = 0x00;
mod_sen.res4 = 0x00;

}
