/*
 *	$Id$
 *
 *	Module:	pwr1.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:38:48  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the two WAVETEK 8502A
 *              Peak Power Meters used for fore and aft peak power 
 *              measurements.  The program also converts the string data from 
 *              the 8502A to binary and writes it to tape.
 */

#ifndef INCpwrDefh
#define INCpwrDefh

/*****************  SET BASE ADDRESS OF THE MZ7500 GPIB BOARD  ************/

#define GPIBCHAN1 0x20006000 /* BASE ADDRESS OF MZ7500 */
#define GPIBCHAN2 0x20006040
#define DMACHAN0 0x20006100
#define DMACHAN1 0x20006140
#define DMACHAN2 0x20006180
#define DMACHAN3 0x200061C0

/*****************  SET UP MZ7500 GPIB REGISTER OFFSETS  ******************/

#define GPIBOFF0 0x01 /* INTERRUPT STATUS REG/MASK 0 */
#define GPIBOFF1 0X03 /* INTERRUPT STATUS REG/MASK 1 */
#define GPIBOFF2 0X05 /* ADDRESS STATUS REGISTER */
#define GPIBOFF3 0x07 /* BUS STATUS REG/AUX CMND REG */
#define GPIBOFF4 0x09 /* ADDRESS REGISTER */
#define GPIBOFF5 0X0B /* SERIAL POLL REGISTER */
#define GPIBOFF6 0X0D /* CMD PASS THRU/PARALLEL POLL REGISTER */
#define GPIBOFF7 0x0F /* DATA IN/DATA OUT REGISTER */

/*****************  SET UP MZ7500 DMA REGISTER OFFSETS  *******************/

#define DMAOFF0 0X00 /* CHANNEL STATUS REGISTER */
#define DMAOFF1 0x01 /* CHANNEL ERROR REGISTER */
#define DMAOFF2 0X04 /* DEVICE CONTROL REGISTER */
#define DMAOFF3 0X05 /* OPERATION CONTROL REGISTER */
#define DMAOFF4 0X06 /* SEQUENCE CONTROL REGISTER */
#define DMAOFF5 0X07 /* CHANNEL CONTROL REGISTER */
#define DMAOFF6 0X0A /* MEMORY TRANSFER COUNTER */
#define DMAOFF7 0X0C /* MEMORY ADDRESS REGISTER */
#define DMAOFF8 0X14 /* DEVICE ADDRESS REGISTER */
#define DMAOFF9 0X1A /* BASE TRANSFER COUNTER */
#define DMAOFF10 0X1C /* BASE ADDRESS REGISTER */
#define DMAOFF11 0X25 /* NORMAL INTERRUPT VECTOR REGISTER */
#define DMAOFF12 0X27 /* ERROR INTERRUPT VECTOR REGISTER  */
#define DMAOFF13 0X2D /* CHANNEL PRIORITY REGISTER */
#define DMAOFF14 0X29 /* MEMORY FUNCTION CODE REGISTER */
#define DMAOFF15 0X31 /* DEVICE FUNCTION CODE REGISTER */
#define DMAOFF16 0X39 /* BASE FUNCTION CODE REGISTER */
#define DMAOFF17 0XFF /* GENERAL  CONTROL REGISTER */

/****************  SET UP INTERRUPT VECTOR ADDRESSES  *********************/

#define INT0  /* USER INTERRUPT VECTOR 0 */
#define INT1  /* USER INTERRUPT VECTOR 1 */
#define INT2  /* USER INTERRUPT VECTOR 2 */
#define INT3  /* USER INTERRUPT VECTOR 3 */
#define INT4  /* USER INTERRUPT VECTOR 4 */
#define INT5  /* USER INTERRUPT VECTOR 5 */
#define INT6  /* USER INTERRUPT VECTOR 6 */
#define BINT  /* BUS CLEAR INTERRUPT */
#define NORM_INT 250
#define ERR_INT 249

#endif /* INC */
