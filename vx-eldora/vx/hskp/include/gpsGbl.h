/*
 *	$Id$
 *
 *	Module:	gpsGbl.h	 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  17:30:06  craig
 * Initial revision
 *
 * Revision 1.1  1992/08/19  19:33:44  craig
 * Initial revision
 *
 *
 * description: These are the global variables used to handle the
 *              Global Positioning System (GPS) interface.
 *              This interface consists of a Matrix
 *              CPU-100 Board running software in an EPROM that can handle the
 *              Trimble TANS II GPS receiver.
 */

#ifndef INCgpsGblh
#define INCgpsGblh

/*****************  SET ADDRESSES IN THE CPU100 Dual Port RAM  ************/

scope char *gps_status;    /* Pointer to the status word */
scope short *gps_d_loc_h;   /* Pointer to the data location high word */
scope short *gps_d_loc_l;   /* Pointer to the data location low word */
scope short *gps_command;   /* Pointer to the GPS command Word */
scope short *gps_mbox;      /* Pointer to the GPS mailbox Command */
scope short *gps_hndshk;    /* Pointer to the GPS handshake area */
scope char *tp41_mbox_0;    /* Pointer to the zeroth mailbox */

#endif /* INC */



