/*
 *	$Id$
 *
 *	Module:	init_dma.c	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:48:23  craig
 * Initial revision
 *
 *
 * description: The purpose of this module is to initialize dma channels 0
 *              and 1 for interrupt operation after transfering 24 and 28
 *              characters from their respective gpib channels.  Please 
 *              refer to the MC68450 direct memory access controller data
 *              book for further details on the various registers and 
 *              about chip operation.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

void init_dma()
{

/********** STRUCTURE TO HOLD 24 BIT DMA REGISTER DATA *************/

  union
    {
	unsigned int dma0;
	unsigned short dma0_array[2];
	unsigned int dma1;
	unsigned short dma1_array[2];
    }dma;

/************** DMA CHANNEL 0 *****************/
  
  *d0ccr=0x10;
  *d0niv=0xF0; /* Load normal interrupt vector of 240 */
  *d0eiv=0xEF; /* Load error interrupt vector of 239 */
  *d0dcr=0x20; /* Set for burst mode, device with ACK, 8 bit port */
  *d0ocr=0x82; /* Device to memory xfer, byte operands, external request */
  *d0scr=0x04; /* MAR counts up, DAR does not count */
  *d0cpr=0x00; /* Channel priority 0 (highest) */
  *d0csr=0xff; /* Clear channel status register by writing all ones to it */
  *d0btc=0x1C; /* Base transfer count set for 28 bytes in continue mode */
  *d0mtc=0x1C; /* Memory transfer count set to 28 bytes before interrupt */
  *d0mfc=0x01; /* Memory function code set for AM of 2D */
  *d0bfc=0x01; /* Base function code set the same for continue mode */

/************ LOAD MEMORY AND BASE ADDRESS REGISTERS CHANNEL 0 ************/

  dma.dma0=(int)&xmit_array; /* Load address of character buffer */
  *d0mar1=dma.dma0_array[0]; /* Load memory address register 16 bits */ 
  *d0mar2=dma.dma0_array[1]; /* at a time. Two writes */
  *d0bar1=dma.dma0_array[0]; /* Load base address register the same way */
  *d0bar2=dma.dma0_array[1]; /* Used in continue mode of operation */

/************ DMA CHANNEL 1 ****************/

  *d1ccr=0x10;
  *d1niv=0xEE; /* Load normal interrupt vector of 238 */
  *d1eiv=0xED; /* Load error interrupt vector of 237 */
  *d1dcr=0x20; /* Set for burst mode, device with ACK, 8 bit port */
  *d1ocr=0x82; /* Device to memory xfer, byte operands, external request */
  *d1scr=0x04; /* MAR counts up, DAR does not count */
  *d1cpr=0x01; /* Channel priority 0 (highest) */
  *d1csr=0xff; /* Clear channel status register by writing all ones to it */
  *d1btc=0x18; /* Base transfer count set for 24 bytes in continue mode */
  *d1mtc=0x18; /* Memory transfer count set to 24 bytes before interrupt */
  *d1mfc=0x01; /* Memory function code set for AM of 2D */
  *d1bfc=0x01; /* Base function code set the same for continue mode */

/************ LOAD MEMORY AND BASE ADDRESS REGISTERS CHANNEL 1 ************/

  dma.dma1=(int)&testp_array; /* Load address of character buffer */
  *d1mar1=dma.dma1_array[0]; /* Load memory address register 16 bits */ 
  *d1mar2=dma.dma1_array[1]; /* at a time. Two writes */
  *d1bar1=dma.dma1_array[0]; /* Load base address register the same way */
  *d1bar2=dma.dma1_array[1]; /* Used in continue mode of operation */
}















