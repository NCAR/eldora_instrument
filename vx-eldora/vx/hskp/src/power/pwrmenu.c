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
 * Revision 1.1  1992/08/14  21:43:30  reif
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

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <pwrIncl.h>

/******************** MAIN PROGRAM ****************************/

float pwr_menu(unsigned long channel)
{
volatile unsigned char *ism0,*ism1,*acr,*adr,*dido;
unsigned char xmt_array[25], rcv_array[25];
unsigned char *csr,*dcr,*ocr,*scr,*chr,*niv,*eiv,*mtc,*mar,*mfc;
int i;
unsigned char *cpr,*mfcr,response;
float rcv1,rcv2;

/***************  GPIB REGISTER POINTERS  *********************************/

ism0 = (unsigned char *)(channel + GPIBOFF0);
ism1 = (unsigned char *)(channel + GPIBOFF1);
acr = (unsigned char *)(channel + GPIBOFF3);
adr = (unsigned char *)(channel + GPIBOFF4);
dido = (unsigned char *)(channel + GPIBOFF7);
  
if(intConnect((VOIDFUNCPTR *)(NORM_INT*4),(VOIDFUNCPTR)norm_isr,GPIBCHAN1)==OK)
  printf("OK\n");
if(intConnect((VOIDFUNCPTR *)(ERR_INT*4),(VOIDFUNCPTR)err_isr,GPIBCHAN1)==ERROR)
  printf("OK\n");
sysIntEnable(3);

/***************  DMA REGISTER POINTERS  **********************************/

csr = (unsigned char *)(DMACHAN0 + DMAOFF0);
dcr = (unsigned char *)(DMACHAN0 + DMAOFF2);
ocr = (unsigned char *)(DMACHAN0 + DMAOFF3);
scr = (unsigned char *)(DMACHAN0 + DMAOFF4);
chr = (unsigned char *)(DMACHAN0 + DMAOFF5);
mtc = (unsigned char *)(DMACHAN0 + DMAOFF6);
mar = (unsigned char *)(DMACHAN0 + DMAOFF7);
niv = (unsigned char *)(DMACHAN0 + DMAOFF11);
eiv = (unsigned char *)(DMACHAN0 + DMAOFF12);
mfc = (unsigned char *)(DMACHAN0 + DMAOFF14);
cpr = (unsigned char *)(DMACHAN0 + DMAOFF13);

/*************** SET UP MZ7500 AS TALKER  *********************************/

*acr = 0x10; /* CLEAR REN */
*dido = 0x04;/* CLEAR DEVICES ON BUS */ 
*acr = 0x80; /* SET SOFTWARE RESET (swrst)*/
*acr = 0x00; /* CLEAR SOFTWARE RESET */
*acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
taskDelay(1); /* SET DELAY */
*acr = 0x0f; /* CLEAR sic */
*acr = 0x93;
*ism0 =0x00; /* Clear Int mask 0 */
*ism1 =0x00; /* Clear Int Mask 1 */
*acr = 0x90;  /* SEND REMOTE ENABLE */
*acr = 0x0c;  /* TAKE CONTROL ASYNCHRONOUSLY */
              /* TO MAKE MZ7500 CONTROLER */
*dido = 0x24; /* SEND PPM LISTEN ADDRESS */
*adr = 0x40;  /* SEND TALK ADDRESS OF MZ7500 */
*acr = 0x8a;  /* SET MZ7500 AS TALKER */
*acr = 0x0b;  /* GO TO STANBY TO DEASSERT ATN */

/**************  SEND COMMAND STRINGS TO 8502A PPM  **********************/

do
  { 
      puts("ENTER A COMMAND STRING");
      scanf("%24s",xmt_array); /* GET COMMAND STRING FROM KEYBOARD */
      for (i=0; i<=24; i++) 
	{
	    *dido = xmt_array[i]; /* SEND COMMAND STRING TO PPM ONE */
	    taskDelay(1);         /* CHARACTER AT A TIME */
	}
      *dido = '\n'; /* END COMMAND STRING WITH A LINE FEED */
      getchar(); /* CLEAR BUFFER */ 

/**************  TURN MZ7500 INTO LISTENER AND COLLECT DATA  *************/

      puts("COLLECT DATA?");
      response = getchar(); /* CHECK TO SEE IF WE WANT TO COLLECT DATA */
      if (response== 'y')   /* IF SO, TURN BUS AROUND TO LISTEN */
	                    /* TAKE CONTROL BY ASSERTING ATN */
	{
	  *acr = 0x0c;	  
	  *acr = 0x0a; /* UNTALK MZ7500*/
	  *dido = 0x3f; /* UNLISTEN PPM */
	  *dido = 0x44; /* TALK ADDRESS PPM */
	  *adr = 0x20; /* LISTEN ADDRESS MZ7500 */
	  *acr = 0x89; /* SET MZ7500 TO LISTEN */
	  *acr = 0x0b; /* DEASSERT ATN */
	  printf("CHR= %02X\n",*chr);
	  printf("STATUS = %02X\n",*csr);
	  *niv = 0xfb;
	  *eiv = 0xfa;
	  *dcr = 0x20;
	  *ocr = 0x82;
	  *scr = 0x04;
	  *mtc = 0x000A;
	  *mar = (int)&rcv_array;
	  *mfc = 0x06;

/*************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/
	  do
	    {
		sscanf(rcv_array,"DBMA%f,DBMB%f",&rcv1,&rcv2); /* CONVERT TO FP */
		/*printf("%s\r",rcv_array);*/
		printf("CHANA = %7.2f dBm  CHANB = %7.2f dBm\n",rcv1,rcv2);
		return(rcv1);
	    }while (*dido != '\n');  
	  *chr=0x00;
	  *chr=0x10;
      } 
    getchar(); /* CLEAR BUFFER */
    *chr = 0x88;
    puts("Another Command?");
    response = getchar(); 
  }while (response == 'y'); /* CHECK TO SEE IF WE WANT TO CONTINUE OR ABORT */
}
