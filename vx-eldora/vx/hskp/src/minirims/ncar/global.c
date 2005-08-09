/* global variables for processor e and processor f */
#include "portdefs.h"
#define DATASEG 5

 short int crcev,crcea;
 unsigned long int _evtae;     /* exception table address entry for proc e */
 unsigned long int _epce;      /* exception PC (last one seen) */
 ushort _evne;                 /* exception vector number (last one seen) */
 ushort _tece;                 /* total exception count */
 ushort vrtx_erre,res_counte;
 short int crcfv,crcfa;
 unsigned long int _evtaf;     /* exception table address entry for proc f */
 unsigned long int _epcf;      /* exception PC (last one seen) */
 ushort _evnf;                 /* exception vector number (last one seen) */
 ushort _tecf;                 /* total exception count */
 ushort vrtx_errf,res_countf;
 long int sens_count;
 utiny eris_tm_buf[482];
