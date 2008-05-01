/*
 *	$Id$
 *
 *	Module: COLL_DIAG		 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:   $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * Initial revision
 *
 *
 *
 * description:
 *
 *  This module programs the Collator with diagnostic flags necessary
 *  to use the output of the DAC's for diagnostic purposes.
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define OK_RPC
#define NO_RP7_SCOPE
#define NO_RDP_SCOPE

#include "vxWorks.h"
#include "stdioLib.h"
#include "ctype.h"
#include "semLib.h"
#include "RadarGbls.h"
#ifndef NU_ARCHIT
#include "ELDRP7.h"
#endif
#ifdef NU_ARCHIT
#include "NU_ELDRP7.h"
#endif
#include "coll_dpramdefs.h"

#define COL_DIAG_FREQ 0x124
#define COL_DIAG_FLAG 0x126

void coll_diag(void) 

{
    unsigned long  bd;
    unsigned char *pio, scr, *temp;
    int nm, sem_status;
    unsigned int val, i, status, cont;
    cont = 1;
    bd = COL0BASE;
    sysIntDisable(1);   /* disable Collator out of sync interrupt */
    while(cont)
      {
	  printf("COLLATOR DIAGNOSTIC MENU \n");
	  printf("TYPE 1 to Select Frequency \n");
	  printf("TYPE 2 to Select Desired Output \n");
	  printf("TYPE 3 to Select Desired Gate \n");
	  printf("TYPE 4 to Exit Program \n");
          taskDelay(100);	/* Allow other tasks time to run */
	  scanf(" %d",&nm);
	  switch(nm)
	    {
	      case 1:
		printf("1. Enter Frequency Number (1-5) \n");
		scanf(" %d",&nm);
		switch(nm)
		  {
		    case 1:
		      val = 1;
		      break;
		    case 2:
		      val = 2;
		      break;
		    case 3:
		      val = 3;
		      break;
		    case 4:
		      val = 4;
		      break;
		    case 5:
		      val = 5;
		      break;
		    default:
		      val = 1;
		      printf("Invalid Entry Defaulting to Frequency 1 \n");
		      break;
		  }
		printf("val = %d\n",val);

		pio = (unsigned char *)(bd +  DSPSEL);
		*(pio + PCRL) = 0x1F;           /* set up DMA and AUTO */
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		*(pio + PARE) = (0xff0000 & COL_DIAG_FREQ) >> 16;
		*(pio + PARL) = 0xff & COL_DIAG_FREQ;
		*(pio + PARH) = (0x00ff00 & COL_DIAG_FREQ) >> 8;
		*(pio + PDRL) = val & 0xFF;
		*(pio + PDRH) = (val >> 8) & 0xFF;
		
		/* check that correct values were written */
    
		*(pio + PARE) = (0xff0000 & COL_DIAG_FREQ) >> 16;
		*(pio + PARL) = 0xff & COL_DIAG_FREQ;
		*(pio + PARH) = (0x00ff00 & COL_DIAG_FREQ) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		if(status != val)
		  printf("PIO R/W ERROR in Collator, Processor 0 \n");
		break;
	      case 2:
		printf("2. Enter Desired Output: \n");
		printf("                FFT == 1 \n");
		printf("    I,Q Time Series == 2 \n");
		printf("         v,p Ascope == 3 \n");
		scanf(" %d", &nm);
		switch(nm)
		  {
		    case 1:
		      val = 0;
		      break;
		    case 2:
		      val = 1;
		      break;
		    case 3:
		      val = 2;
		      break;
		    default:
		      val = 1;
		      printf("Invalid Entry Defaulting to FFT \n");
		      break;
		  }
		printf("val = %d\n",val);
		pio = (unsigned char *)(bd +  DSPSEL);
		*(pio + PCRL) = 0x1F;           /* set up DMA and AUTO */
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		*(pio + PARE) = (0xff0000 & COL_DIAG_FLAG) >> 16;
		*(pio + PARL) = 0xff & COL_DIAG_FLAG;
		*(pio + PARH) = (0x00ff00 & COL_DIAG_FLAG) >> 8;
		*(pio + PDRL) = val & 0xFF;
		*(pio + PDRH) = (val >> 8) & 0xFF;
    
		/* check that correct values were written */
		
		*(pio + PARE) = (0xff0000 & COL_DIAG_FLAG) >> 16;
		*(pio + PARL) = 0xff & COL_DIAG_FLAG;
		*(pio + PARH) = (0x00ff00 & COL_DIAG_FLAG) >> 8;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		if(status != val)
		  printf("PIO R/W ERROR in Collator, Processor 0 \n");
		break;
	      case 3:
		printf("Enter Desired Gate for Diagnostics \n");
		scanf(" %d", &nm);
		val = (float)(nm)/4.0 + 0.5;
		pio = (unsigned char *)(bd +  DSPSEL);
		*(pio + PCRL) = 0x1F;           /* set up DMA and AUTO */
		if(*(pio + PCRL) != 0)scr = *(pio + PDRH);
		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x6C;
		*(pio + PARH) = 0x0;
		*(pio + PDRL) = val & 0xFF;
		*(pio + PDRH) = (val >> 8) & 0xFF;
    
		/* check that correct values were written */
		
		*(pio + PARE) = 0x0;
		*(pio + PARL) = 0x6C;
		*(pio + PARH) = 0x0;
		temp = (unsigned char *)(pio + PDRL);
		status = *temp & 0xff;
		temp = (unsigned char *)(pio + PDRH);
		status += ((*temp & 0xff)<< 8);
		if(status != val)
		  printf("PIO R/W ERROR in Collator, Processor 0 \n");
		break;
	      case 4:
		cont = 0;
		break;
	      default:
		printf("Invalid Selection Please Make Another \n");
		break;
	    }
      }
    sysIntEnable(1);    /* re-enable Collator out of sync interrupt */
    
      /* Clear Collator Out of Sync Interrupt that may occur as a result of 
         the memory writes to the Collator 
      */

    sem_status = semTake(bim_int3_sem, NO_WAIT); /* don't wait ISR to pass sem */



}



