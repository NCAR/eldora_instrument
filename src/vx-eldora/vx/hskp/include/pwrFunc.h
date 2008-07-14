/*
 *	$Id$
 *
 *	Module:	pwrFunc.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1999/09/27  15:49:07  thor
 * Fix prototypes.
 *
 * Revision 1.2  1993/09/01 20:07:20  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  21:39:01  reif
 * Initial revision
 *
 *
 * description: The purpose of this module is to 
 */

#ifndef INCpwrFunch
#define INCpwrFunch

/********************  FUNCTION PROTOTYPES *************************/

void pwr(void);
int start_ieee(void);	/* returns 1 if power meter stuff started successfully, 0 otherwise */
void xmit_norm_isr(void);
void xmit_err_isr(void);
void testp_norm_isr(void);
void testp_err_isr(void);
void flt_pt(int);
void init_ptrs(void);
void init_isr_vecs(void);
int send_cmnd_string(char,char *);
void init_gpib(int);
void init_dma(int,int);
void listener(int);
void talk(int);
unsigned char ReadStatusByte(int);
unsigned char * ReceiveData(int,int);
void Return_to_Local(int);
void xmit_rt(void);
void testp_rt(void);

#endif /* INC */








