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
printf("MODE DATA LEN = %X\n",mod_sen.sense_data_len);
printf("MEDIUM TYPE = %X\n",mod_sen.medium);
printf("WRT PROT AND BUFF MODE = %X\n",mod_sen.WP_buf_mode);
printf("BLOCK DESC LEN = %X\n",mod_sen.blk_desc_len);
printf("DENSITY = %X\n",mod_sen.density);
printf("NUMBLKS MSB = %X\n",mod_sen.num_blks_msb);
printf("NUMBLKS MMID = %X\n",mod_sen.num_blks_mid);
printf("NUMBLKS LSB =%X\n",mod_sen.num_blks_lsb);
printf("RESV3 = %X\n",mod_sen.res3);
printf("BLK LEN MSB = %X\n",mod_sen.blk_len_msb);
printf("BLK LEN MID = %X\n",mod_sen.blk_len_mid);
printf("BLK LEN LSB = %X\n",mod_sen.blk_len_lsb);
printf("VEND PARMS = %X\n",mod_sen.vend_parms);
printf("RESV4 = %X\n",mod_sen.res4);
printf("MOTION THR = %X\n",mod_sen.motion_thr);
printf("RECON THR = %X\n",mod_sen.recon_thr);
printf("GAP THR = %X\n",mod_sen.gap_thr);
taskDelay(200);
return;
}
