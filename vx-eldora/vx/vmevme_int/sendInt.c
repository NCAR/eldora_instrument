/*
 *	$Id$
 *
 *	Module:		 sendInt.c
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

#include "sendInt.h"

void triggerVMEInts(int hbox);

/* These two routines are for debug use only */
void checkInt();
void setInt(unsigned char c);

static volatile unsigned char *forePSAddr = 
(unsigned char *)(SHORT_BASE + FORE_SHORT_START + REMOTE_COMMAND_REG);

static volatile unsigned char *aftPSAddr =
(unsigned char *)(SHORT_BASE + AFT_SHORT_START + REMOTE_COMMAND_REG);

static short *foreHndShk =
(short *)(STANDARD_BASE + FORE_STAND_START + 0xffffd);

static short *aftHndShk = 
(short *)(STANDARD_BASE + AFT_STAND_START + 0xffffd);

void triggerVMEInts(int hbox)
{
  // 1st check to see if the other side got the last interrupt.
  // Note that reading this address gives us the status, but writting it is
  // for commands.

  unsigned char status; // = *forePSAddr;

  status &= 0x20;		/* Check the PS bit. If it is set,
			           The other side didn't get the last 
			           interrupt. */

  if (status != 0)
    printf("Fore failed to service last handshake interrupt.\n");
  else
    {
      *foreHndShk = (short)hbox;
      *forePSAddr = 0x20;
    }

  status = *aftPSAddr;

  status &= 0x20;		/* Check the PS bit. If it is set,
				   The other side didn't get the last 
				   interrupt. */

  if (status != 0)
    printf("Aft failed to service last handshake interrupt.\n");
  else
    {
      *aftHndShk = (short)hbox;
      *aftPSAddr = 0x20;
    }
}

void checkInt()
{
  printf("fore = 0x%x 0x%x\n",(unsigned int)forePSAddr,
	 (unsigned int)*forePSAddr);
  printf("aft = 0x%x 0x%x\n",(unsigned int)aftPSAddr,(unsigned int)*aftPSAddr);
}

void setInt(unsigned char c)
{
  *forePSAddr =c;
  *aftPSAddr =c;
}
