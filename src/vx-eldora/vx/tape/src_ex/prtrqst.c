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
 * Revision 1.1  1994/01/06  21:31:33  craig
 * Initial revision
 *
 * Revision 1.3  1992/10/25  17:08:08  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/10/17  21:32:50  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  17:50:16  reif
 * Initial revision
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include <cipincl.h>
/************ PRINT REQUEST SENSE BLOCK *****************/

void print_rqst_sns()
{
printf("ERROR CODE = %X\n",rqst_sen.vld_err_code);     
printf("SEQ NUM = %X\n",rqst_sen.seg_num);     
printf("SENSE KEY = %X\n",rqst_sen.sense_key);     
printf("INFO MSB = %X\n",rqst_sen.info_msb);     
printf("INFO MID HI = %X\n",rqst_sen.info_mid_high);     
printf("INFO MID LO = %X\n",rqst_sen.info_mid_low);     
printf("INFO LSB = %X\n",rqst_sen.info_lsb);     
printf("ADIT SENSE LEN = %X\n",rqst_sen.addit_sense_len);     
printf("UNDR OVR CNTR = %X\n",rqst_sen.undr_ovr_cntr);     
printf("ADIT SENSE CODE = %X\n",rqst_sen.addit_sense_code);     
printf("ADDIT SENSE QUAL = %X\n",rqst_sen.addit_sense_qual);     
printf("ERR CNTR MSB = %X\n",rqst_sen.err_ctr_msb);     
printf("ERR CNTR MID = %X\n",rqst_sen.err_ctr_mid);     
printf("ERR CNTR LSB = %X\n",rqst_sen.err_ctr_lsb);     
printf("STAT1 = %X\n",rqst_sen.stat1);     
printf("STAT2 = %X\n",rqst_sen.stat2);     
printf("STAT3 = %X\n",rqst_sen.stat3);     
printf("REM TAPE MSB = %X\n",rqst_sen.rem_tape_msb);     
printf("REM TAPE MID = %X\n",rqst_sen.rem_tape_mid);     
printf("REM TAPE LSB = %X\n",rqst_sen.rem_tape_lsb);     
printf("TRKNG RTRY COUNT = %X\n",rqst_sen.trkng_retry_cntr);     
printf("RD WRT RTRY COUNT = %X\n",rqst_sen.rd_wrt_retry_cntr);     
printf("FAULT CODE = %X\n",rqst_sen.fault_code);     
taskDelay(200);
return;
}


