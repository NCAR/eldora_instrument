/***************************************************************************
****************************************************************************
*                                                                          *
*     NNNNNN         N                A             TTTTTTT TTTTTTTTTT     *
*     N NNNNN        N               AAA                  T TTTTT          *
*     NN NNNNN       N              AAAAA                 T TTTTT          *
*     N N NNNNN      N             A AAAAA                T TTTTT          *
*     N  N NNNNN     N            A A AAAAA               T TTTTT          *
*     N   N NNNNN    N           A   A AAAAA              T TTTTT          *
*     N    N NNNNN   N          A     A AAAAA             T TTTTT          *
*     N     N NNNNN  N         AAAAAAAAA AAAAA            T TTTTT          *
*     N      N NNNNN N        A         A AAAAA           T TTTTT          *
*     N       N NNNNNN       A           A AAAAA          T TTTTT          *
*     N        N NNNNN  OO  A             A AAAAA  OO     T TTTTT     OO   *
*                       OO                         OO                 OO   *
*                                                                          *
*     Gesellschaft fuer Netzwerk- und Automatisierungstechnologie m.b.H    *
*        Kamillenweg 22, D-53757 Sankt Augustin, Tel.: ++2241/3989-0       *
*                                                                          *
****************************************************************************
****************************************************************************
*
* Module      : neterr.h
*
* Description :	Error codes used by N.A.T. network protocol stacks
*
* Author      : H. Laufkoetter
*
****************************************************************************
****************************************************************************
*
*                    Copyright (c) by N.A.T. GmbH
*
*	All rights reserved. Copying, compilation, modification,
*	distribution or any other use whatsoever of this material
*	is strictly prohibited except in accordance with a Software
*	License Agreement with N.A.T. GmbH.
*
****************************************************************************
****************************************************************************
*
* Module's Description Record:
* ============================
*
* $ProjectName: //NATTDF/natsrc/tcpip/tcpip.pj $
* $ProjectRevision: null $
* $Source$
* $State$
* $Revision$ $Name$
* $Date$
* $Locker$
*
* Module's revision history:
* ==========================
*
* --------- $Log$
* --------- Revision 1.1  2000/05/15 10:15:18Z  nat
* --------- Initial revision
*
****************************************************************************
* 000406 ss Added NAT file header for RCS.
* 910603 hl Initial Version
****************************************************************************
*/

#ifndef __NETERR__
#define __NETERR__

#define NFSERR    0xa00		/* NFS: using 10:xxx numbers               */
#define DECERR	  0x900		/* using 9:xxx Numbers for OS9             */
#define L2ERR     0x800		/* LAYER 2 errors of all protocols         */

/***************** DECNet Error Codes **************************************/

#define E_UNEX     (DECERR+40)    /* DAP : unexpected message received     */
#define E_LICENSE  (DECERR+41)    /* decman: License number invalid        */

#define E_NOCOM    (DECERR+30)    /* no Communication                      */
#define E_NODE     (DECERR+29)    /* Node name not in HOSTS table          */
#define E_MODUL    (DECERR+28)    /* Module not found                      */
#define E_NORES    (DECERR+27)    /* no resources - memreq request failed  */
#define E_NOPORT   (DECERR+26)    /* No Port or in CLOSE state             */
#define E_NOBUFF   (DECERR+25)    /* No Receive Buffer available           */
#define E_REJECT   (DECERR+24)    /* Connect rejected                      */
#define E_NOTRUN   (DECERR+23)    /* Port not in running state             */
#define E_STATE    (DECERR+22)    /* Port in wrong state                   */
#define E_TIME     (DECERR+21)    /* Timeout                               */
#define E_LITTLE   (DECERR+20)    /* to little buffer space                */


/***************** NFS Error Codes *****************************************/
#define E_UNKNOWN  (NFSERR+1)	  /* unknown file or cannot recover fh     */
#define E_CORUPT   (NFSERR+2)     /* VDM database has been corrupted       */

/***************** TCP/IP Error Codes **************************************/

/*************** Common LAYER 2 Error Definitions **************************/

/*----- LAYER 2 Sublayer Kennung ------------------------------------------*/

#define L2LLC 	    1		/* Layer 2 Sublayer LLC                    */
#define L2MAC       2           /* Layer 2 Sublayer MAC                    */

/*-------------- Definition der LAYER 2 Fehlerkodes -----------------------*/

#define E_LMEM      (L2ERR+1)	/* Request for extra LAN-memory failed     */
#define E_MEM       (L2ERR+2)	/* Memory request failed        	   */
#define E_TXFULL    (L2ERR+3)	/* Transmit Ring full                      */
#define E_NOFRAME   (L2ERR+4)	/* got no frame                            */
#define E_LMERR     (L2ERR+5)	/* Lance Memory Error                      */
#define E_RXUFLO    (L2ERR+6)	/* Receiver Underflow                      */
#define E_FULL      (L2ERR+7)	/* list or structure is full	           */
#define E_NOTFND    (L2ERR+8)	/* Not found 			           */
#define E_TIMEL2    (L2ERR+9)	/* Timeout                                 */
#define E_FORK     (L2ERR+10)	/* cannot fork task                        */
#define E_NOMEM    (L2ERR+11)	/* no memory available                     */
#define E_UNSUP	   (L2ERR+12)   /* Unsupported Service Call                */
#define E_INUSE	   (L2ERR+13)   /* L2 in use by other protokoll            */
#define E_LINIT	   (L2ERR+14)	/* LANCE initialization failed             */

/*------ Definition der Layer 2 Error Message Codes -----------------------*/

#define IBL_ALLOC   1
#define TRING_ALLOC 2
#define RRING_ALLOC 3
#define AUX_TRING   4
#define AUX_RRING   5
#define TX_FULL     6
#define RX_NO_FRAME 7
#define RX_ERROR    8
#define TYPE_LIST   9
#define ATT_PORT    10		/* Error during attaching of a new port    */
#define I_CNTR      11          /* Error initializing the controller       */
#define RDISP       12          /* Error during forking pf RDisp process   */
#define FILL_TX	    13          /* Error during filling of TX-Ring         */
#define STOP        14
#define INIT_AGAIN  15
#define DESC_POOL   16		/* Error during allocation of descriptor mem*/

/*-------- ERR_RET - Macro ------------------------------------------------*/

#define ERR_RET(layer,msg,code) {show_err(layer,msg,code); return(code);}

#endif	/* __NETERR__ */

/* EOF */



