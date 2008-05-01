/*
 *	$Id$
 *
 *	Module:		 ecbMenu.c
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1992/09/24  18:20:32  shawn
 * Added "get temperature" options to if, rf, and atten menus.  Added
 * "show rpc global status" option to utility menu.
 *
 * Revision 1.2  1992/06/30  20:44:35  shawn
 * Added attenuator and Utility menu code.
 *
 * Revision 1.1  1992/06/29  19:52:47  shawn
 * Initial revision
 *
 * description: Upper level menu for manipulating ECB slaves from HSKP
 *              console.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/*****************************************************************************/
/*	ecbMenu()                                                            */
/*		Upper level menu for manipulating ECB slaves from HSKP       */
/*              console.                                                     */
/*      Return values:                                                       */
/*              None.  void function.                                        */
/*****************************************************************************/

#include <vxWorks.h>
#include <stdioLib.h>
#include "semLib.h"

#include "ecbFunc.h"     /* function prototypes for ecb______ */
#include "ecbStat.h"     /* global structures for ecb status */
#include "ecbAdr.h"      /* Slave addresses on ECB bus */
#include "ecbErrBound.h" /* various #defines for error bounds */

#define BSTATTIMEOUT  500000 /* get-bus-status timeout value */
#define TEMPITIMEOUT  300000 /* get-temp-integer timeout value */
#define TEMPRTIMEOUT  300000 /* get-temp-raw timeout value */
#define DDSOOLTIMEOUT 500000 /* get-DDS-out-of-lock timeout value */

/* function prototypes for functions internal to this file */
void clr();
void ifchoices();
void rfchoices();
void atchoices();
void mstrchoices();
void utilchoices();

void ecbMenu()
{
    int menu_choice=0;

    clr();

    while(1)
      {
	printf("\necbMenu - Upper level menu for manipulating ECB Slaves ");
	printf("from the HSKP console.\n\n");
	printf("1.)  LEAVE THIS MENU (return to vxWorks).\n");
	printf("2.)  IF processors.\n");
	printf("3.)  RCVR/XCTRs.\n");
	printf("4.)  Attenuator Chassis.\n");
	printf("5.)  Master Module.\n");
	printf("6.)  Utility stuff.\n");
	printf("\nEnter a choice 1-6 ==>");
	scanf("%d",&menu_choice);
	if (menu_choice == 1) break;
	switch(menu_choice)
	  {
	    case 2:
	      clr();
	      ifchoices();
	      clr();
	      break;
	    case 3:
	      clr();
	      rfchoices();
	      break;
	    case 4:
	      clr();
	      atchoices();
	      break;
	    case 5:
	      clr();
	      mstrchoices();
	      break;
	    case 6:
	      clr();
	      utilchoices();
	      break;
	    default:
	      printf("ecbMenu:  %d IS NOT A VALID MENU OPTION.  TRY AGAIN...\n",menu_choice);
	      break;
	  }
    }
}


void clr()
{
unsigned char lncount;
for(lncount=0; lncount < 20; lncount++) printf("\n");
}

void ifchoices()
{
    int menu_choice=0,icount;
    int foraft=0,unitnum=0,filtnum=0;
    
    while(1)
      {
	printf("\nifchoices - Menu for manipulating ECB Slaves in ");
	printf("IF Processors\n\n");
	printf("1.)  LEAVE THIS MENU (return to main menu).\n");
	printf("2.)  Set Filter in IF processor unit.\n");
	printf("3.)  Get Bus Status.\n");
	printf("4.)  Get Temperature (scaled integer).\n");
	printf("5.)  Get Temperature (raw counts).\n");
	printf("\nEnter a choice 1-3 ==>");
	scanf("%d",&menu_choice);
	if (menu_choice == 1) break;
	switch(menu_choice)
	  {
	    case 2:
	      printf("Set IF Filter\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      printf("   Enter IF processor unit number (1-3) ==>");
	      scanf("%d",&unitnum);
	      if(unitnum < ECBIFLOWUNIT || unitnum > ECBIFHIGHUNIT)
		{
		    printf("%d is an invalid IF processor ",unitnum);
		    printf("unit number\n");
		    break;
		}
	      printf("   Enter filter number (1-5) ==>");
	      scanf("%d",&filtnum);
	      if(filtnum < ECBIFLOWFILT || filtnum > ECBIFHIGHFILT)
		{
		    printf("%d is an invalid filter number\n",filtnum);
		    break;
		}
	      if(foraft)
		ecbSetIF((unsigned char) ECBIFFOR,(unsigned char) unitnum, (unsigned char) filtnum);
	      else
		ecbSetIF((unsigned char) ECBIFAFT,(unsigned char) unitnum, (unsigned char) filtnum);
	      break;
	    case 3:
	      printf("Get IF Processor slave Bus Status\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetBusStat((unsigned char) ECBIFFOR);
	      else
		ecbGetBusStat((unsigned char) ECBIFAFT);
	      for(icount=0; icount<BSTATTIMEOUT; icount++)
		if(ecbSlvStat.newdata) break;
	      if(!ecbSlvStat.newdata)
		printf("GET-BUS-STATUS FROM IF PROCESSOR HAS TIMED OUT.\n");
	      else
		ecbShowBusStat();
	      break;
	    case 4:
	      printf("Get IF Processor Temperature (scaled integer)\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetTsami((unsigned char) ECBIFFOR);
	      else
		ecbGetTsami((unsigned char) ECBIFAFT);
	      for(icount=0; icount<TEMPITIMEOUT; icount++)
		if(ecbTempi.newdata) break;
	      if(!ecbTempi.newdata)
		printf("GET-TEMP-INTEGER FROM IF PROCESSOR HAS TIMED OUT.\n");
	      else
		ecbShowTempi();
	      break;
	    case 5:
	      printf("Get IF Processor Temperature (raw counts)\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetTsamr((unsigned char) ECBIFFOR);
	      else
		ecbGetTsamr((unsigned char) ECBIFAFT);
	      for(icount=0; icount<TEMPRTIMEOUT; icount++)
		if(ecbTempr.newdata) break;
	      if(!ecbTempr.newdata)
		printf("GET-TEMP-RAW FROM IF PROCESSOR HAS TIMED OUT.\n");
	      else
		ecbShowTempr();
	      break;
	    default:
	      printf("ifchoices:  %d IS NOT A VALID MENU ",menu_choice);
	      printf("OPTION.   TRY AGAIN...\n");
	      break;
	  }
    }
}

void rfchoices()
{
    int menu_choice=0,icount;
    int foraft=0,unitnum=0,nrval=0;
    double freq=0.0;
    
    while(1)
      {
	printf("\nrfchoices - Menu for manipulating ECB Slaves in ");
	printf("RCVR/XCTRs\n\n");
	printf("1.)  LEAVE THIS MENU (return to main menu).\n");
	printf("2.)  Set Frequency in a DDS unit.\n");
	printf("3.)  Get Bus Status.\n");
	printf("4.)  Get Temperature (scaled integer).\n");
	printf("5.)  Get Temperature (raw counts).\n");
	printf("6.)  Get Lock Status.\n");
	printf("\nEnter a choice 1-6 ==>");
	scanf("%d",&menu_choice);
	if (menu_choice == 1) break;
	switch(menu_choice)
	  {
	    case 2:
	      printf("Set DDS Frequency\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      printf("   Enter DDS unit number (1-6) ==>");
	      scanf("%d",&unitnum);
	      if(unitnum < ECBDDSLOWUNIT || unitnum > ECBDDSHIGHUNIT)
		{
		    printf("%d is an invalid DDS unit number.\n",unitnum);
		    break;
		}
	      printf("   Enter frequency in GHz (9.3-9.8) ==>");
	      scanf("%lf",&freq);
	      freq *= 1000000000.0;
	      if(freq < ECBDDSLOWFREQ || freq > ECBDDSHIGHFREQ)
		{
		    printf("%lf is an invalid frequency\n",freq);
		    break;
		}
	      printf("   Enter 1 for noise reduction, 0 for no NR ==>");
	      scanf("%d",&nrval);
	      if(nrval && nrval!=1)
		{
		    printf("%d is an invalid NR value (0 or 1)\n",nrval);
		    break;
		}
	      if(foraft)
		ecbSetDDSfloat((unsigned char) ECBRFFOR,(unsigned char) unitnum, (double) freq,(unsigned char) nrval);
	      else
		ecbSetDDSfloat((unsigned char) ECBRFAFT,(unsigned char) unitnum, (double) freq,(unsigned char) nrval);
	      break;
	    case 3:
	      printf("Get RCVR/XCTR slave Bus Status\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetBusStat((unsigned char) ECBRFFOR);
	      else
		ecbGetBusStat((unsigned char) ECBRFAFT);
	      for(icount=0; icount<BSTATTIMEOUT; icount++)
		if(ecbSlvStat.newdata) break;
	      if(!ecbSlvStat.newdata)
		printf("GET-BUS-STATUS FROM RCVR/XCTR HAS TIMED OUT.\n");
	      else
		ecbShowBusStat();
	      break;
	    case 4:
	      printf("Get RCVR/XCTR Temperature (scaled integer)\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetTsami((unsigned char) ECBRFFOR);
	      else
		ecbGetTsami((unsigned char) ECBRFAFT);
	      for(icount=0; icount<TEMPITIMEOUT; icount++)
		if(ecbTempi.newdata) break;
	      if(!ecbTempi.newdata)
		printf("GET-TEMP-INTEGER FROM RCVR/XCTR HAS TIMED OUT.\n");
	      else
		ecbShowTempi();
	      break;
	    case 5:
	      printf("Get RCVR/XCTR Temperature (raw counts)\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetTsamr((unsigned char) ECBRFFOR);
	      else
		ecbGetTsamr((unsigned char) ECBRFAFT);
	      for(icount=0; icount<TEMPRTIMEOUT; icount++)
		if(ecbTempr.newdata) break;
	      if(!ecbTempr.newdata)
		printf("GET-TEMP-RAW FROM RCVR/XCTR HAS TIMED OUT.\n");
	      else
		ecbShowTempr();
	      break;
	    case 6:
	      printf("Get RCVR/XCTR Lock status\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(foraft && foraft!=1)
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      if(foraft)
		ecbGetDDSool((unsigned char) ECBRFFOR);
	      else
		ecbGetDDSool((unsigned char) ECBRFAFT);
	      for(icount=0; icount<DDSOOLTIMEOUT; icount++)
		if(ecbOol.newdata) break;
	      if(!ecbOol.newdata)
		printf("GET-DDS-OOL FROM RCVR/XCTR HAS TIMED OUT.\n");
	      else
		ecbShowDDSool();
	      break;
	    default:
	      printf("rfchoices:  %d IS NOT A VALID MENU ",menu_choice);
	      printf("OPTION.   TRY AGAIN...\n");
	      break;
	  }
    }
}

void atchoices()
{
    int menu_choice=0,icount;
    int foraft=0,xmittp=0,N=0,muxval=0,attenval=121;
    
    while(1)
      {
	printf("\natchoices - Menu for manipulating ECB Slave in ");
	printf("ATTENUATOR Chassis\n\n");
	printf("1.)  LEAVE THIS MENU (return to main menu).\n");
	printf("2.)  Set Attenuator Value.\n");
	printf("3.)  Get Bus Status.\n");
	printf("4.)  Get Temperature (scaled integer).\n");
	printf("5.)  Get Temperature (raw counts).\n");
	printf("6.)  Set Trigger Mux Value.\n");
	printf("7.)  Set Trigger Divide-by-N Value.\n");
	printf("\nEnter a choice 1-5 ==>");
	scanf("%d",&menu_choice);
	if (menu_choice == 1) break;
	switch(menu_choice)
	  {
	    case 2:
	      printf("Set Attenuator Value\n");
	      printf("   Enter 1 for forward, 0 for aft ==>");
	      scanf("%d",&foraft);
	      if(!foraft)
		printf("\nAFT ATTENUATOR SELECTED\n\n");
	      else if (foraft==1)
		printf("\nFORWARD ATTENUATOR SELECTED\n\n");
	      else
		{
		    printf("%d is an invalid forward/aft ",foraft);
		    printf("value (0 or 1)\n");
		    break;
		}
	      printf("   Enter Attenuator Value in dB (0-121) ==>");
	      scanf("%d",&attenval);
	      if(attenval < ECBATLOWATTEN || attenval > ECBATHIGHATTEN)
		{
		    printf("%d is an invalid attenuator value\n",attenval);
		    break;
		}
		ecbSetAtten((unsigned char) ECBATTEN,(unsigned char) foraft, (unsigned char) attenval);
	      break;
	    case 3:
	      printf("Get Attenuator slave Bus Status\n");
	      ecbGetBusStat((unsigned char) ECBATTEN);
	      for(icount=0; icount<BSTATTIMEOUT; icount++)
		if(ecbSlvStat.newdata) break;
	      if(!ecbSlvStat.newdata)
		printf("GET-BUS-STATUS FROM ATTENUATOR SLAVE HAS TIMED OUT.\n");
	      else
		ecbShowBusStat();
	      break;
	    case 4:
	      printf("Get Attenuator slave Temperature (scaled integer)\n");
	      ecbGetTsami((unsigned char) ECBATTEN);
	      for(icount=0; icount<TEMPITIMEOUT; icount++)
		if(ecbTempi.newdata) break;
	      if(!ecbTempi.newdata)
		printf("GET-TEMP-INTEGER FROM ATTENUATOR SLAVE HAS TIMED OUT.\n");
	      else
		ecbShowTempi();
	      break;
	    case 5:
	      printf("Get Attenuator slave Temperature (raw counts)\n");
	      ecbGetTsamr((unsigned char) ECBATTEN);
	      for(icount=0; icount<TEMPRTIMEOUT; icount++)
		if(ecbTempr.newdata) break;
	      if(!ecbTempr.newdata)
		printf("GET-TEMP-RAW FROM ATTENUATOR SLAVE HAS TIMED OUT.\n");
	      else
		ecbShowTempr();
	      break;
	    case 6:
	      printf("Set Trigger Mux Value\n");
	      printf("   Enter 1 for transmit , 0 for testpulse ==>");
	      scanf("%d",&xmittp);
	      if(!xmittp)
		printf("\nTESTPULSE MUX SELECTED\n\n");
	      else if (xmittp==1)
		printf("\nTRANSMIT MUX SELECTED\n\n");
	      else
		{
		    printf("%d is an invalid xmit/testpulse ",xmittp);
		    printf("value (0 or 1)\n");
		    break;
		}
	      printf("Mux Choices:\n");
	      printf("   0.) Forward frequency 'chip' #1\n");
	      printf("   1.) Forward frequency 'chip' #2\n");
	      printf("   2.) Forward frequency 'chip' #3\n");
	      printf("   3.) Forward frequency 'chip' #4\n");
	      printf("   4.) Forward frequency 'chip' #5\n");
	      printf("   5.) Forward 'preknock'\n");
	      printf("   6.) Forward 'testpulse'\n");
	      printf("   8.)  Aft frequency 'chip' #1\n");
	      printf("   9.)  Aft frequency 'chip' #2\n");
	      printf("   10.) Aft frequency 'chip' #3\n");
	      printf("   11.) Aft frequency 'chip' #4\n");
	      printf("   12.) Aft frequency 'chip' #5\n");
	      printf("   13.) Aft 'preknock'\n");
	      printf("   14.) Aft 'testpulse'\n");
	      printf("\nEnter a choice 0-6 || 8-14 ==>");
	      scanf("%d",&muxval);
	      if(muxval<ECBATLOWMUX||muxval>ECBATHIGHMUX||muxval==ECBATBADMUX)
		{
		    printf("%d is an invalid trigger Mux value\n",muxval);
		    break;
		}
		ecbSetMux((unsigned char) ECBATTEN,(unsigned char) xmittp, (unsigned char) muxval);
	      break;
	    case 7:
	      printf("Set Divide-by-N Value\n");
	      printf("   Enter 1 for transmit , 0 for testpulse ==>");
	      scanf("%d",&xmittp);
	      if(!xmittp)
		printf("\nTESTPULSE DIVIDE-BY-N COUNTER SELECTED\n\n");
	      else if (xmittp==1)
		printf("\nTRANSMIT DIVIDE-BY-N SELECTED\n\n");
	      else
		{
		    printf("%d is an invalid xmit/testpulse ",xmittp);
		    printf("value (0 or 1)\n");
		    break;
		}
	      printf("   Enter Divide-by-N value (2-255) ==>");
	      scanf("%d",&N);
	      if(N < ECBATLOWN || N > ECBATHIGHN)
		{
		    printf("%d is an invalid N value\n",N);
		    break;
		}
		ecbSetDivN((unsigned char) ECBATTEN,(unsigned char) xmittp, (unsigned char) N);
	      break;
	    default:
	      printf("atchoices:  %d IS NOT A VALID MENU ",menu_choice);
	      printf("OPTION.   TRY AGAIN...\n");
	      break;
	  }
    }
}

void mstrchoices()
{
printf("mstrchoices: There are no master-related functions implemented at\n");
printf("mstrchoices: this level yet.\n");
}

void utilchoices()
{
    int menu_choice=0;
    while(1)
      {
	printf("\nutilchoices - Menu for ECB utilities\n\n");
	printf("1.)  LEAVE THIS MENU (return to main menu).\n");
	printf("2.)  Show ID of last command successfully executed.\n");
	printf("3.)  Show contents of global ecbSlvStat structure.\n");
	printf("4.)  Show contents of global ecbTempi structure.\n");
	printf("5.)  Show contents of global ecbTempr structure.\n");
	printf("6.)  Show contents of ecb Master VME Status buffer.\n");
	printf("7.)  Show contents of rpc global currStatus structure.\n");
	printf("\nEnter a choice 1-7 ==>");
	scanf("%d",&menu_choice);
	if (menu_choice == 1) break;
	switch(menu_choice)
	  {
	    case 2:
	      ecbShowLast();
	      break;
	    case 3:
	      ecbShowBusStat();
	      break;
	    case 4:
	      ecbShowTempi();
	      break;
	    case 5:
	      ecbShowTempr();
	      break;
	    case 6:
	      ecbShowMstat();
	      break;
	    case 7:
	      ecbShowRpc();
	      break;
	    default:
	      printf("utilchoices:  %d IS NOT A VALID MENU ",menu_choice);
	      printf("OPTION.   TRY AGAIN...\n");
	      break;
	  }
    }
}
