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
void start_ieee(float,float);
void xmit_norm_isr(void);
void xmit_err_isr(void);
void testp_norm_isr(void);
void testp_err_isr(void);
void flt_pt(void);
void init_ptrs(void);
void init_isr_vecs(void);
void send_cmnd_string(char,char *);
void init_gpib(void);
void init_dma(void);
void listen(void);

#endif /* INC */








