/*
 *	$Id$
 *
 *	Module: rqst_sns_init
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/09/03  16:41:24  craig
 * cleaned up
 *
 * Revision 1.1  1996/06/18  16:08:05  craig
 * Initial revision
 *
 *
 * description: Initializes the request sense structure
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/*********** INITIALIZE REQUEST SENSE BLOCK **************/

void rqst_sns_init()
{

rqst_sen->vld_err_code=0x00;     
rqst_sen->seg_num=0x00;     
rqst_sen->sense_key=0x00;     
rqst_sen->info_msb=0x00;     
rqst_sen->info_mid_high=0x00;     
rqst_sen->info_mid_low=0x00;     
rqst_sen->info_lsb=0x00;     
rqst_sen->addit_sense_len=0x00;     
rqst_sen->cmnd_spcfc_msb =0x00;     
rqst_sen->cmnd_spcfc_mid_high =0x00;     
rqst_sen->cmnd_spcfc_mid_low = 0x00;     
rqst_sen->addit_sense_code=0x00;     
rqst_sen->addit_sense_qual=0x00;     
rqst_sen->sub_assembly_code=0x00;     
rqst_sen->SKSV_CD_BPV_bitpntr=0x00;     
rqst_sen->fld_pntr_msb=0x00;     
rqst_sen->fld_pntr_lsb=0x00;     
rqst_sen->internal_stat=0x00;     

}
