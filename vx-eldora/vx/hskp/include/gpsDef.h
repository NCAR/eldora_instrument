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
 *
 * description: These are the definitions used to handle the Global Positioning
 *              System (GPS) interface.  This interface consists of a Matrix
 *              CPU-100 Board running software in an EPROM that can handle the  *              Trimble TANS II GPS receiver.
 */

#ifndef INCgpsDefh
#define INCgpsDefh

/*****************  SET ADDRESSES IN THE CPU100 Dual Port RAM  ************/

#define GPS_BASE     0x600000   /* BASE ADDRESS OF CPU100 Dual port Ram */
#define GPS_STATUS   0x0        /* Offset to the status word */
#define GPS_DATA_LOC 0x4        /* Offset to the data location long word */
#define GPS_CMND     0x8        /* Offset to the GPS command Word */
#define GPS_BUF_0    0x10       /* Offset to the first GPS data Buffer */
#define GPS_BUF_1    0x10       /* Offset to the second GPS data Buffer */

#define scope

#endif /* INC */



