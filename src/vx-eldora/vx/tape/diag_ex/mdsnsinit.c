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
mod_sen.sense_data_len=0x00;
mod_sen.medium=0x00;
mod_sen.WP_buf_mode=0x00;
mod_sen.blk_desc_len=0x00;
mod_sen.density=0x00;
mod_sen.num_blks_msb=0x00;
mod_sen.num_blks_mid=0x00;
mod_sen.num_blks_lsb=0x00;
mod_sen.res3=0x00;
mod_sen.blk_len_msb=0x00;
mod_sen.blk_len_mid=0x00;
mod_sen.blk_len_lsb=0x00;
mod_sen.vend_parms=0x00;
mod_sen.res4=0x00;
mod_sen.motion_thr=0x00;
mod_sen.recon_thr=0x00;
mod_sen.gap_thr=0x00;
}
