/*+ DEC/CMS REPLACEMENT HISTORY, Element VRTXVISI.H+*/
/*+ *1    12-JUL-1988 08:43:03 JRFELLE "vrtx definitions"+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element VRTXVISI.H+*/
/* VRTX header file */

#define VRTX       0x00 /* VRTX trap number */

/* VRTX task-level function codes */

#define SCFTCREATE   0x0000 /* task create */
#define SCFTDELETE   0x0001 /* task delete */
#define SCFTSUSPEND  0x0002 /* task suspend */
#define SCFTRESUME   0x0003 /* task resume */
#define SCFTPRIORITY 0x0004 /* task priority change */
#define SCFTINQUIRY  0x0005 /* task status inquiry */
#define SCFGBLOCK    0x0006 /* get memory block */
#define SCFRBLOCK    0x0007 /* release memory block */
#define SCFPOST      0x0008 /* post message to mailbox */
#define SCFPEND      0x0009 /* pend for message from mailbox */
#define SCFGTIME     0x000A /* get time */
#define SCFSTIME     0x000B /* set time */
#define SCFTDELAY    0x000C /* task delay */
#define SCFGETC      0x000D /* get character */
#define SCFPUTC      0x000E /* put character */
#define SCFWAITC     0x000F /* wait for special character */
#define SCFTSLICE    0x0015 /* enable round-robin scheduling */
#define SCFLOCK      0x0020 /* disable task rescheduling */
#define SCFUNLOCK    0x0021 /* enable task rescheduling */
#define SCFPCREATE   0x0022 /* create memory partition */
#define SCFPEXTEND   0x0023 /* extend memory partition */
#define SCFACCEPT    0x0025 /* accept message from mailbox */
#define SCFQPOST     0x0026 /* post message to queue */
#define SCFQPEND     0x0027 /* pend for message from queue */
#define SCFQACCEPT   0x0028 /* accept message from queue */
#define SCFQCREATE   0x0029 /* create message queue */
#define SCFQINQUIRY  0x002A /* queue status inquiry */

/* VRTX interrupt-level function codes */

#define UIFEXIT    0x0011 /* exit interrupt handler */
#define UIFTIMER   0x0012 /* announce timer interrupt */
#define UIFRXCHR   0x0013 /* received-character interrupt */
#define UIFTXRDY   0x0014 /* transmit-ready interrupt */
#define UIFENTER   0x0016 /* enter interrupt handler */

/* VRTX initialization function codes */

#define VRTXFINIT  0x0030 /* initialize VRTX */
#define VRTXFGO    0x0031 /* start application execution */

/* System-wide error codes */

#ifndef RET_OK
#define RET_OK     0x0000 /* successful return */
#endif

/* VRTX error codes */

#ifndef ER_TID
#define ER_TID     0x0001 /* task ID error */
#define ER_TCB     0x0002 /* no TCBs available */
#define ER_MEM     0x0003 /* no memory available */
#define ER_NMB     0x0004 /* not a memory block */
#define ER_MIU     0x0005 /* mailbox in use */
#define ER_ZMW     0x0006 /* zero message */
#define ER_BUF     0x0007 /* buffer full */
#define ER_WTC     0x0008 /* WAITC in progress */
#define ER_ISC     0x0009 /* invalid system call */
#define ER_TMO     0x000A /* timeout */
#define ER_NMP     0x000B /* no message present */
#define ER_QID     0x000C /* queue ID error */
#define ER_QFL     0x000D /* queue full */
#define ER_PID     0x000E /* partition ID error */
#define ER_INI     0x000F /* fatal initialization error */
#define ER_NCP     0x0010 /* no character present */
#define ER_ICP     0x0011 /* invalid config parameter during INIT */
#define ER_IIP     0x0012 /* invalid input parameter, PCREATE/PEXTEND */
#define ER_CVT     0x0020 /* component vector table not present */
#define ER_COM     0x0021 /* undefined component */
#define ER_OPC     0x0022 /* undefined opcode for component */
#endif

/* TBSTAT task status flags */

#define TBSSUSP    0x0001 /* explicitly suspended */
#define TBSMBOX    0x0002 /* suspended for mailbox message */
#define TBSGETC    0x0004 /* suspended for character input */
#define TBSPUTC    0x0008 /* suspended for character output */
#define TBSWAITC   0x0010 /* suspended for special character */
#define TBSDELAY   0x0020 /* suspended for task delay/timeout */
#define TBSQUEUE   0x0040 /* suspended for queue message */
