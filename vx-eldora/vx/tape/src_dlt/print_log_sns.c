/*
 *	$Id$
 *
 *	Module:		 print_log_sns
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/06/18  16:03:54  craig
 * Initial revision
 *
 *
 * description: Prints out values stored in the log sence structure
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/*********** PRINT LOG SENSE STRUCTURE *****************/

void print_log_sns()
{

char dummy;

union {
    long along;
    char achar[4];
}lcnt;

union {
    short ashort;
    char achar[4];
}scnt;

int tbytes_top;

printf("PAGE CODE: %2x SHOULD BE: 0x02\n",log_page->head.page_code & 0x7F);

scnt.achar[0] = log_page->head.page_length_msb;
scnt.achar[1] = log_page->head.page_length_lsb;
printf("PAGE LENGTH: %3d SHOULD BE: 68\n",scnt.ashort);

printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x00\n",
       log_page->sub_delay.code_msb  & 0x7F,log_page->sub_delay.code_lsb);
printf("MISC: %2x\n",log_page->sub_delay.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->sub_delay.length);
lcnt.achar[0] = log_page->sub_delay.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->sub_delay.pvalue_midh;
lcnt.achar[2] = log_page->sub_delay.pvalue_midl;
lcnt.achar[3] = log_page->sub_delay.pvalue_lsb;
printf("ERRORS CORRECTED WITH SUBSTANTIAL DELAYS: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x01\n",
       log_page->pos_delay.code_msb & 0x7F,log_page->pos_delay.code_lsb);
printf("MISC: %2x\n",log_page->pos_delay.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->pos_delay.length);
lcnt.achar[0] = log_page->pos_delay.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->pos_delay.pvalue_midh;
lcnt.achar[2] = log_page->pos_delay.pvalue_midl;
lcnt.achar[3] = log_page->pos_delay.pvalue_lsb;
printf("ERRORS CORRECTED WITH POSSIBLE DELAYS: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x02\n",
       log_page->total_rewrites.code_msb & 0x7F,
       log_page->total_rewrites.code_lsb);
printf("MISC: %2x\n",log_page->total_rewrites.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->total_rewrites.length);
lcnt.achar[0] = log_page->total_rewrites.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->total_rewrites.pvalue_midh;
lcnt.achar[2] = log_page->total_rewrites.pvalue_midl;
lcnt.achar[3] = log_page->total_rewrites.pvalue_lsb;
printf("TOTAL NUMBER OF REWRITES: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x03\n",
       log_page->total_errors_cor.code_msb & 0x7F,
       log_page->total_errors_cor.code_lsb);
printf("MISC: %2x\n",log_page->total_errors_cor.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->total_errors_cor.length);
lcnt.achar[0] = log_page->total_errors_cor.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->total_errors_cor.pvalue_midh;
lcnt.achar[2] = log_page->total_errors_cor.pvalue_midl;
lcnt.achar[3] = log_page->total_errors_cor.pvalue_lsb;
printf("TOTAL ERRORS CORRECTED: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x04\n",
       log_page->total_proc.code_msb & 0x7F,log_page->total_proc.code_lsb);
printf("MISC: %2x\n",log_page->total_proc.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->total_proc.length);
lcnt.achar[0] = log_page->total_proc.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->total_proc.pvalue_midh;
lcnt.achar[2] = log_page->total_proc.pvalue_midl;
lcnt.achar[3] = log_page->total_proc.pvalue_lsb;
printf("TOTAL TIME CORRECTION ALGORITHM PROCESSED: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x05\n",
       log_page->total_bytes.code_msb & 0x7F,log_page->total_bytes.code_lsb);
printf("MISC: %2x\n",log_page->total_bytes.misc);
printf("LENGTH: %2d SHOULD BE: 8\n",log_page->total_bytes.length);
lcnt.achar[0] = log_page->total_bytes.pvalue0 & 0x7F;
lcnt.achar[1] = log_page->total_bytes.pvalue1;
lcnt.achar[2] = log_page->total_bytes.pvalue2;
lcnt.achar[3] = log_page->total_bytes.pvalue3;
tbytes_top = lcnt.along;
lcnt.achar[0] = log_page->total_bytes.pvalue4 & 0x7F;
lcnt.achar[1] = log_page->total_bytes.pvalue5;
lcnt.achar[2] = log_page->total_bytes.pvalue6;
lcnt.achar[3] = log_page->total_bytes.pvalue7;

printf("TOTAL BYTES PROCESSED: %d (TOP) %d (BOTTOM)\n",tbytes_top,lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x06\n",
       log_page->total_errors_uncor.code_msb & 0x7F,
       log_page->total_errors_uncor.code_lsb);
printf("MISC: %2x\n",log_page->total_errors_uncor.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->total_errors_uncor.length);
lcnt.achar[0] = log_page->total_errors_uncor.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->total_errors_uncor.pvalue_midh;
lcnt.achar[2] = log_page->total_errors_uncor.pvalue_midl;
lcnt.achar[3] = log_page->total_errors_uncor.pvalue_lsb;
printf("TOTAL UNCORRECTED ERRORS: %d\n",lcnt.along);
 
printf("PARAMETER CODE: %2x%2x SHOULD BE: 0x8000\n",
       log_page->vendor_unique.code_msb,
       log_page->vendor_unique.code_lsb);
printf("MISC: %2x\n",log_page->vendor_unique.misc);
printf("LENGTH: %2d SHOULD BE: 4\n",log_page->vendor_unique.length);
lcnt.achar[0] = log_page->vendor_unique.pvalue_msb & 0x7F;
lcnt.achar[1] = log_page->vendor_unique.pvalue_midh;
lcnt.achar[2] = log_page->vendor_unique.pvalue_midl;
lcnt.achar[3] = log_page->vendor_unique.pvalue_lsb;
printf("VENDOR UNIQUE COUNT: %d\n",lcnt.along);

taskDelay(200);
return;
}

/**********************************************************/
/*  Routine to print out the supported log pages **********/
/**********************************************************/

void print_log_pgs()
{

union {
    short ashort;
    char achar[4];
}scnt;

int tbytes_top;

printf("PAGE CODE: %2x SHOULD BE: 0x00\n",suprt_pgs->page_code);

scnt.achar[0] = suprt_pgs->length_msb;
scnt.achar[1] = suprt_pgs->length_lsb;
printf("PAGE LENGTH: %3d SHOULD BE: 5\n",scnt.ashort);

printf("PAGE ONE: %2x  SHOULD BE 0x00\n",suprt_pgs->page1);
printf("PAGE TWO: %2x  SHOULD BE 0x02\n",suprt_pgs->page2);
printf("PAGE THREE: %2x  SHOULD BE 0x03\n",suprt_pgs->page3);
printf("PAGE FOUR: %2x  SHOULD BE 0x07\n",suprt_pgs->page4);
printf("PAGE FIVE: %2x  SHOULD BE 0x32\n",suprt_pgs->page5);

taskDelay(200);
return;
}


