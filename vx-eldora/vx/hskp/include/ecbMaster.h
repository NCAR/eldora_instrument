/*
 *	$Id$
 *
 *	Module:	ecbMaster.h	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description: Contains general #defines (offsets, etc.) for ECB MASTER card.
 *        
 */

#define MASTERBASE 0x30800000 /* ECB MASTER BASE ADDRESS */

#define MASTERDPRAM0 0x0000    /* OFFSET TO DPRAM BANK 0 (read-write) */
#define MASTERDPRAM1 0x4000    /* OFFSET TO DPRAM BANK 1 (read-write) */
#define MASTERDPRAM2 0x8000    /* OFFSET TO DPRAM BANK 2 (read-write) */
#define MASTERDPRAM3 0xC000    /* OFFSET TO DPRAM BANK 3 (read-write) */

#define MASTERIN     0xF000    /* OFFSET TO "IN" FIFO (write-only) */
#define MASTEROUT    0xF100    /* OFFSET TO "OUT" FIFO (read-only) */
#define MASTERCTL    0xF200    /* OFFSET TO CONTROL REGISTER (write-only) */
#define MASTERSTAT   0xF300    /* OFFSET TO STATUS REGISTER (read-only) */
#define MASTERBIM    0xF400    /* OFFSET TO BIM (read-write) */
#define MASTERNVXFR  0xF500    /* OFFSET TO NVRAM TRANSFER (read-write) */
#define MASTERNVRAM  0xF600    /* OFFSET TO NVRAM (read-write) */
