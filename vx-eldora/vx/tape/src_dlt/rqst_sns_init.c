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
rqst_sen->res1=0x00;     
rqst_sen->res2=0x00;     
rqst_sen->res3=0x00;     
rqst_sen->undr_ovr_cntr=0x00;     
rqst_sen->addit_sense_code=0x00;     
rqst_sen->addit_sense_qual=0x00;     
rqst_sen->res5=0x00;     
rqst_sen->res6=0x00;     
rqst_sen->err_ctr_msb=0x00;     
rqst_sen->err_ctr_mid=0x00;     
rqst_sen->err_ctr_lsb=0x00;     
rqst_sen->stat1=0x00;     
rqst_sen->stat2=0x00;     
rqst_sen->stat3=0x00;     
rqst_sen->res7=0x00;     
rqst_sen->rem_tape_msb=0x00;     
rqst_sen->rem_tape_mid=0x00;     
rqst_sen->rem_tape_lsb=0x00;     
rqst_sen->trkng_retry_cntr=0x00;     
rqst_sen->rd_wrt_retry_cntr=0x00;     
rqst_sen->fault_code=0x00;     
}
