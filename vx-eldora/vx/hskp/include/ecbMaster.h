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
 * Revision 1.1  1992/05/29  17:53:54  shawn
 * Initial revision
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

#define BIMCR0       0x01      /* OFFSET TO BIM CONTROL REGISTER 0 */
#define BIMCR1       0x03      /* OFFSET TO BIM CONTROL REGISTER 1 */
#define BIMCR2       0x05      /* OFFSET TO BIM CONTROL REGISTER 2 */
#define BIMCR3       0x07      /* OFFSET TO BIM CONTROL REGISTER 3 */
#define BIMVR0       0x09      /* OFFSET TO BIM VECTOR REGISTER 0 */
#define BIMVR1       0x0b      /* OFFSET TO BIM VECTOR REGISTER 1 */
#define BIMVR2       0x0d      /* OFFSET TO BIM VECTOR REGISTER 2 */
#define BIMVR3       0x0f      /* OFFSET TO BIM VECTOR REGISTER 3 */

#define ECBEOS       0x00      /* Special End-Of-Sequence Character */

