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
 * Revision 1.1  1992/05/29  17:55:11  shawn
 * Initial revision
 *
 * description: Contains ecb function prototypes for HSKP processor
 *        
 */

/* Interrupt related functions, residing in ecbIntStuff.o */
void ecbCmpltIsr();
void ecbErrorIsr();
void ecbIntInit();

/* ecbSet___ functions, residing in ecbSets.o */
unsigned char ecbSetAtten(unsigned char ecbadr, unsigned char foraft, unsigned char attenval);
unsigned char ecbSetDDS(unsigned char ecbadr, unsigned char unitnum, unsigned char B, unsigned long T);
unsigned char ecbSetDDSfloat(unsigned char ecbadr, unsigned char unitnum, double desfreq);
unsigned char ecbSetDivN(unsigned char ecbadr, unsigned char xmittp, unsigned char N);
unsigned char ecbSetIF(unsigned char ecbadr, unsigned char unitnum, unsigned char filtnum);
unsigned char ecbSetMux(unsigned char ecbadr, unsigned char xmittp, unsigned char muxval);

/* ecbSet___ functions, residing in ecbSets.o */
unsigned char ecbGetTsami(unsigned char ecbadr);
unsigned char ecbGetTsamr(unsigned char ecbadr);
unsigned char ecbGetBusStat(unsigned char ecbadr);
unsigned char ecbGetDDSool(unsigned char ecbadr); 

/* ecbShow___ functions, residing in ecbShows.o */
void ecbShowBim();
void ecbShowLast();
void ecbShowMstat();
void ecbShowTempi();
void ecbShowTempr();

/* Utility functions, residing in ecbUtils.o */
void ecbClrFifos();
void ecbGiveSem();
