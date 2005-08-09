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
 * Revision 1.3  1992/10/25  17:08:06  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:47  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:15  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/************** PRINT MODE SENSE BLOCKS ******************/

void print_md_sns()
{
printf("MODE DATA LENGTH = %X\n",mod_sen.sense_data_len);
printf("MEDIUM TYPE = %X\n",mod_sen.medium);
printf("WRITE PROTECT AND BUFFERED MODE = %X\n",mod_sen.WP_buf_mode);
printf("BLOCK DESCRIPTOR LENGTH = %X\n",mod_sen.blk_desc_len);
printf("DENSITY = %X\n",mod_sen.density);
printf("NUMBER OF BLOCKS MSB = %X\n",mod_sen.num_blks_msb);
printf("NUMBER OF BLOCKS MMID = %X\n",mod_sen.num_blks_mid);
printf("NUMBER OF BLOCKS LSB =%X\n",mod_sen.num_blks_lsb);
printf("RESERVED3 = %X\n",mod_sen.res3);
printf("BLOCK LENGTH MSB = %X\n",mod_sen.blk_len_msb);
printf("BLOCK LENGTH MID = %X\n",mod_sen.blk_len_mid);
printf("BLOCK LENGTH LSB = %X\n",mod_sen.blk_len_lsb);
printf("VENDOR_UNIQUE PARAMETERS = %X\n",mod_sen.vend_parms);
printf("RESEVERD4 = %X\n",mod_sen.res4);
printf("MOTION THRESHOLD = %X\n",mod_sen.motion_thr);
printf("RECONNECT THRESHOLD = %X\n",mod_sen.recon_thr);
printf("GAP THRESHOLD = %X\n",mod_sen.gap_thr);
taskDelay(200);
return;
}
