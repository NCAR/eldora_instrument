/*
 *	$Id$
 *
 *	Module:	todDef.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:39:48  reif
 * Initial revision
 *
 *
 * description: These are definitions that are used in conjunction with
 *              The time-of-day (TOD) board 
 */

#ifndef INCtodDefh
#define INCtodDefh

/***************** SYMBOLIC CONSTANTS ***************************/

#define TOD_BASE (0x4000 + SHORT_BASE) /*0x4000 into SHORT I/O SPACE*/

/**************** ICM7170 REGISTERS ****************************/

#define HUNDRETHS (0x0040 + TOD_BASE)  /* Where to read hundreths of a sec */
#define SECONDS (0x0043 + TOD_BASE)    /* Where to read seconds */
#define MINUTES (0x0042 + TOD_BASE)    /* Where to read minutes */
#define HOURS (0x0041 + TOD_BASE)      /* Where to read hours */
#define DAY (0x0045 + TOD_BASE)        /* Where to read the day of month */
#define MONTH (0x0044 + TOD_BASE)      /* Where to read month */   
#define YEAR (0x0046 + TOD_BASE)       /* Where to read year */
#define DAY_O_W (0x0047 + TOD_BASE)    /* Where to read day of week */
#define RAM_HUNDRETHS (0x0048 + TOD_BASE)  /* Where to write a hundreths
                                              compare */
#define RAM_SEC (0x004B + TOD_BASE)   /* Where to write seconds compare */
#define RAM_MIN (0x004A + TOD_BASE)   /* Where to write minutes compare */
#define RAM_HR (0x0049 + TOD_BASE)    /* Where to write hours compare */
#define RAM_DAY (0x004D + TOD_BASE)   /* Where to write day of month compare */
#define RAM_MONTH (0x004C + TOD_BASE) /* Where to write month compare */
#define RAM_YEAR (0x004E + TOD_BASE)  /* Where to write year compare */
#define RAM_DAY_O_W (0x004F + TOD_BASE) /*Where to write day of week compare */
#define INT_STAT_AND_MASK (0x0050 + TOD_BASE) /* Interrupt status and mask */ 
#define TOD_CMND_REG (0x0051 + TOD_BASE)      /* Command register */

/********** Registers For Milliseconds, and 525 interrupt *******/ 

#define SEL_525 (0x00C0 + TOD_BASE)   /* Select for the 525 counter */
#define RST_525 (0x00E0 + TOD_BASE)   /* Reset the 525 counter */
#define READ_MS (0x00A0 + TOD_BASE)   /* Read the subsecond counter address */
#define MS_DIVISOR 31.25              /* Divisor to change subsecond counter
                                         number to milliseconds */

/*************** IRIG-B REGISTERS ******************************/

#define ENIRIGB (0x0060 + TOD_BASE)   /* Enable searching for IRIG-B sync */
#define SELSTRG (0x0080 + TOD_BASE)   /* Select status */
#define SELSEC (0x0000 + TOD_BASE)    /* Irig B seconds */
#define SELMIN (0x0001 + TOD_BASE)    /* IRIG-B minutes */
#define SELHRS (0x0002 + TOD_BASE)    /* IRIG-B hours */
#define SELDAY1 (0x0003 + TOD_BASE)   /* IRIG-B day byte # 1 */
#define SELDAY2 (0x0004 + TOD_BASE)   /* IRIG-B day byte # 2 */


/*************** BIM REGISTERS *********************************/

#define BIM_CR0 (0x0020 + TOD_BASE)  /* BIM control register #0 */
#define BIM_CR1 (0x0022 + TOD_BASE)  /* BIM control register #1 */
#define BIM_CR2 (0x0024 + TOD_BASE)  /* BIM control register #2 */
#define BIM_CR3 (0x0026 + TOD_BASE)  /* BIM control register #3 */
#define BIM_VR0 (0x0028 + TOD_BASE)  /* BIM interrupt vector register #0 */
#define BIM_VR1 (0x002A + TOD_BASE)  /* BIM interrupt vector register #1 */
#define BIM_VR2 (0x002C + TOD_BASE)  /* BIM interrupt vector register #2 */
#define BIM_VR3 (0x002E + TOD_BASE)  /* BIM interrupt vector register #3 */

/*************** IRIG_B STATUS BITS ***************************/

#define IRIGB_INIT 0x00              /* Initializing */
#define LDDAY2 0x01                  /* Complete IRIG-B sequence has
					been loaded */
#define TMRKFND 0x02                 /* Time mark has been found */

#endif /* INC */
