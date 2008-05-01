/*
 *	$Id$
 *
 *	Module:		 AuxClk.h.h
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
#ifndef INCAuxClk_h
#define INCAuxClk_h

typedef void *AuxDogPtr;

AuxDogPtr AuxDogCreate(FUNCPTR func, int arg, int rate);

int AuxDogSetTmo(AuxDogPtr ptr, double tmo);

int AuxDogStart(AuxDogPtr ptr);

int AuxDogCancel(AuxDogPtr ptr);

void AuxDogRestart(AuxDogPtr ptr);

void AuxDogDestroy(AuxDogPtr ptr);

#endif /* INCAuxClk_h */
