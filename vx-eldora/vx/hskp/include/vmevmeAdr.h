/*
 *	$Id$
 *
 *	Module:	vmevmeAdr.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  19:35:34  craig
 * Initial revision
 *
 *        
 */

/* Starting Addresses for Vme to Vme interface Memory Areas  */

#define FORE_STAND_START 0x400000  /* Fore radar data and handshake area 
                                   this area maps to 0xC00000 in the Fore
                                   radar processor */
#define AFT_STAND_START  0x500000  /* Fore radar data and handshake area 
                                   this area maps to 0xC00000 in the Aft
                                   radar processor */
#define FORE_SHORT_START 0x8000  /* Fore radar VME to VME interface short 
                                   I/O space Control area */
#define AFT_SHORT_START  0x2000  /* Aft radar VME to VME interface short 
                                   I/O space Control area */
#define VMEVME_SHORT_START 0xC000 /* Where the Short I/O space on the vme to
                                     vme interfaces in the radar processors
                                     are mapped. */

/* Offsets to the control registers in short I/O space */

#define LOCAL_COMMAND_REG   0x01   /* Local command register offset */
#define REMOTE_COMMAND_REG  0x09   /* Remote command register offset */
#define LOCAL_STATUS_REG    0x03   /* Local status register offset */
#define REMOTE_STATUS_REG   0x09   /* Remote status register offset */























