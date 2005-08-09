/*
 *	$Id$
 *
 *	Module:		 getInt.c
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope 
#include "getInt.h"
#include "tp41vAdr.h"
#include "vmevmeAdr.h"

#define VME_VME_VEC    255    /* vme-to-vme interface interrupt vector */
#define VMEMEM_BASE         0xC00000

void triggerVMEInts(int hbox);

/* These are for debugging only. */
void checkInt();
void clrInt();

static volatile unsigned char *PSAddr = 
(unsigned char *)(SHORT_BASE + VMEVME_SHORT_START + LOCAL_COMMAND_REG);

static volatile unsigned char *StatAddr = 
(unsigned char *)(SHORT_BASE + VMEVME_SHORT_START + LOCAL_STATUS_REG);

static volatile short *hndShk = (short *)(VMEMEM_BASE + STANDARD_BASE + 0xffffd);

static void getVMEIsr(int junk);

void setupVMEIsr()
{
  *PSAddr = 0x40;		/* Be paranoid, clear PS interrupt. */

  intConnect((VOIDFUNCPTR *)(VME_VME_VEC * 4),(VOIDFUNCPTR)getVMEIsr,0);

  VMEQue = msgQCreate(100,2,0);
}

void checkInt()
{
  printf("0x%x 0x%x\n",(unsigned int)StatAddr,(unsigned int)*StatAddr);
}

void clrInt()
{
  unsigned char c = *StatAddr;

  unsigned char outByte = 0x40;

  if (c & 2)                    /* Check to see if a PM interrupt is active */
    outByte |= 0x20;            /* If yes, don't turn it off. */

  *PSAddr = outByte;            /* Clear PS interrupt. */
}

static void getVMEIsr(int junk)
{
  unsigned char c = *StatAddr;
  
  unsigned char outByte = 0x40;

  if (msgQSend(VMEQue,(char *)hndShk,2,NO_WAIT,MSG_PRI_NORMAL) != OK)
    logMsg("Dropped a beam because queue is full.\n",0,0,0,0,0,0);

  if (c & 2)			/* Check to see if a PM interrupt is active */
    outByte |= 0x20;		/* If yes, don't turn it off. */

  *PSAddr = outByte;		/* Clear PS interrupt. */
}
