/*
 *      $Id$
 *
 *      Module:          testpulse_test.c
 *      Original Author: Eric Loew
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "taskLib.h"
#include "stdio.h"

int testpulse_test(int iterations)

{
  int i;
  void testpulseb(int,int,int,int,int);
  for (i=0; i< iterations; i++)
    {
      testpulseb(2000,1,0,15000,20000);
      taskDelay(5);
      testpulseb(2000,1,0,25000,5000);
      taskDelay(5);
      if(i % 100 == 0)
	printf("Completed %d iterations\r",i); 
    }
  printf("Testpulse Test Completed\n");
  return(-1);
}
