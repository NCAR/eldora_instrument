/*
 *	$Id$
 *
 *	Module:	pwrDef.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/09/01  20:07:18  craig
 * *** empty log message ***
 *
 * Revision 1.2  1992/09/01  17:25:20  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:38:48  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */

#include "semLib.h"
#ifndef INCpwrDefh
#define INCpwrDefh
#ifndef scope
#define scope extern
#endif /* scope */

/*****************  SET BASE ADDRESS OF THE MZ7500 GPIB BOARD  ************/

#define GPIBCHAN1 0x20006000 /* BASE ADDRESS OF MZ7500 */
#define GPIBCHAN2 0x20006040
#define DMACHAN0 0x20006100
#define DMACHAN1 0x20006140
#define DMACHAN2 0x20006180
#define DMACHAN3 0x200061C0

/*****************  SET UP MZ7500 GPIB REGISTER OFFSETS  ******************/

#define INT_STAT_REG0 0x01 /* INTERRUPT STATUS REG/MASK 0 */
#define INT_STAT_REG1 0X03 /* INTERRUPT STATUS REG/MASK 1 */
#define ADDR_STAT_REG 0X05 /* ADDRESS STATUS REGISTER */
#define AUX_CMND_REG 0x07 /* BUS STATUS REG/AUX CMND REG */
#define ADDRESS_REG 0x09 /* ADDRESS REGISTER */
#define SER_POLL_REG 0X0B /* SERIAL POLL REGISTER */
#define PAR_POLL_REG 0X0D /* CMD PASS THRU/PARALLEL POLL REGISTER */
#define DATA_IN_OUT_REG 0x0F /* DATA IN/DATA OUT REGISTER */

/*****************  SET UP MZ7500 DMA REGISTER OFFSETS  *******************/

#define CHAN_STAT_REG  0X00 /* CHANNEL STATUS REGISTER */
#define CHAN_ERR_REG  0x01 /* CHANNEL ERROR REGISTER */
#define DEV_CNTRL_REG  0X04 /* DEVICE CONTROL REGISTER */
#define OP_CNTRL_REG 0X05 /* OPERATION CONTROL REGISTER */
#define SEQ_CNTRL_REG 0X06 /* SEQUENCE CONTROL REGISTER */
#define CHAN_CNTRL_REG 0X07 /* CHANNEL CONTROL REGISTER */
#define MEM_XFER_CNTR 0X0A /* MEMORY TRANSFER COUNTER */
#define MEM_ADDR_REG1 0X0C /* MEMORY ADDRESS REGISTER1 */
#define MEM_ADDR_REG2 0X0E /* MEMORY ADDRESS REGISTER2 */
#define DEV_ADDR_REG 0X14 /* DEVICE ADDRESS REGISTER */
#define BASE_XFER_CNTR 0X1A /* BASE TRANSFER COUNTER */
#define BASE_ADDR_REG1 0X1C /* BASE ADDRESS REGISTER */
#define BASE_ADDR_REG2 0X1E /* BASE ADDRESS REGISTER */
#define NORM_INT_VECT 0X25 /* NORMAL INTERRUPT VECTOR REGISTER */
#define ERR_INT_VECT 0X27 /* ERROR INTERRUPT VECTOR REGISTER  */
#define CHAN_PRTY_REG 0X2D /* CHANNEL PRIORITY REGISTER */
#define MEM_FUNC_CODE 0X29 /* MEMORY FUNCTION CODE REGISTER */
#define DEV_FUNC_CODE 0X31 /* DEVICE FUNCTION CODE REGISTER */
#define BASE_FUNC_CODE 0X39 /* BASE FUNCTION CODE REGISTER */
#define GEN_CNTRL_REG 0XFF /* GENERAL  CONTROL REGISTER */

/****************  DEFINE INTERRUPT VECTORS *********************/

#define XMIT_NORM_INT 240  /* XMIT NORMAL INTERRUPT VECTOR */
#define XMIT_ERR_INT 239  /* XMIT ERROR INTERRUPT VECTOR  */
#define TESTP_NORM_INT 238  /* TESTP NORMAL INTERRUPT VECTOR */
#define TESTP_ERR_INT 237  /* TESTP ERROR INTERRUPT VECTOR */

/*************** POWER METER OFFSETS ******************/

#define XMIT_OFFSET_FORE 65.00
#define XMIT_OFFSET_AFT 65.00
#define TESTP_OFFSET_FORE 65.00
#define TESTP_OFFSET_AFT 65.00

/****************** MISC DEFINES ****************************/

#define XMIT 1
#define TESTP 2
#define XMIT_AND_TESTP 3

/**************** POWER METER COMMAND STRINGS ***************/

#define RATIO_A_CW_TO_B_CW "ACBC\0"
#define RATIO_A_CW_TO_B_PK "ACBP\0"
#define RATIO_A_PK_TO_B_CW "APBC\0"
#define RATIO_A_PK_TO_B_PK "APBP\0"
#define AUTOSCALE_DELAY "ASDL\0"
#define AVERAGE_AUTOSCALE "AVAS\0"
#define AVERAGE_CW "AVCW\0"
#define AVERAGE_PK "AVPK\0"
#define CALIBRATE_A "CALA\0"
#define CALIBRATE_B "CALB\0"
#define CURSOR_DELAY_A "CDLA\0"
#define CURSOR_DELAY_B "CDLB\0"
#define READ_CURSOR_DELAY_A "CDOA\0"
#define READ_CURSOR_DELAY_B "CDOB\0"
#define CW_DUAL_CHANNEL "CWAB\0"
#define CW_CHANNEL_A "CWPA\0"
#define CW_CHANNEL_B "CWPB\0"
#define DATA_FAST "DATF\0"
#define DATA_NORMAL "DATN\0"
#define DISPLAY_IN_DBM "DBMW\0"
#define CAL_FACTOR_A "DCPA\0"
#define CAL_FACTOR_B "DCPB\0"
#define OFFSET_A "OFFA\0"
#define OFFSET_B "OFFB\0"
#define PEAK_DUAL_CHANNEL "PKAB\0"
#define PEAK_CHANNEL_A "PKPA\0"
#define PEAK_CHANNEL_B "PKPB\0"
#define REFERENCE_DELAY_A "RDLA\0"
#define REFERENCE_DELAY_B "RDLB\0"
#define START_DELAY_A "SDLA\0"
#define START_DELAY_B "SDLB\0"
#define DISABLE_SRQ "SRQD\0"
#define ENABLE_SRQ "SRQE\0"
#define EXTERNAL_TRIGGER "TRGE\0"
#define INTERNAL_TRIGGER_A "TRGA\0"
#define INTERNAL_TRIGGER_B "TRGB\0"
#define UPDATE_CONTINUOUSLY "UPDC\0"
#define DISPLAY_IN_WATTS "WATT\0"
#define AUTOZERO_A "ZERA\0"
#define AUTOZERO_B "ZERB\0"

/*************** GLOBAL VARIABLES ******************/

scope SEM_ID xmit_pwr_sem;
scope SEM_ID xmit_data_sem;
scope SEM_ID xmit_err_sem;
scope SEM_ID testp_data_sem;
scope SEM_ID testp_err_sem;
scope volatile int isr_count,freq_count,fore_aft,tp_cnt,New_tp;
scope int pgm,xmit,testp, Avg, Debug;
scope float fore_xmit_pwr,aft_xmit_pwr;
scope float fore_testp_pwr,aft_testp_pwr;
scope float xmit_csr_delay,testp_csr_delay;
scope char xmit_isr_done;
scope char testp_isr_done,dma_err1,dma_err2;
scope volatile unsigned char *xmit_array;
scope volatile unsigned char *testp_array;
scope long ieee_xmit_cnt,ieee_testp_cnt;

/**************** GLOBAL GPIB POINTERS ******************/

scope volatile unsigned char *g1ism0,*g1ism1,*g1acr,*g1adr,*g1dido;
scope volatile unsigned char *g2ism0,*g2ism1,*g2acr,*g2adr,*g2dido;

/********************* GLOBAL DMA POINTERS ***********************/

scope volatile unsigned char *d0csr,*d0dcr,*d0ocr,*d0scr,*d0ccr,*d0niv,*d0eiv;
scope volatile unsigned char *d1csr,*d1dcr,*d1ocr,*d1scr,*d1ccr,*d1niv,*d1eiv;
scope volatile unsigned char *d0bfc,*d0mfc,*d0cer;
scope volatile unsigned char *d1bfc,*d1mfc,*d1cer;
scope volatile unsigned short *d0bar1,*d0bar2;
scope volatile unsigned short *d1bar1,*d1bar2;
scope volatile unsigned short *d0mtc,*d0btc;
scope volatile unsigned short *d1mtc,*d1btc;
scope volatile unsigned short *d0mar1,*d0mar2;
scope volatile unsigned short *d1mar1,*d1mar2;
scope volatile unsigned char *d0cpr;
scope volatile unsigned char *d1cpr;
scope volatile unsigned char *gcr;

#endif /* INC */



























 
