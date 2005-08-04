/*
 *	$Id$
 *
 *	Module:		 ctor_dtor.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/02/11  18:38:05  thor
 * Initial revision
 *
 *
 * description:
 *        These two functions allow the use of global
 * constructors/destructors for C++. They walk the CTOR/DTOR lists,
 * invoking the functions listed. In order for these functions to work
 * one must first run collect over C++ object files and include the
 * generated file's object in the load module. 
 *
 * Note: Until I get a fix for collect, index values are adjusted by
 * 2, since collect puts 2 references to each function into the lists!
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"

struct ct_dt_list {
    int length;			/* How many? */
    int function[1];		/* Array of functions. */
};

typedef struct ct_dt_list CT_DT_LIST;

/* These are allocated in your collect code. */
extern CT_DT_LIST __CTOR_LIST__; 
extern CT_DT_LIST __DTOR_LIST__;

void do_global_cplusplus_init(void)
{
    FAST int i;
    FAST int len = __CTOR_LIST__.length;
    FAST void (**funcptr)() = (void (**)())__CTOR_LIST__.function;
    
    for (i = 0; i < len; i += 2) /* Increment by 2 due to collect */
				 /* doubling the table entries. */
      (*funcptr[i])();
}

void do_global_cplusplus_cleanup(void)
{
    FAST int len = __DTOR_LIST__.length;
    FAST void (**funcptr)() = (void (**)())__DTOR_LIST__.function;
    
    while ((len -= 2) >= 0) /* Same reason as above. */
      (*funcptr[len])();
}
