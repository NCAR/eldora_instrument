/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        This code is used to pass commands to the QPDMs on the AGC.
 * We don't use interrupts, since most commands a small enough and
 * executed fast enough to fit into the fifo.
 *
 */
#include "GraphicController.hh"

#include "taskLib.hh"

void GraphicController::qpdmCmd(FAST long *cmd, FAST int number)
{
    volatile FAST long *fifo = qpdmFifo;
    volatile FAST unsigned char *fifoState = fifoFlags;
    FAST SEM_ID semaphore = accessSem;
    FAST unsigned char test = FIFO_HF | FIFO_AE;
    FAST unsigned char hf = FIFO_HF;

    semTake(semaphore);

    while (1)
      {
	  FAST int maxWrites;
	  FAST int blksize = 64; // This is the size of the do loop &
				 // also the smallest amount of
				 // assured free space in the fifo.
	  
	  FAST unsigned char state = *fifoState;

	  state &= test;

	  if (state)
	    {
		if (number <= blksize) // Less then 64 longs.
		  {
		      switch (number)
			{
			  case 64: *fifo = *cmd++;
			    case 63: *fifo = *cmd++;
			    case 62: *fifo = *cmd++;
			    case 61: *fifo = *cmd++;
			    case 60: *fifo = *cmd++;
			    case 59: *fifo = *cmd++;
			    case 58: *fifo = *cmd++;
			    case 57: *fifo = *cmd++;
			    case 56: *fifo = *cmd++;
			    case 55: *fifo = *cmd++;
			    case 54: *fifo = *cmd++;
			    case 53: *fifo = *cmd++;
			    case 52: *fifo = *cmd++;
			    case 51: *fifo = *cmd++;
			    case 50: *fifo = *cmd++;
			    case 49: *fifo = *cmd++;
			    case 48: *fifo = *cmd++;
			    case 47: *fifo = *cmd++;
			    case 46: *fifo = *cmd++;
			    case 45: *fifo = *cmd++;
			    case 44: *fifo = *cmd++;
			    case 43: *fifo = *cmd++;
			    case 42: *fifo = *cmd++;
			    case 41: *fifo = *cmd++;
			    case 40: *fifo = *cmd++;
			    case 39: *fifo = *cmd++;
			    case 38: *fifo = *cmd++;
			    case 37: *fifo = *cmd++;
			    case 36: *fifo = *cmd++;
			    case 35: *fifo = *cmd++;
			    case 34: *fifo = *cmd++;
			    case 33: *fifo = *cmd++;
			    case 32: *fifo = *cmd++;
			    case 31: *fifo = *cmd++;
			    case 30: *fifo = *cmd++;
			    case 29: *fifo = *cmd++;
			    case 28: *fifo = *cmd++;
			    case 27: *fifo = *cmd++;
			    case 26: *fifo = *cmd++;
			    case 25: *fifo = *cmd++;
			    case 24: *fifo = *cmd++;
			    case 23: *fifo = *cmd++;
			    case 22: *fifo = *cmd++;
			    case 21: *fifo = *cmd++;
			    case 20: *fifo = *cmd++;
			    case 19: *fifo = *cmd++;
			    case 18: *fifo = *cmd++;
			    case 17: *fifo = *cmd++;
			    case 16: *fifo = *cmd++;
			    case 15: *fifo = *cmd++;
			    case 14: *fifo = *cmd++;
			    case 13: *fifo = *cmd++;
			    case 12: *fifo = *cmd++;
			    case 11: *fifo = *cmd++;
			    case 10: *fifo = *cmd++;
			    case 9: *fifo = *cmd++;
			    case 8: *fifo = *cmd++;
			    case 7: *fifo = *cmd++;
			    case 6: *fifo = *cmd++;
			    case 5: *fifo = *cmd++;
			    case 4: *fifo = *cmd++;
			    case 3: *fifo = *cmd++;
			    case 2: *fifo = *cmd++;
			    case 1: *fifo = *cmd;
			    default: break;
				}
		      
		      semGive(semaphore);
		      return;
		  }

		if (state == hf)
		  {
		      // Do 488 longs!
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;

		      number -= 448;
		  }
 		else if (state == test)
		  {
		      // Do 256 longs!
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;

		      number -= 256;
		  }
		else
		  {
		      // Write 64 longs out!
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      *fifo = *cmd++;
		      
		      number -= blksize;
		  }
	    }
      }
    semGive(semaphore);
}
