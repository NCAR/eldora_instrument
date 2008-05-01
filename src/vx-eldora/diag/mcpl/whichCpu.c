/*
 *	$Id$
 *
 *	Module:		 whichCpu.c
 *	Original Author: Richard E. Neitzel
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

#include "whichCpu.h"

static char *names[] = {
    "Tadpole Technology TP41V",
    "Motorola MVME-133A",
    0
};

int whichCpu(void)
{
    FAST char *model = sysModel();

    FAST char **ptr = names;

    FAST int type = 0;

    for (; *ptr; type++, ptr++)
      {
          if (!strcmp(model,*ptr))
            break;
      }
    return(type);
}
