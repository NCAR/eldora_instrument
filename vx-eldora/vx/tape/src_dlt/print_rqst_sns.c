/*
 *	$Id$
 *
 *	Module: print_rqst_sns 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Prints out the request sence structure
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/************ PRINT REQUEST SENSE BLOCK *****************/

void print_rqst_sns()
{

printf("ERROR CODE = %X\n",rqst_sen->vld_err_code);     
printf("SEGMENT NUMBER = %X\n",rqst_sen->seg_num);     
printf("FLMRK, EOM, ILI and SENSE KEY = %X\n",rqst_sen->sense_key);     
printf("INFO MSB = %X\n",rqst_sen->info_msb);     
printf("INFO MID HI = %X\n",rqst_sen->info_mid_high);     
printf("INFO MID LO = %X\n",rqst_sen->info_mid_low);     
printf("INFO LSB = %X\n",rqst_sen->info_lsb);     
printf("ADDITIONAL SENSE LEN = %X\n",rqst_sen->addit_sense_len);     
printf("COMMAND SPECIFIC MSB %X\n",rqst_sen->cmnd_spcfc_msb);     
printf("COMMAND SPECIFIC MID HI %X\n",rqst_sen->cmnd_spcfc_mid_high);     
printf("COMMAND SPECIFIC MID LO %X\n",rqst_sen->cmnd_spcfc_mid_low);     
printf("COMMAND SPECIFIC LSB %X\n",rqst_sen->cmnd_spcfc_lsb);     
printf("ADDITIONAL SENSE CODE = %X\n",rqst_sen->addit_sense_code);     
printf("ADDITIONAL SENSE QUALIFIER = %X\n",rqst_sen->addit_sense_qual);     
printf("SUB ASSEMBLY CODE = %X\n",rqst_sen->sub_assembly_code);     
printf("SKSV, CD, BPV, BIT POINTER = %X\n",rqst_sen->SKSV_CD_BPV_bitpntr);     
printf("FIELD POINTER MSB = %X\n",rqst_sen->fld_pntr_msb);     
printf("FIELD POINTER LSB = %X\n",rqst_sen->fld_pntr_lsb);     
printf("INTERNAL STATUS CODE = %X\n",rqst_sen->internal_stat);     

taskDelay(200);
return;
}


