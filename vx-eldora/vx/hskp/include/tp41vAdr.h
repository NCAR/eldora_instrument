/*
 *	$Id$
 *
 *	Module:	tp41vAdr.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  19:35:31  craig
 * Initial revision
 *
 *        
 */

/* Starting Addresses for Various Memory Areas on the VMEBus */

#define STANDARD_BASE  0x30000000   /* Where the VME Standard Address Space
                                        starts on the tp41vb CPU */
#define EXTEND_BASE    0x80000000   /* Where the VME Extended Address Space
                                       starts on the tp41vb CPU (this is paged
                                       memory */
#define SHORT_BASE    0x20000000   /* Where the VME Short I/O Address Space
                                       starts on the tp41vb CPU */
#define DRAM_BASE     0x40000000   /* Where DRAM starts in tp41v space */
#define TP41_MBOX_0   0x3FF000     /* Where the zeroth mailbox starts */
