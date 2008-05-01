//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          rawtst.cc
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "Raw.hh"
#include <string.h>
#include <stdlib.h>
#include <tickLib.h>
#include <math.h>
#include <taskLib.h>

extern "C"
{
  void rawtest();
};

struct {
  DataBeam db;
  RAW_DATA rd;
  float fd[500];
} testdata;

DispCommand cmd;

Header hdr;


static float d1[500];
static float d2[500];

void rawtest()
{
  memset((void *)&hdr,0,sizeof(Header));

  strcpy(testdata.rd.x_label,"label 1");
  strcpy(testdata.rd.y_label,"label 2");

  testdata.rd.xmax = 500.0;
  testdata.rd.xmin = 0.0;
  testdata.rd.ymax = 10.0;
  testdata.rd.ymin = 0.0;
  testdata.rd.numPoints = 500;

  FAST int j = 500;

  srand(tickGet());
  FAST float *fptr = d1;
  
  for (FAST int i = 0; i < j; i++)
    {
      double tmp = sin((double)(i) * M_PI / 180.0);
      if (tmp < 0.0)
        tmp = 0.0;
      
      *fptr++ = (float)tmp;
    }

  fptr = d2;
  for (FAST int i = 0; i < j; i++)
    {
      double tmp = cos((double)(i) * M_PI / 180.0);
      if (tmp < 0.0)
        tmp = 0.0;
      *fptr++ = (float)tmp;
    }

  GraphicController gc((void *)0x80000000,1280,1024,2048,250);
  Raw raw(&gc);

  Clock clk(&gc,3,0,0,1872,1024);

  raw.setClock(clk);

  cmd.cmd = FORWARD_RAW;

  raw.reset(&hdr,&cmd);

  j = 100;

  raw.setcolor(9);

  strncpy(&testdata.rd.raw_des[0],"RAWD",4);
  
  for (FAST int i = 0; i < j; i++)
    {
      if (i & 1)
        memcpy((void *)&testdata.fd[0],(void *)d1,sizeof(d1));
      else
        memcpy((void *)&testdata.fd[0],(void *)d2,sizeof(d2));
      
      raw.drawBeam(&testdata.db);
    }
}
