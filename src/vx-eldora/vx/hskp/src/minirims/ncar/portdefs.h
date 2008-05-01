/* portdefs.h - definitions for portability
   THESE ARE FOR SANDAC V ONLY               */

#ifndef PORTDEFS_H
#define PORTDEFS_H

typedef unsigned short ushort;      /* 16 bit unsigned integer    */
typedef unsigned char utiny;        /* to get unsigned byte       */
typedef unsigned int uint;          /* 32 bit unsigned integer    */

typedef char tbits;                 /* one byte, for bitwise uses */
typedef char tbool;                 /* one byte: {0:1}            */
typedef ushort bits;                /* 16 bits for bitwise uses   */
typedef int bool;                   /* for function returns: {0:1}*/

#define MICROTIME    (*(unsigned long *)0xe0000l)
#define FOREVER      for(;;)

typedef enum { INIT_SUCC,
               INIT_EXEC,
               INIT_REQ,
               INIT_ERR} INIT_STAT;

#endif


