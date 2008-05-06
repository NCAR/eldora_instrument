 /*
 *	$id$
 *
 *	Module: big_little.c		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: These routines handle unpacking bytes differently for
 *              big endin and little endin machines. To use these
 *              routines you must declare a global variable of type int
 *              called processor_type and call the routine big_little
 *              (setting processor_type to its returned value) once before
 *              processing any data.  
 *        
 */

#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"


/* Stores whether the machine is big endin (=0) or little endin (=1) */ 

/* This routine returns the processor type */


int big_little(void)
{
  union
  {
    long along;
    char achar[4];
  }tst;
  int type;

  tst.along = 4096;

  if(tst.achar[2] == 16)
    {
      printf("*** I am a Big Endian machine\n");
      printf("\n");
      type = 0;
      return(type);
    }
  else
    {
      printf("*** I am a Little Endian machine\n");
      printf("\n");
      type = 1;
      return(type);
    }
  
}

/* This routine unpacks short data arrays, send it the starting address of
   the array (data), the number of shorts to unpack (num) the the type of
   data you are unpacking (type = 0 for big endin and 1 for little endin) */

void unpk_short(short *data, int num, int type)
{
  union{
      short ashort;
      char achar[2];
  }tmp;
  int i;
  char single_char;

  if(type == processor_type) return;

for(i=0; i<num; i++)
  {
    tmp.ashort = *data;
    single_char = tmp.achar[0];
    tmp.achar[0] = tmp.achar[1];
    tmp.achar[1] = single_char;
    *data++ = tmp.ashort;
  }
}


/* This routine unpacks long data arrays, send it the starting address of
   the array (data), the number of longs to unpack (num) the the type of
   data you are unpacking (type = 0 for big endin and 1 for little endin) */


void unpk_long(long *data, int num, int type)
{
  union{
      long along;
      char achar[4];
  }tmp;
  int i;
  char single_char;

  if(type == processor_type) return;

for(i=0; i<num; i++)
  {
    tmp.along = *data;
    single_char = tmp.achar[0];
    tmp.achar[0] = tmp.achar[3];
    tmp.achar[3] = single_char;
    single_char = tmp.achar[1];
    tmp.achar[1] = tmp.achar[2];
    tmp.achar[2] = single_char;
    *data++ = tmp.along;
  }
}


/* This routine unpacks float data arrays, send it the starting address of
   the array (data), the number of floats to unpack (num) the the type of
   data you are unpacking (type = 0 for big endin and 1 for little endin) */

void unpk_float(float *data, int num, int type)
{
  union{
      float afloat;
      char achar[4];
  }tmp;
  int i;
  char single_char;

  if(type == processor_type) return;

for(i=0; i<num; i++)
  {
    tmp.afloat = *data;
    single_char = tmp.achar[0];
    tmp.achar[0] = tmp.achar[3];
    tmp.achar[3] = single_char;
    single_char = tmp.achar[1];
    tmp.achar[1] = tmp.achar[2];
    tmp.achar[2] = single_char;
    *data++ = tmp.afloat;
  }
}









