/*
 *	$Id$
 *
 *	Module:	init_iru	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module initializes the Inertial Reference Unit (IRU)
 *              interface and it pointers etc.  This interface is a ARINC429
 *              serial bus interface designed by VMIC part number VMIVME-6005.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern

#include "hskpAll.h"

void init_iru()
{
int timeout;

iru_pntr = (struct iru_rcontrol *)(IRU_BASE + STANDARD_BASE); 

/* Set up interface to not interrupt on command completion */
iru_pntr->resp_irq = 0;
iru_pntr->resp_vec = 0;

/* Initialize the IRU interface */
timeout = 0;
iru_pntr->response = 0;
iru_pntr->command = INITIALIZE;
do{
  taskDelay(3);
  timeout += 1;
}while(iru_pntr->response == 0 && timeout<100);
if(iru_pntr->response != COMMAND_ACK)
  printf("IRU board problem on INITIALIZE. Response: %2x Timeout: %3d\n",
	 iru_pntr->response,timeout);

/* Now disable all of the receivers */
iru_pntr->rx_mask = 0;
timeout = 0;
iru_pntr->response = 0;
iru_pntr->command = SET_RX_MASK;
do{
  taskDelay(1);
  timeout += 1;
}while(iru_pntr->response == 0 && timeout<100);
if(iru_pntr->response != COMMAND_ACK)
  printf("IRU board problem on SET_RX_MASK. Response: %2x Timeout: %3d\n",
	 iru_pntr->response,timeout);

/* Turn off interrupts on rxa0 channel (the only one we will enable) */
iru_pntr->rxa0_irq = 0;
iru_pntr->rxa0_irq = 0;

/* Tell the interface we are going to use sorting mode on labels only
   in channel rxa0 */

iru_pntr->rx_mode = 1;
iru_pntr->sort_type = 0x01;

/* send the set RX mode command */
timeout = 0;
iru_pntr->response = 0;
iru_pntr->command = SET_RX_MODE;
do{
  taskDelay(1);
  timeout += 1;
}while(iru_pntr->response == 0 && timeout<100);
if(iru_pntr->response != COMMAND_ACK)
  printf("IRU board problem on SET_RX_MODE. Response: %2x Timeout: %3d\n",
	 iru_pntr->response,timeout);

/* Now set the limit */
iru_pntr->rxa0_limit = 1200;  /* 4 times # of ARINC words */
iru_pntr->rxid = 0;  /* Set ID to RXA0 */
timeout = 0;
iru_pntr->response = 0;
iru_pntr->command = SET_RX_LIMIT;
do{
  taskDelay(1);
  timeout += 1;
}while(iru_pntr->response == 0 && timeout<100);
if(iru_pntr->response != COMMAND_ACK)
  printf("IRU board problem on SET_RX_LIMIT. Response: %2x Timeout: %3d\n",
	 iru_pntr->response,timeout);

/* Now enable the rx0a receiver */
iru_pntr->rx_mask = 0x01;
timeout = 0;
iru_pntr->response = 0;
iru_pntr->command = SET_RX_MASK;
do{
  taskDelay(1);
  timeout += 1;
}while(iru_pntr->response == 0 && timeout<100);
if(iru_pntr->response != COMMAND_ACK)
  printf("IRU board problem on SET_RX_MASK. Response: %2x Timeout: %3d\n",
	 iru_pntr->response,timeout);

}





