/*****************************************************************************
******************************************************************************
*                                                                            *
*     NNNNNN         N                A             TTTTTTT TTTTTTTTTT       *
*     N NNNNN        N               AAA                  T TTTTT            *
*     NN NNNNN       N              AAAAA                 T TTTTT            *
*     N N NNNNN      N             A AAAAA                T TTTTT            *
*     N  N NNNNN     N            A A AAAAA               T TTTTT            *
*     N   N NNNNN    N           A   A AAAAA              T TTTTT            *
*     N    N NNNNN   N          A     A AAAAA             T TTTTT            *
*     N     N NNNNN  N         AAAAAAAAA AAAAA            T TTTTT            *
*     N      N NNNNN N        A         A AAAAA           T TTTTT            *
*     N       N NNNNNN       A           A AAAAA          T TTTTT            *
*     N        N NNNNN  OO  A             A AAAAA  OO     T TTTTT     OO     *
*                       OO                         OO                 OO     *
*                                                                            *
*     Gesellschaft fuer Netzwerk- und Automatisierungstechnologie m.b.H      *
*        Kamillenweg 22, D-53757 Sankt Augustin, Tel.: ++2241/3989-0         *
*                                                                            *
******************************************************************************
******************************************************************************
*
* Module      : fifo.h
*
* Description : fifo management
*
* Author      : Christof Girgenrath
*
******************************************************************************
******************************************************************************
*
*                    Copyright (c) by N.A.T. GmbH
*
*       All rights reserved. Copying, compilation, modification,
*       distribution or any other use whatsoever of this material
*       is strictly prohibited except in accordance with a Software
*       License Agreement with N.A.T. GmbH.
*
******************************************************************************
******************************************************************************
*
* Module's Description Record:
* ============================
*
* $ProjectRevision$
* $Source$
* $State$
* $Revision$ $Name$
* $Date$
* $Locker$
*
* Module's revision history:
* ==========================
*
* --------- $Log$
* 981120 cg initial version;
*
*******************************************************************************
******************************************************************************/

/*-----------------------------------------------------------------------------
GENERAL NOTES

The fifo macros manage a memory pool by forming a linked list of memory
blocks. 

The size of a fifo is given by the number of elements the linked list
contains.

To put an memory block to the fifo following steps are performed:

 1. new memory block to be put to fifo:
    _______  _______  _______   ..____  _______
    |     |  |     |  |     |        |  |     |
    ----  -> ----  -> ----  -> ..    -> ----  -> undefined   -----
    |  |     |  |     |  |              |  |                 |   |
    ----     ----     ----              ----                 -----

    ^                                   ^                    ^
    |                                   |                    |
    fifo.out                            fifo.in              m_block

 2. first four byte of fifo.in set to address m_block points to:
    _______  _______  _______   ..____  _______
    |     |  |     |  |     |        |  |     |
    ----  -> ----  -> ----  -> ..    -> ----  -> -----
    |  |     |  |     |  |              |  |     |   |
    ----     ----     ----              ----     -----

    ^                                   ^        ^
    |                                   |        |
    fifo.out                            fifo.in  m_block

 3. new fifo.in position is m_block:
    _______  _______  _______   ..____  _______
    |     |  |     |  |     |        |  |     |
    ----  -> ----  -> ----  -> ..    -> ----  -> -----
    |  |     |  |     |  |              |  |     |   |
    ----     ----     ----              ----     -----

    ^                                            ^
    |                                            |
    fifo.out                                     fifo.in = m_block

To get a memory block from the fifo following steps have to be performed:
 1. set pointer of m_block to fifo.out
 2. set fifo.out to next memory block = address wich is contained in the
    first fout bytes of m_block

Be sure to call INIT_FIFO if you use local fifos because of possibly 
non-zero initialization of local variables.

-----------------------------------------------------------------------------*/



#ifndef FIFO_H
#define FIFO_H



/*****************************************************************************/
/*	INCLUDES							     */
/*****************************************************************************/

#include <stdio.h>



/*****************************************************************************/
/*       PUBLIC MACROS                                                       */
/*****************************************************************************/

#define INIT_FIFO(fifo) \
    (fifo).in = NULL; \
    (fifo).out = NULL; \
    (fifo).size = 0



#define PUT_FIFO(fifo,m_block) \
    if((fifo).in) { \
        *(void**)(fifo).in = (void*)m_block; \
        (fifo).in = (void*)m_block; \
    } \
    else { \
        (fifo).in = (fifo).out = (void*)m_block; \
    } \
    (fifo).size++



#define GET_FIFO(fifo,m_block) \
    if((fifo).out) { \
        if((fifo).out != (fifo).in) { \
            (void*)m_block = (fifo).out; \
            (fifo).out = *(void**)m_block; \
        } \
        else { \
            (void*)m_block = (fifo).out; \
            (fifo).out = (fifo).in = NULL; \
        } \
        (fifo).size--; \
    } \
    else { \
        m_block = NULL; \
    }



#define OVERFLOW_FIFO(fifo,max) \
    (((fifo).size > ((unsigned long)(max))) ? 1 : 0)



#define UNDERFLOW_FIFO(fifo) \
    (((fifo).size <= 0) ? 1 : 0)



#define FIFO_SIZE(fifo) \
    (fifo).size



#define FIFO_IN(fifo) \
    (fifo).in



#define FIFO_OUT(fifo) \
    (fifo).out



#define SHOW_FIFO(fifo) \
    printf("fifo input:  0x%08lx\n", (fifo).in); \
    printf("fifo output: 0x%08lx\n", (fifo).out); \
    printf("fifo size  : %ld\n", (fifo).size)



/*****************************************************************************/
/*       PUBLIC DATA TYPES                                                   */
/*****************************************************************************/

typedef struct {
    void *in;                                    /* pointer to fifo input */
    void *out;                                 /* pointer to fifo  output */
    long size;                                 /* number of fifo elements */
} FIFO;



/*****************************************************************************/
/*       PUBLIC FUNCTION PROTOTYPES                                          */
/*****************************************************************************/



/*****************************************************************************/
/*       PUBLIC GLOBALS                                                      */
/*****************************************************************************/



#endif  /* FIFO_H */



