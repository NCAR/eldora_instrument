/*
 *	$Id$
 *
 *	Module:	gpsDef.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/19  19:33:44  craig
 * Initial revision
 *
 *
 * description: These are the definitions used to handle the Global Positioning
 *              System (GPS) interface.  This interface consists of a Matrix
 *              CPU-100 Board running software in an EPROM that can handle the  *              Trimble TANS II GPS receiver.
 */

#ifndef INCgpsDefh
#define INCgpsDefh

/*****************  SET ADDRESSES IN THE CPU100 Dual Port RAM  ************/

#define GPS_BASE     0x600000   /* BASE ADDRESS OF CPU100 Dual port Ram */
#define GPS_STATUS   0x10000    /* Offset to the status word */
#define GPS_DATA_LOC 0x10004    /* Offset to the data location long word */
#define GPS_COMMAND  0x10008    /* Offset to the GPS command Word */
#define GPS_MBOX     0x1000A    /* Offset to the GPS mailbox Command */
#define GPS_HNDSHK   0x10010    /* Offset to the GPS handshake area */


#endif /* INC */



