/*
  Module Name:   adinta
  Purpose:       Produce a ramp out of the a/d card onto HSDB A
  Arguments:     none
  Author:        Shawn B. Copeland
  Date Written:  21 FEB 91
  Revised:       Added delay
  */

#include <vxWorks.h>
#include <stdioLib.h>
#include "/ELDORA/vx/mhen/include/ELDRP7.h"

adinta()
     
{
  unsigned long *qvaltoa;
  unsigned short *modesel,*dsy;
  unsigned char rampval,i;
  
  qvaltoa = (unsigned long *)(ATOD0BASE + ATOD_QHSDB);
  dsy = (unsigned short *)(ATOD0BASE + ATOD_DC);
  modesel = (unsigned short *)(ATOD0BASE + ATOD_M);
  
  printf("qvaltoa, dsy, modesel===> %8x,%8x,%8x\n",qvaltoa,dsy,modesel);

  *dsy = 0;
  *modesel = 0x81;
  rampval = 0;
  i = 0;

  while(1)
    {
      *qvaltoa = (unsigned long) rampval++;
      i++;i--;i++;i--;
    }

}




