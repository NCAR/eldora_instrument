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
