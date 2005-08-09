/*
 *      $Id$
 *
 *      Module:          read_adc.c
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

#define OK_RPC
#define NO_MIN_SCOPE
#define TEST_BASE_ADDR 0x20006000

#include "stdio.h"
#include "taskLib.h"
#include "math.h"

void read_adc()
{
  int j;
  unsigned short rsin, rcos;
  unsigned short *sel_cos;
  unsigned short *sel_sin;
  volatile unsigned short *adc_busy;
  unsigned short *mincmd;
  double fsin,fcos,ftan,rdc;

  sel_cos = (unsigned short *)(TEST_BASE_ADDR + 0x22);
  sel_sin = (unsigned short *)(TEST_BASE_ADDR + 0x24);
  adc_busy = (volatile unsigned short *)(TEST_BASE_ADDR + 0x2A);
  mincmd = (unsigned short *)(0x20007020);
  /*  *mincmd = 0x90; */ /* rotate gimbal ccw */
  
for(;;)
  {
  /* start converters */

    *sel_cos = 0x0;
    *sel_sin = 0x0;

  /* delay a little */

    /*    printf("adc_busy = %8x\n",adc_busy); */
    taskDelay(1);
    while(((*adc_busy) & 0x0001) == 1)
      {
	j++;
      }
    rcos = *sel_cos & 0xfff;
    rsin = *sel_sin & 0xfff;
    fsin = (rsin - 2048.0)/2048.0;
    fcos = (rcos - 2048.0)/2048.0;
    ftan = fsin/fcos;
    rdc = atan(ftan);
#ifndef FULL_DIAGN
    printf("COS = %3X       SIN = %3X\r",rcos,rsin);
#endif
#ifdef FULL_DIAGN
    printf("COS = %3X       SIN = %3X\n",rcos,rsin);
    printf("FCOS = %8f      FSIN = %8f     FTAN = %8f\n",(float)fcos,(float)fsin,(float)ftan);
    printf("RDC = %8f\n",rdc);
#endif
    taskDelay(50);
  }
}
