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
 *
 * description: The purpose of this module is to set up the MZ7500 GPIB
 *              board as controller with DMA and interrupts in order to
 *              send commands to and receive data from the two WAVETEK 8502A
 *              Peak Power Meters used for fore and aft peak power 
 *              measurements.  The program also converts the string data from 
 *              the 8502A to binary and writes it to tape.
 */

#define scope
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#include </ELDORA/vx/hskp/include/gbl.h>
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
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

/********************  FUNCTION PROTOTYPES *************************/

float pwr(unsigned char);

/******************** MAIN PROGRAM ****************************/

float pwr(unsigned char channel)
{
  volatile unsigned char *ism0,*acr,*adr,*dido;
  unsigned char xmt_array[25], rcv_array[25];
  unsigned char *csr,*dcr,*ocr,*scr,*chr,*niv,*eiv,*mtc,*mar,*mfc;
  int i;
  unsigned char response;
  float rcv1,rcv2;

/***************  GPIB REGISTER POINTERS  *********************************/

  ism0 = (unsigned char *)(GPIBCHAN2 + GPIBOFF0);
  acr = (unsigned char *)(GPIBCHAN2 + GPIBOFF3);
  adr = (unsigned char *)(GPIBCHAN2 + GPIBOFF4);
  dido = (unsigned char *)(GPIBCHAN2 + GPIBOFF7);


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

/*************** SET UP MZ7500 AS TALKER  *********************************/
*acr = 0x10; /* CLEAR REN */
*dido = 0x04;/* CLEAR DEVICES ON BUS */ 
*acr = 0x80; /* SET SOFTWARE RESET (swrst)*/
*acr = 0x00; /* CLEAR SOFTWARE RESET */
*acr = 0x8f; /* SEND INTERFACE CLEAR (sic)*/
taskDelay(1); /* SET DELAY */
*acr = 0x0f; /* CLEAR sic */
*acr = 0x93;
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
	  do 
	    {          /* DO UNTIL LINE FEED CHARACTER */
		for(i=0; i<23; i++) 
		  {
		      while((*ism0 & 0x20) != 0x20);
		      rcv_array[i] = *dido; /* BUILD STRING ARRAY */
		  }
		rcv_array[23] = '\0'; /* ADD NULL TO END OFF STRING */

/*************  CONVERT DATA TO FLOATING POINT AND DISPLAY  *************/

		sscanf(rcv_array,"DBMA%f,DBMB%f",&rcv1,&rcv2); /* CONVERT TO FP */
		/*printf("%s\r",rcv_array);*/
		printf("CHANA = %7.2f dBm  CHANB = %7.2f dBm\r",rcv1,rcv2);
		return(rcv1);
	    }while (*dido != '\n');  
      } 
    getchar(); /* CLEAR BUFFER */
    puts("Another Command?");
    response = getchar(); 
  }while (response == 'y'); /* CHECK TO SEE IF WE WANT TO CONTINUE OR ABORT */
}








































































