/*
 *	$Id$
 *
 *	Module:	start_ieee.c	 
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
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the fore and
 *              aft WAVETEK 8502A Peak Power Meters used for the fore 
 *              and aft peak power measurements.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <string.h>
#include <pwrDef.h>
#include <pwrFunc.h>

 void start_ieee(float xmit_csr_delay,float testp_csr_delay)
{
  unsigned char string_array[15];
  unsigned char number_array[11];
  int i;

/************ INITIALIZE STRING AND NUMBER ARRAYS *************/

  for(i=0; i<11; i++)
    {
	number_array[i]=0;
    }
  for(i=0; i<15; i++)
    {
	string_array[i]=0;
    }

/************* INITIALIZE POINTERS AND ISR VECTORS ************/

  init_ptrs();
  init_isr_vecs();

/******************* INITIALIZE GPIB AND DMA **********************/

  init_gpib();
  init_dma();

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

  strncpy(string_array,"PKAB\0",5); /* Select peak dual channel mode */  
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* on both power meters */
 
  strncpy(string_array,"UPDC\0",5); /* Select update continuously mode */ 
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* on both power meters */
 
  strncpy(string_array,"DATN\0",5); /* Select data normal mode of operation */
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* for both power meters */
 
  strncpy(string_array,"WATT\0",5); /* Select linear power output mode for */ 
  send_cmnd_string(XMIT,string_array); /* the fore and aft xmit power meter */

  strncpy(string_array,"TRGE\0",5); /* Select external trigger for both */ 
  send_cmnd_string(XMIT_AND_TESTP,string_array); /* power meters */

  strncpy(string_array,"DBMW\0",5); /* Select dbm output mode for the fore */ 
  send_cmnd_string(TESTP,string_array); /* and aft testpulse power meter */

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",XMIT_OFFSET_FORE); /* fore xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",XMIT_OFFSET_AFT); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"OFFA\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",TESTP_OFFSET_FORE); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"OFFB\0",5); /* Input system losses as offset for */
  sprintf(number_array,"%7.2f",TESTP_OFFSET_AFT); /* aft xmit power meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"CDLA\0",5); /*Set cursor delay for fore xmit meter */  
  sprintf(number_array,"%9.2f",xmit_csr_delay);
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);

  strncpy(string_array,"CDLB\0",5); /*Set cursor delay for fore xmit meter */  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT,string_array);
 
  strncpy(string_array,"CDLA\0",5); /* Set cursor delay for fore tp meter */  
  sprintf(number_array,"%9.2f",testp_csr_delay);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"CDLB\0",5); /* Set cursor delay for aft tp meter */  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(TESTP,string_array);

  strncpy(string_array,"RDLA\0",5); /* Set reference delay for fore xmit and tp meter */  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"RDLB\0",5); /* Set reference delay for aft xmit and tp meter */
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"SDLA\0",5); /* Set start delay for fore xmit and tp meters*/  
  sprintf(number_array,"%9.2f",0.0);  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

  strncpy(string_array,"SDLB\0",5); /* Set start delay for aft xmit and tp meters*/  
  strncat(string_array,number_array,strlen(number_array));
  send_cmnd_string(XMIT_AND_TESTP,string_array);

/*********** TURN PPM INTO TALKER, MZ7500 INTO LISTENER ************/

  listen();

/*********** START DMA CHANNEL 0 - GO! ***************/

  *d0csr=0xff; /* Clear channel status register by writing all ones */
  *d0ccr=0xC8; /* Set start, continue, and interrupt bits in */
               /* channel control register */

/*********** START DMA CHANNEL 1 - GO! **************/

  *d1csr=0xff; /* Clear channel status register by writing all ones */
  *d1ccr=0xC8; /* Set start, continue, and interrupt bits in the */
               /* channel control register */
}
