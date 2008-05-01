/*
 *	$Id$
 *
 *	Module:	ecbFunc.h	 
 *	Original Author: Shawn B. Copeland
 *      Copyright National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1993/09/20  17:05:12  shawn
 * *** empty log message ***
 *
 * Revision 1.1  1992/06/24  20:45:29  shawn
 * Initial revision
 *
 * description: Contains ecb function prototypes for HSKP processor
 *        
 */

/* Interrupt related functions, residing in ecbIntStuff.o */
void ecbCmpltIsr();
void ecbErrorIsr();
unsigned char ecbIntInit(unsigned int bimtries);

/* ecbSet___ functions, residing in ecbSets.o */
unsigned char ecbSetAtten(unsigned char ecbadr, unsigned char foraft, unsigned char attenval);
unsigned char ecbSetDDS(unsigned char ecbadr, unsigned char unitnum, unsigned char B, unsigned long T, unsigned char nr);
unsigned char ecbSetDDSfloat(unsigned char ecbadr, unsigned char unitnum, double desfreq, unsigned char nr);
unsigned char ecbSetDivN(unsigned char ecbadr, unsigned char xmittp, unsigned char N);
unsigned char ecbSetIF(unsigned char ecbadr, unsigned char unitnum, unsigned char filtnum);
unsigned char ecbSetMux(unsigned char ecbadr, unsigned char xmittp, unsigned char muxval);
unsigned char ecbSetIFBad(unsigned char ecbadr, unsigned char unitnum, unsigned char filtnum);

/* ecbSet___ functions, residing in ecbSets.o */
unsigned char ecbGetTsami(unsigned char ecbadr);
unsigned char ecbGetTsamr(unsigned char ecbadr);
unsigned char ecbGetBusStat(unsigned char ecbadr);
unsigned char ecbGetDDSool(unsigned char ecbadr); 

/* ecbShow___ functions, residing in ecbShows.o */
void ecbShowBim();
void ecbShowBusStat();
void ecbShowLast();
void ecbShowMstat();
void ecbShowTempi();
void ecbShowTempr();
void ecbShowDDSool();

/* Utility functions, residing in ecbUtils.o */
void ecbClrFifos();
void ecbGiveSem();
void ecbMenu();
