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
 * Revision 1.2  1995/03/31  22:50:02  craig
 * Modified for the DLT
 *
 * Revision 1.1  1994/01/06  21:31:35  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:11  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:56  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:20  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <cipincl.h>
/*********** INITIALIZE REQUEST SENSE BLOCK **************/

void rqst_sns_init()
{
rqst_sen.vld_err_code=0x00;     
rqst_sen.seg_num=0x00;     
rqst_sen.sense_key=0x00;     
rqst_sen.info_msb=0x00;     
rqst_sen.info_mid_high=0x00;     
rqst_sen.info_mid_low=0x00;     
rqst_sen.info_lsb=0x00;     
rqst_sen.addit_sense_len=0x00;     
rqst_sen.cmnd_spcfc_msb =0x00;     
rqst_sen.cmnd_spcfc_mid_high =0x00;     
rqst_sen.cmnd_spcfc_mid_low = 0x00;     
rqst_sen.addit_sense_code=0x00;     
rqst_sen.addit_sense_qual=0x00;     
rqst_sen.sub_assembly_code=0x00;     
rqst_sen.SKSV_CD_BPV_bitpntr=0x00;     
rqst_sen.fld_pntr_msb=0x00;     
rqst_sen.fld_pntr_lsb=0x00;     
rqst_sen.internal_stat=0x00;     
}
