/*
 *	$Id$
 *
 *	Module:		 string.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/09/25  20:02:56  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INClstringh
#define INClstringh

#include "string.h"

#ifndef __STDC__
char *strchr(), *strrchr(), *strpbrk(), *strtok(), *strdup();
int strspn(), strcspn();
int atoi();
long atol();
#else
char *strchr(char *, int);
char *strrchr(char *, int);
char *strpbrk(char *, char *);
char *strtok(char *, char *);
char *strdup(char *);
int strspn(char *, char *);
int nstrcspn(char *, char *);
int atoi(char *);
long atol(char *);
#endif

#endif /* INClstringh */
