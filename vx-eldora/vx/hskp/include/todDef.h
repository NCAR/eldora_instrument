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
 *
 * description: The purpose of this module is to test the various functions
 *              of the TIME OF DAY board (TOD).  These include setting the
 *              Real Time Clock (RTC), reading time of power failure, writing
 *              and reading the various register on the RTC, setting up the 
 *              TOD for interrupts using the Bus Interrupter Module (BIM), the 
 *              RTC and the Power Monitor (PM) chip.  The software also
 *              allows the TOD to be set to IRIGB and reading time from the
 *              RTC.
 */

#ifndef INCtodDefh
#define INCtodDefh

/***************** SYMBOLIC CONSTANTS ***************************/

#define TOD_BASE 0x20004000 /*SHORT I/O SPACE*/

/**************** ICM7170 REGISTERS ****************************/

#define HUNDRETHS (0x0040 + TOD_BASE)
#define SECONDS (0x0043 + TOD_BASE)
#define MINUTES (0x0042 + TOD_BASE)
#define HOURS (0x0041 + TOD_BASE)
#define DAY (0x0045 + TOD_BASE)
#define MONTH (0x0044 + TOD_BASE)
#define YEAR (0x0046 + TOD_BASE)
#define DAY_OF_WK (0x0047 + TOD_BASE)
#define RAM_HUNDRETHS (0x0048 + TOD_BASE)
#define RAM_SEC (0x004B + TOD_BASE)
#define RAM_MIN (0x004A + TOD_BASE)
#define RAM_HR (0x0049 + TOD_BASE)
#define RAM_DAY (0x004D + TOD_BASE)
#define RAM_MONTH (0x004C + TOD_BASE)
#define RAM_YEAR (0x004E + TOD_BASE)
#define RAM_DAY_OF_WK (0x004F + TOD_BASE)
#define INT_STAT_AND_MASK (0x0050 + TOD_BASE)
#define CMND_REG (0x0051 + TOD_BASE)

/*************** IRIG-B REGISTERS ******************************/

#define ENIRIGB (0x0060 + TOD_BASE)
#define SELSTRG (0x0080 + TOD_BASE)
#define READ_MS (0x00A0 + TOD_BASE)
#define SEL_525 (0x00C0 + TOD_BASE)
#define RST_525 (0x00E0 + TOD_BASE)
#define SELSEC (0x0000 + TOD_BASE)
#define SELMIN (0x0001 + TOD_BASE)
#define SELHRS (0x0002 + TOD_BASE)
#define SELDAY1 (0x0003 + TOD_BASE)
#define SELDAY2 (0x0004 + TOD_BASE)

/*************** BIM REGISTERS *********************************/

#define BIM_CR0 (0x0020 + TOD_BASE)
#define BIM_CR1 (0x0022 + TOD_BASE)
#define BIM_CR2 (0x0024 + TOD_BASE)
#define BIM_CR3 (0x0026 + TOD_BASE)
#define BIM_VR0 (0x0028 + TOD_BASE)
#define BIM_VR1 (0x002A + TOD_BASE)
#define BIM_VR2 (0x002C + TOD_BASE)
#define BIM_VR3 (0x002E + TOD_BASE)

/*************** IRIG_B STATUS BITS ***************************/

#define INIT 0x00
#define LDDAY2 0x01
#define TMRKFND 0x02

#define IGREG1 2299161
#define IGREG2 (15+31L*(10+12L*15882))

#define scope

#endif /* INC */
