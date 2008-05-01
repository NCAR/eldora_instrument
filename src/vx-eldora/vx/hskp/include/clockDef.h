/*
 * $Id$
 *
 * Module: clockDef.h  
 * Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 * Date:   $Date$
 *
 *
 * Description: This include module contains all of the addresses
 *              that are specific to the Bancomm clock card.
 *
 * revision history
 * ----------------
 * $Log: clockDef.h,v $
 * Revision 1.1  2005/08/04 22:27:39  granger
 * commit vx/hskp as copied from /net/eldora/eldora, except a few obsolete
 * (afaik) directories were removed, like hskp/src/clock/newclk and
 * hskp/src/arinc_NCAR
 *
 * Revision 1.1  2003/09/23  20:15:23  kapoor
 * Initial revision
 *
 * Revision 1.1  2003/09/23  20:15:23  kapoor
 * Initial revision
 *
 * Revision 2.0  2001/06/12 19:33:57  thor
 * *** empty log message ***
 *
 * Revision 1.2  2001/05/31 20:45:44  thor
 * Syncing source tree to ACE-ASIA
 *
 * Revision 1.4  1995/09/25  17:44:52  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/08/14  14:49:45  craig
 * Initial revision
 *
 *
 *        
 */


/* Base address of the card */

#define  BASE_CLOCK     0x4000       /* Base address of clock Card */

/* Offsets to the clock card registers */

#define  TIMEREQ 0x0A            /* Time request register (latches time) */
#define  TIME    0x0C            /* Starting address where time is stored */
#define  CLK_ACK 0x22            /* Acknowledge register */
#define  CLK_CMD 0x24            /* Command register */
#define  CLK_INTSTAT  0x2A       /* Interrupt status register */
#define  CLK_FIFO 0x26           /* FIFO for clock commends */

//Tom 3/24/08
#define  CLK_VECTOR 0x2C         // Interrupt Vector
#define  CLK_LEVEL 0x2E          // Interrupt Level
#define  CLK_MASK 0x28           // Interrupt Mask








