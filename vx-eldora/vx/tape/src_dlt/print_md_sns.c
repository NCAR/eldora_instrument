/*
 *	$Id$
 *
 *	Module: print_md_sns		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description: Prints out the mode sense block
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include <tapeDef.h>
#include <tapeGbl.h>

/************** PRINT MODE SENSE BLOCKS ******************/

void print_md_sns()
{

printf("BUFFERED MODE AND SPEED = %X\n",mod_sen->buf_mode_spd);
printf("BLOCK DESC LEN = %X\n",mod_sen->blk_desc_len);

printf("DENSITY = %X\n",mod_sen->density);
printf("NUMBLKS MSB = %X\n",mod_sen->num_blks_msb);
printf("NUMBLKS MMID = %X\n",mod_sen->num_blks_mid);
printf("NUMBLKS LSB =%X\n",mod_sen->num_blks_lsb);
printf("RESVERVED = %X\n",mod_sen->res3);
printf("BLK LEN MSB = %X\n",mod_sen->blk_len_msb);
printf("BLK LEN MID = %X\n",mod_sen->blk_len_mid);
printf("BLK LEN LSB = %X\n",mod_sen->blk_len_lsb);

printf("PAGE CODE = %X\n",mod_sen->page_code);
printf("PAGE LENGTH = %X\n",mod_sen->page_len);
printf("CAP CAF ACTIVE FORMAT = %X\n",mod_sen->CAP_CAF_fmt);
printf("ACTIVE PARTITION = %X\n",mod_sen->active_par);
printf("WRITE BUFFER FULL RATIO = %X\n",mod_sen->write_full_ratio);
printf("READ BUFFER FULL RATIO = %X\n",mod_sen->read_full_ratio);
printf("WRITE DELAY TIME MSB = %X\n",mod_sen->write_dly_msb);
printf("WRITE DELAY TIME LSB = %X\n",mod_sen->write_dly_lsb);
printf("DBR BIS RSmk AVC SOCF RBO REW = %X\n",mod_sen->DBR_other_stuff);
printf("GAP SIZE = %X\n",mod_sen->gap_size);
printf("EOD DEFINED EEG SEW RES = %X\n",mod_sen->EOD_EGG_SEW);
printf("BUFFER SIZE AT EARLY WARNING MSB = %X\n",mod_sen->buff_size_msb);
printf("BUFFER SIZE AT EARLY WARNING MID = %X\n",mod_sen->buff_size_mid);
printf("BUFFER SIZE AT EARLY WARNING LSB = %X\n",mod_sen->buff_size_lsb);
printf("DATA COMPRESSION = %X\n",mod_sen->data_compress);
taskDelay(200);
return;
}
