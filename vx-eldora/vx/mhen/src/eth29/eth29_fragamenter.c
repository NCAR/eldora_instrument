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
* Module      : eth29.c
*
* Description : test routines for ETH29 on MVME2306 host system
*               running under vxWorks
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
* --------- Revision 1.9  2003/04/15 17:12:51  martinc
* --------- adding udp packet fragmentation, based on scheme used for piraq systems
* ---------
* --------- Revision 1.8  2003/04/12 19:41:02  martinc
* --------- call ioctl to set the interface broadcast address during initialization. Not really sure that this is required, since I found that if I read back the broadcast address without setting it, the lower octect is set to 255 anyway.
* ---------
* --------- Revision 1.7  2003/04/10 22:45:57  ericloew
* --------- changed intLock etc. to sysIntDisable etc., and changed test program ip addrs to the 192 network
* ---------
* --------- Revision 1.6  2003/04/09 18:56:51  martinc
* --------- bug fixes, testEth29 now working
* ---------
* --------- Revision 1.5  2003/04/09 18:28:15  martinc
* --------- adding eldora specific functions to initialize the eth29 and send datagrams on a socket that is kept open
* ---------
* --------- Revision 1.4  2003/04/09 16:25:56  martinc
* --------- New versions based on updated source code from NAT, sent on
* --------- 9-april-03. The changes made by NAT include:
* ---------
* --------- 1. Task is suspended on a semaphore, while waiting
* --------- for response from NAT card, and then the semaphore
* --------- is given in the interruot routine.
* ---------
* --------- 2. A fifo has been added to queue commands in
* --------- the interrupt service routine.
* ---------
* --------- 3. A datagram send test routine has been added (eth29_tx).
* ---------
* --------- 4. Additional socket interface calls have been added, which
* --------- were missing from the initial code (P_Send(), P_Sendto(), etc.)
* ---------
* --------- The code checked in now is slightly modified from what NAT sent:
* --------- 1. The intConnect() vector parameter had to be changed to use the INT_VEC_NUM()
* --------- macro, which may be an artifact of vxWroks 5.2
* ---------
* --------- 2. The IP addresses are specified with a char string, and translated
* --------- using inet_addr().
* ---------
* 030408 cg added routine eth29_dbg to switch the debug level on the
*           ETH29;
*           renamed eth29_init into eth29_init_l2;
* 030404 cg adapted to be compilable using special VxWorks mechanisms
*           that requires inclusion of VxWorks header files (VXWORKS
*           must be defined in the makefile);
*           according to these adaptions now VxWorks semaphores are
*           used to avoid polling and taskDelay that reduced speed;
*           removed unused routines eth29_poll_rx and eth29_check_data;
* 011025 cg added routine eth29_check_data to check data sent by two
*           different remote systems;
* 011023 cg in the two copying while-loops in eth29_poll_rx removed check
*           on sb_cc to copy the whole mbuf-list into the host system, sb_cc
*           now is only used to check if there is a valid datagram containing
*           the UDP header and the user data;
* 011022 cg in eth29_poll_rx removed saving the next mbuf chain given
*           by the mbuf's m_act element, this is to copy only one UDP
*           packet per polling loop;
*           additionally socket buffer's character counter is checked to
*           be greater than 16 bytes (= size of UDP header) to avoid copying
*           only the UDP header in one loop and the data in the next loop;
*           removed warning if copied_len != sb_cc because this may occur
*           since we copy only one UDP packet per loop;
* 011017 cg in struct socket changed type of so_proto from 'struct protosw *'
*           into 'void *' because this element is unused but may cause
*           compiler errors;
*           in eth29_poll_rx changed order of setting SB_CPY and 'continue'
*           if no mbuf is indicated because SB_CPY will not be reset if
*           loop is continued and no P_DropData is issued;
*           in eth29_poll_rx replaced error message by warning in case
*           copied_len != sb_cc because in this case we are in the time
*           window where the mbuf chain is linked to the socket buffer,
*           the left bytes are then copied in the next loop, therefore
*           it is important to return the number of copied bytes indicated
*           by 'copied_len' in P_DropData, this effect only occurs on
*           very high interrupt load (tested: 36 byte buffer every 1ms
*           => 1 of 1000000 transfers);
* 011009 cg initial version;
*
******************************************************************************
******************************************************************************/

/*-----------------------------------------------------------------------------
GENERAL NOTES


-----------------------------------------------------------------------------*/



/*****************************************************************************/
/*	COMMON CONFIGURATION						     */
/*****************************************************************************/

/*
 * test application's version
 */
#define ETH29_APP_VER	"$Revision$"



/*
 * application configuration
 */
/* enable receive */
//#define ETH29_RX

/* receive statistics support */
#define ETH29_RX_STAT_SUPP

/* check compile time configuration */
#if defined ETH29_RX_STAT_SUPP && !defined VXWORKS
#error ETH29_RX_STAT_SUPP only valid for VXWORKS
#endif

#if defined ETH29_RX_STAT_SUPP && !defined ETH29_RX
#warning ETH29_RX_STAT_SUPP only valid for ETH29_RX
#undef ETH29_RX_STAT_SUPP
#endif



/*****************************************************************************/
/*	INCLUDES							     */
/*****************************************************************************/

#ifdef VXWORKS
#include <stdio.h>
#include <ioLib.h>						    /* ioctl */
#include <intLib.h>			       /* generic interrupt routines */
#include <iv.h>					/* intConnect, intEnable ... */
#include <semLib.h>					/* semaphore library */
#include <taskLib.h>					     /* task library */
#include <string.h>					   /* memcpy, memset */
#include "sysLib.h"				 /* system dependent library */
#include "tickLib.h"					     /* tick library */
#include "netinet/in.h"
#endif	/* VXWORKS */

#include "eth29.h"
#include "mbuf.h"
#include "nif.h"
#include "socket.h"
#include "neterr.h"
#include "fifo.h"
#include "piraq_udp.h"


/*****************************************************************************/
/*	PRIVATE MACROS							     */
/*****************************************************************************/

/*
 * debug
 */
//#define ETH29_DBG				   /* host application debug */
//#define ETH29_BUF_DBG					     /* buffer debug */
//#define ETH29_INT_DBG						/* isr debug */
//#define ETH29_FIRMWARE_DBG			 /* switch on firmware debug */

#ifdef ETH29_DBG
#define PRINTD(fmt,arg...)	printd(fmt, ## arg)
#else
#define PRINTD(fmt,arg...)
#endif

#ifdef ETH29_BUF_DBG
#define PRINTB(buf,len)		printb(buf,len)
#else
#define PRINTB(buf,len)
#endif

#ifndef printd
#define printd	printf
#endif



/*
 * ETH29 configuration
 */
#define LOOPBACK_IP_ADDR        "127.0.0.1"
#define ETH29_IP_ADDR           "128.117.80.81"
#define SEND_TO_IP_ADDR         "128.117.80.170"



/*
 * ETH29 access
 */
#define ETH29_VME_BASE		0x90000000		 /* VME base address */

#define ETH29_MPRAM_BASE	ETH29_VME_BASE	       /* MPRAM base address */
#define ETH29_MPRAM_SIZE	0x00400000		       /* MPRAM size */
#define ETH29_MPRAM_MASK	(ETH29_MPRAM_SIZE - 1)	       /* MPRAM mask */
#define ETH29_RESET_CELL	((ETH29_VME_BASE)+0xffffc)   /* ETH29 reset if written */
#define ETH29_MBOX_CELL		((ETH29_VME_BASE)+0xfffe0)   /* mail box */
#define ETH29_NIF_ADDR		((ETH29_VME_BASE) + 0x4000)   /* NIF address */

#define ETH29_INT_VEC		0xb0			 /* interrupt vector */
#define ETH29_INT_LEV		3			  /* interrupt level */

#define ETH29_PID		1		   /* for compatibility only */
#define ETH29_MAX_SOCK		8	 /* max. number of supported sockets */



/*
 * NIF related
 */
#define NIF_MAX_WAIT		400		      /* NIF polling timeout */



/*
 * address conversion over VME bus
 */
#define ADDR_S2H(a) \
	(void *)(a) = (void *)(((u_long)(a) & ETH29_MPRAM_MASK) + ETH29_VME_BASE)

#define ADDR_H2S(a) \
	(void *)(a) = (void *)((u_long)(a) & ETH29_MPRAM_MASK)



/*
 * vxWorks routines
 */
#define INTLOCK(ik)	sysIntDisable(eth29InterruptLevel)
#define INTUNLOCK(ik)	sysIntEnable(eth29InterruptLevel)
//#define INTLOCK(ik)	((int)(ik) = intLock())
//#define INTUNLOCK(ik)	intUnlock((int)(ik))

#define SEMBCREATE(sid,state)	sid = semBCreate(SEM_Q_FIFO,state)
#define SEMCCREATE(sid,count)	sid = semCCreate(SEM_Q_FIFO,count)
#define SEMCREATE(sid)		sid = semBCreate(SEM_Q_FIFO,SEM_EMPTY)
#define SEMTAKE(sid)		semTake(sid,WAIT_FOREVER)
#define SEMGIVE(sid)		semGive(sid)
#define SEMDELETE(sid)		semDelete(sid)

#define TSPAWN(name,prio,stack_size,entry_func) \
	taskSpawn(name,prio,0,(int)stack_size,(FUNCPTR)entry_func, \
		  0,0,0,0,0,0,0,0,0,0)
#define TSLEEP			taskDelay



/*
 * receive statistics related
 */
/* rx rate update time (seconds) */
#define ETH29_RX_RATE_UPDATE_TIME	2

#if ETH29_RX_RATE_UPDATE_TIME == 0
#error ETH29_RX_RATE_UPDATE_TIME must be > 0
#endif



/*
 * protocols defined by the internet system, RFC 790, 09/81
 */
#define	IPPROTO_IP		0		/* dummy for IP */
#define	IPPROTO_ICMP		1		/* control message protocol */
#define	IPPROTO_IGMP		2		/* group mgmt protocol */
#define	IPPROTO_GGP		3		/* gateway^2 (deprecated) */
#define	IPPROTO_TCP		6		/* tcp */
#define	IPPROTO_EGP		8		/* exterior gateway protocol */
#define	IPPROTO_PUP		12		/* pup */
#define	IPPROTO_UDP		17		/* user datagram protocol */
#define	IPPROTO_IDP		22		/* xns idp */
#define	IPPROTO_TP		29 		/* tp-4 w/ class negotiation */
#define	IPPROTO_EON		80		/* ISO cnlp */
#define	IPPROTO_ENCAP		98		/* encapsulation header */

#define	IPPROTO_RAW		255		/* raw IP packet */
#define	IPPROTO_MAX		256



/*
 * Ioctl's have the command encoded in the lower word,
 * and the size of any in or out parameters in the upper
 * word.  The high 3 bits of the upper word are used
 * to encode the in/out status of the parameter.
 */
#define	IOCPARM_MASK	0x1fff		/* parameter length, at most 13 bits */
#define	IOCPARM_LEN(x)	(((x) >> 16) & IOCPARM_MASK)
#define	IOCPARM_MAX	NBPG		/* max size of ioctl, mult. of NBPG */
#define	IOC_VOID	0x20000000	/* no parameters */
#define	IOC_OUT		0x40000000	/* copy out parameters */
#define	IOC_IN		0x80000000	/* copy in parameters */
#define	IOC_INOUT	(IOC_IN|IOC_OUT)
#define	IOC_DIRMASK	0xe0000000	/* mask for IN/OUT/VOID */
/* the 0x20000000 is so we can distinguish new ioctl's from old */
#define	_IO(x,y)	(IOC_VOID|(x<<8)|y)
#define	_IOR(x,y,t)	(IOC_OUT|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
#define	_IOW(x,y,t)	(IOC_IN|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)
/* this should be _IORW, but stdio got there first */
#define	_IOWR(x,y,t)	(IOC_INOUT|((sizeof(t)&IOCPARM_MASK)<<16)|(x<<8)|y)



/*
 * socket I/O controls
 */
#define	SIOCSHIWAT	_IOW('s',  0, int)		/* set high watermark */
#define	SIOCGHIWAT	_IOR('s',  1, int)		/* get high watermark */
#define	SIOCSLOWAT	_IOW('s',  2, int)		/* set low watermark */
#define	SIOCGLOWAT	_IOR('s',  3, int)		/* get low watermark */
#define	SIOCATMARK	_IOR('s',  7, int)		/* at oob mark? */
#define	SIOCSPGRP	_IOW('s',  8, int)		/* set process group */
#define	SIOCGPGRP	_IOR('s',  9, int)		/* get process group */

#define	SIOCADDRT	_IOW('r', 10, struct rtentry)	/* add route */
#define	SIOCDELRT	_IOW('r', 11, struct rtentry)	/* delete route */

#define	SIOCSIFADDR	_IOW('i', 12, struct ifreq)	/* set ifnet address */
#define	SIOCGIFADDR	_IOWR('i',13, struct ifreq)	/* get ifnet address */
#define	SIOCSIFDSTADDR	_IOW('i', 14, struct ifreq)	/* set p-p address */
#define	SIOCGIFDSTADDR	_IOWR('i',15, struct ifreq)	/* get p-p address */
#define	SIOCSIFFLAGS	_IOW('i', 16, struct ifreq)	/* set ifnet flags */
#define	SIOCGIFFLAGS	_IOWR('i',17, struct ifreq)	/* get ifnet flags */
#define	SIOCGIFBRDADDR	_IOWR('i',18, struct ifreq)	/* get broadcast addr */
#define	SIOCSIFBRDADDR	_IOW('i',19, struct ifreq)	/* set broadcast addr */
#define	SIOCGIFCONF	_IOWR('i',20, struct ifconf)	/* get ifnet list */
#define	SIOCGIFNETMASK	_IOWR('i',21, struct ifreq)	/* get net addr mask */
#define	SIOCSIFNETMASK	_IOW('i',22, struct ifreq)	/* set net addr mask */
#define	SIOCGIFMETRIC	_IOWR('i',23, struct ifreq)	/* get IF metric */
#define	SIOCSIFMETRIC	_IOW('i',24, struct ifreq)	/* set IF metric */

#define	SIOCSARP	_IOW('i', 30, struct arpreq)	/* set arp entry */
#define	SIOCGARP	_IOWR('i',31, struct arpreq)	/* get arp entry */
#define	SIOCDARP	_IOW('i', 32, struct arpreq)	/* delete arp entry */

#define SIOCGIFDEBUG	_IOWR('i',50, struct ifreq)	/* get debug level */
#define SIOCSIFDEBUG	_IOW('i',51, struct ifreq)	/* set debug level */



/*
 * socket buffer flags
 */
#define	SB_MAX		(64*1024)	/* max chars in sockbuf */
#define	SB_LOCK		0x01		/* lock on data queue (so_rcv only) */
#define	SB_WANT		0x02		/* someone is waiting to lock */
#define	SB_WAIT		0x04		/* someone is waiting for data/space */
#define	SB_SEL		0x08		/* buffer is selected */
#define	SB_COLL		0x10		/* collision selecting */
#define SB_EVENT	0x20		/* NAT: OS9 ss_sevent done on socket */
#define SB_CPY		0x40		/* copy in progress flag */



/*****************************************************************************/
/*	PRIVATE DATA TYPES						     */
/*****************************************************************************/

/*
 * simplify some types
 */
#ifndef VFP
#define _VFP
typedef void (* VFP)(int);
#endif



/*
 * ETH29 module's addresses
 */
typedef volatile struct eth29_addresses {
  u_char *vme_base;	       /* VME base address */
  u_char *mpram_base;	       /* multi port RAM base */
  u_char *reset_cell;	       /* ETH29 reset cell */
  u_char *mbox_cell;	       /* mail box */
  NIF *nif;		       /* NIF address */
} ETH29_ADDR;



/*
 * Internet address (a structure for historical reasons)
 */
//struct in_addr {
//  u_long s_addr;
//};



/*
 * Socket address, internet style.
 */
//struct sockaddr_in {
//  short	sin_family;
//  u_short	sin_port;
//  struct	in_addr sin_addr;
//  char	sin_zero[8];
//};



/*
 * Interface request structure used for socket
 * ioctl's.  All interface ioctl's must have parameter
 * definitions which begin with ifr_name.  The
 * remainder may be interface specific.
 */
struct	ifreq {
#define	IFNAMSIZ	16
  char	ifr_name[IFNAMSIZ];		/* if name, e.g. "en0" */
  union {
    struct	sockaddr ifru_addr;
    struct	sockaddr ifru_dstaddr;
    struct	sockaddr ifru_broadaddr;
    short	ifru_flags;
    int	ifru_metric;
    int	ifru_debug;
    caddr_t	ifru_data;
  } ifr_ifru;
#define	ifr_addr	ifr_ifru.ifru_addr	/* address */
#define	ifr_dstaddr	ifr_ifru.ifru_dstaddr	/* other end of p-to-p link */
#define	ifr_broadaddr	ifr_ifru.ifru_broadaddr	/* broadcast address */
#define	ifr_flags	ifr_ifru.ifru_flags	/* flags */
#define	ifr_metric	ifr_ifru.ifru_metric	/* metric */
#define	ifr_debug	ifr_ifru.ifru_debug	/* debug level */
#define	ifr_data	ifr_ifru.ifru_data	/* for use by interface */
};



/*
 * Kernel structure per socket.
 * Contains send and receive buffer queues,
 * handle on protocol and pointer to protocol
 * private data and error information.
 */
struct socket {
  long	so_num;			/* socket number */
  short	so_type;		/* generic type, see socket.h */
  short	so_options;		/* from socket call, see socket.h */
  short	so_linger;		/* time to linger while closing */
  short	so_state;		/* internal state flags SS_*, below */
  caddr_t	so_pcb;			/* protocol control block */
  void	*so_proto;		/* protocol handle - unused */
  /*
   * Variables for connection queueing.
   * Socket where accepts occur is so_head in all subsidiary sockets.
   * If so_head is 0, socket is not related to an accept.
   * For head socket so_q0 queues partially completed connections,
   * while so_q is a queue of connections ready to be accepted.
   * If a connection is aborted and it has so_head set, then
   * it has to be pulled out of either so_q0 or so_q.
   * We allow connections to queue up based on current queue lengths
   * and limit on number of queued connections for this socket.
   */
  struct	socket *so_head;	/* back pointer to accept socket */
  struct	socket *so_q0;		/* queue of partial connections */
  struct	socket *so_q;		/* queue of incoming connections */
  short	so_q0len;		/* partials on so_q0 */
  short	so_qlen;		/* number of connections on so_q */
  short	so_qlimit;		/* max number queued connections */
  short	so_timeo;		/* connection timeout */
  u_short	so_error;		/* error affecting connection */
  short	so_pgrp;		/* pgrp for signals */
  u_long	so_oobmark;		/* chars to oob mark */
  /*
   * Variables for socket buffering.
   */
  struct	sockbuf {
    u_long	sb_cc;		/* actual chars in buffer */
    u_long	sb_hiwat;	/* max actual char count */
    u_long	sb_mbcnt;	/* chars of mbufs used */
    u_long	sb_mbmax;	/* max chars of mbufs to use */
    u_long	sb_lowat;	/* low water mark (not used yet) */
    struct	mbuf *sb_mb;	/* the mbuf chain */
#ifdef SMARTBOARD
    caddr_t	sb_sel;		/* process selecting read/write */
#else
    struct	pr_pdsc *sb_sel; /* process selecting read/write */
#endif
    short	sb_timeo;	/* timeout (not used yet) */
    short	sb_flags;	/* flags, see below */
  } so_rcv, so_snd;
#define	SB_MAX		(64*1024)	/* max chars in sockbuf */
#define	SB_LOCK		0x01		/* lock on data queue (so_rcv only) */
#define	SB_WANT		0x02		/* someone is waiting to lock */
#define	SB_WAIT		0x04		/* someone is waiting for data/space */
#define	SB_SEL		0x08		/* buffer is selected */
#define	SB_COLL		0x10		/* collision selecting */
#define SB_EVENT	0x20		/* NAT: OS9 ss_sevent done on socket */
};



/*
 * receive statistics
 */
#ifdef ETH29_RX_STAT_SUPP
typedef struct eth29_receive_statistics {
  unsigned long num_rx_buf;	/* number of received buffer */
  unsigned long rx_buf_size_max;	/* maximum rx buf size */
  unsigned long num_rx_byte;	/* number of received bytes */
  unsigned long rx_rate_max;	/* maximum rx rate */
} ETH29_RX_STAT;
#endif	/* ETH29_RX_STAT_SUPP */



/*****************************************************************************/
/*	PRIVATE FUNCTION PROTOTYPES					     */
/*****************************************************************************/

/* PUBLIC */
int eth29(void);
int eth29_init_l2(interruptVector, interruptLevel);
void eth29_isr(int dummy);
long eth29_com_with_slave(struct mbuf *m_cmd);
int eth29_tx(int loops, int dlen);
int eth29_dbg(int dlevel);

/* L2 support */
int L2_Init(struct L2M_init_p *initpar);
int L2_PhysEthID(u_char *ethid);
int L2_AddVect(u_long proto, u_char vector, u_char level);

/* Socket related */
int P_Socket(int dom, int type, int proto);
int P_Close(int sock);
int P_Shutdown(int sock, int how);
int P_Bind(int sock, caddr_t name, int namelen);
int P_Getsockopt(int sock, int level, int optname, caddr_t optval, int *optlen);
int P_Setsockopt(int sock, int level, int optname, caddr_t optval, int optlen);
int P_Ioctl(int sock, u_long cmd, caddr_t cmdarg);
int P_Connect(int sock, caddr_t name, int namelen);
int P_Send(int sock, caddr_t buf, int len, int flags);
int P_Sendto(int sock, caddr_t buf, int len, int flags, caddr_t to, int tolen);
int P_Recvfrom(int sock, caddr_t buf, int len, int flags, caddr_t from, int *fromlen);
int P_DropData(int sock, caddr_t sockbuf, int len);

/* NIF handling */
void nif_wait(void);
struct mbuf *get_mbuf(void);
u_char *get_page(void);
int put_mbuf(struct mbuf *m);
int put_page(u_char *page);
int action(struct mbuf *m);
int do_mbox(u_char irq_code);
struct mbuf *wait_sl(void);

/* miscellaneous */
u_int printb(void *vpb, u_int lb);
void print_nif(void);
void print_mbuf(struct mbuf *m, int mode);
void print_ieee(u_char *buf, char *txt);

#ifdef ETH29_RX_STAT_SUPP
void eth29_rx_stat_clear(void);
void eth29_rx_stat_print(void);
void eth29_rx_stat_print_rate(void);
void eth29_rx_rate_task(void);
#endif



/*****************************************************************************/
/*	PRIVATE GLOBALS							     */
/*****************************************************************************/

/* ETH29 address mapping */
ETH29_ADDR addr;

/* local general purpose mbuf */
struct mbuf loc_m;

/* receive buffer */
u_char rx_buf[0x20000];

/* array holding socket parameter */
int sock_a[ETH29_MAX_SOCK];				 /* socket identifer */
u_long so_rcv_a[ETH29_MAX_SOCK];		  /* socket buffer addresses */

/* semaphore to indicate interrupt event */
SEM_ID i_evt_sem;

/* receive fifo */
FIFO rx_fifo;

#ifdef ETH29_RX_STAT_SUPP
/* receive statistics */
ETH29_RX_STAT eth29_rx_stat;
#endif


/*****************************************************************************/
/*	PUBLIC GLOBALS							     */
/*****************************************************************************/

int eth29InterruptLevel;

// set true after the eth29 has been succesfully initialized
int eth29hasBeenInitialized = 0;

// the file descriptor to send the datagrams on. If it is < 0,
// the socket is not opened yet, or there has been an error
int datagramFD = -1;

// the current send to ip address
u_long datagramDestIP = 0;

struct sockaddr_in datagramDestSa;		/* internet socket address struct */

// the current send to port, -1 if unitialized
int datagramDestPort = -1;


/*****************************************************************************/
/*	EXTERNAL REFERENCES						     */
/*****************************************************************************/

/*****************************************************************************/
/*	PUBLIC FUNCTION DEFINITIONS					     */
/*****************************************************************************/


void testEth29(int count) {

  int status;
  int i;
  char buffer[5000];

  if (count <= 0) 
    count = 1000;

  status = eth29Init("192.168.10.2", 0xb0, 3);
  
  if (status) {
    logMsg("testEth29: warning, initialization failed\n");
  }

  // try to send anyway; the thing may have already been initialized

  for (i=0; i < count; i++) {
    status = eth29SendDgram("192.168.10.11", 3101, 
			    buffer, sizeof(buffer));
    if (status < 0) {
      logMsg("testEth29: eth29SendDgram failed\n");
      return status; 
    }
  }
}

/*---------------------------------------------------------------------------*
Function:	initialize eth29
Parameters:	none
Input:		none
Output:		none
Return:		0 => ok
		!0 => error

This routine does the folowing:
1. initializes the L2 level of the eth29fc.
2. assigns an IP address to the eth29fc interface

*----------------------------------------------------------------------------*/
int eth29Init(char* interfaceIPaddress, int intVector, int intLevel)
{
  int status;
  u_long ip_addr;					       /* IP address */
  char if_name[32];				   /* interface name */
  struct ifreq ifr;		      /* interface request structure */
  u_char *buf_p;					   /* buffer pointer */
  int i;
  status = 0;

  printd("*** ETH29 initializtion %s (%s %s) ***\n",
	 ETH29_APP_VER, __DATE__, __TIME__);

  if (eth29hasBeenInitialized) {
    logMsg("eth29Init: WARNING, eth29 has already been initialized\n");
    return -1;
  }

  if (!interfaceIPaddress) {
    logMsg("eth29Init: ERROR, interface IP address is bad\n");
    return -1;
  }

  if (intVector < 0 || intVector > 255) {
    logMsg("eth29Init: ERROR, illegal interrupt vector\n");
    return -1;
  }
	 
  if (intLevel < 1 || intLevel > 6) {
    logMsg("eth29Init: ERROR, illegal interrupt level\n");
    return -1;
  }

  /* set ETH29 addresses */
  addr.vme_base = (u_char *)ETH29_VME_BASE;
  addr.mpram_base = (u_char *)ETH29_MPRAM_BASE;
  addr.reset_cell = (u_char *)ETH29_RESET_CELL;
  addr.mbox_cell = (u_char *)ETH29_MBOX_CELL;
  addr.nif = (NIF *)ETH29_NIF_ADDR;

  /* initialize local mbuf */
  loc_m.m_next = NULL;
  loc_m.m_off = (u_long)&(loc_m.m_dat) - (u_long)&loc_m;
  loc_m.m_len = 0;
  loc_m.m_type = MT_DATA;
  loc_m.m_act = NULL;

  /* initialize rx fifo */
  INIT_FIFO(rx_fifo);

  /* clear receive buffer */
  buf_p = rx_buf;
  printd("buf_p 0x%lx rx_buf 0x%lx size %d\n",
	 (u_long)buf_p, (u_long)rx_buf, sizeof(rx_buf));
  for(i = 0; i < 256; i++)
    *buf_p++ = 0xff;

  /* initialize ETH29 */
  eth29_init_l2(intVector, intLevel);

  /* create semaphore */
  SEMCREATE(i_evt_sem);

  /* connect interrupt service routine and enable interrupt */
  printd("  enable isr 0x%lx vector 0x%x level %d\n",
	 (u_long)eth29_isr, intVector, intLevel);
  status = intConnect(INUM_TO_IVEC(intVector), eth29_isr, 0);
  if(status != 0) {
    printd("eth29: connecting isr 0x%lx to vector 0x%x failed\n",
	   (u_long)eth29_isr, intVector);
    return(-1);
  }

  status = sysIntEnable(intLevel);
  if(status != 0) {
    printd("eth29: enabling irq level %d failed\n",
	   intLevel);
    return(-1);
  }

  /* open socket for configuration */
  status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, 0);

  /* issue ioctl to set IP address */
  ip_addr = inet_addr(interfaceIPaddress);
  if(ip_addr == LOOPBACK_IP_ADDR)
    strcpy(if_name, "lo0");
  else
    strcpy(if_name, "nat0");

  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip_addr;
  status = P_Ioctl(sock_a[0], SIOCSIFADDR, (caddr_t)&ifr);

  printd("  set if '%s' to IP address %lu.%lu.%lu.%lu / 0x%lx\n",
	 if_name,
	 (ip_addr >> 24) & 0xff,
	 (ip_addr >> 16) & 0xff,
	 (ip_addr >> 8) & 0xff,
	 ip_addr & 0xff,
	 ip_addr);

  /* issue ioctl to read back IP address */
  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  status = P_Ioctl(sock_a[0], SIOCGIFADDR, (caddr_t)&ifr);

  strcpy(if_name, ifr.ifr_name);
  ip_addr = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
  printd("  read back if '%s' IP address %lu.%lu.%lu.%lu / 0x%lx\n",
	 if_name,
	 (ip_addr >> 24) & 0xff,
	 (ip_addr >> 16) & 0xff,
	 (ip_addr >> 8) & 0xff,
	 ip_addr & 0xff,
	 ip_addr);

  /* issue ioctl to set the interface broadcast address */
  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = (ip_addr & 0xffffff00) | 0xff;
  status = P_Ioctl(sock_a[0], SIOCSIFBRDADDR, (caddr_t)&ifr);
  logMsg("eth29Init: SIOCIFBRDADDR returned %d\n", status);

  /* issue ioctl to read back IP broadcast address */
  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  status = P_Ioctl(sock_a[0], SIOCGIFBRDADDR, (caddr_t)&ifr);

  strcpy(if_name, ifr.ifr_name);
  ip_addr = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
  printd("  read back broadcast if '%s' IP address %lu.%lu.%lu.%lu / 0x%lx\n",
	 if_name,
	 (ip_addr >> 24) & 0xff,
	 (ip_addr >> 16) & 0xff,
	 (ip_addr >> 8) & 0xff,
	 ip_addr & 0xff,
	 ip_addr);

  /* close configuration socket */
  status = P_Close(sock_a[0]);

  eth29hasBeenInitialized = 1;

  return(0);
}


/*---------------------------------------------------------------------------*
Function:	send a datagram to the specified address
Parameters:	sendToIPaddress - the destination IP address
                port  - the destination port
                buffer - the source buffer
                length - the number of bytes to send
Input:		none
Output:		none
Return:		0 => success 
		!0 => error
*----------------------------------------------------------------------------*/
int eth29SendDgram(char* sendToIPaddress, int dest_port, unsigned char* buffer, int length) {

  int i;
  u_long optval;					     /* option value */
  int optlen;					    /* option length */
  u_long dest_addr;
  int status;
  int remain;
  int dataSize;
  int totalsize;
  int pagenum;
  static int sequence_num = 0;
  unsigned char* dataPtr;
  int assemblySize;

  /* a buffer that will contain a UDPHEADER folowed by the data */
  unsigned char  assemblyBuf[MAXPACKET+sizeof(UDPHEADER)];

  /* the UDPHEADER will start at the beginning of assemblyBuf */
  UDPHEADER* udp;

  /* calculate offset into the assembly buffer where the data will go */
  unsigned char* assemblyDataPtr;

  assemblyDataPtr  = assemblyBuf + sizeof(UDPHEADER);
  udp = (UDPHEADER*) &assemblyBuf;
  dest_addr = inet_addr(sendToIPaddress);
  optlen = 0;
  status = 0;

  if (dest_addr == ERROR) {
    logMsg("eth29SendDgram: unable to translate IP address\n");
    return -1;
  }

  /* check data length */
  if(length == 0)
    return 0;

  if ((dest_addr != datagramDestIP) ||
      (dest_port != datagramDestPort) ) {
    if (datagramFD > -1) {
      status = P_Close(datagramFD);
      datagramFD = -1;
    }
  }

  if (datagramFD < 0) {
    /* open UDP transmit socket */
    datagramFD = P_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	
    if (datagramFD < 0) {
      logMsg("eth29SendDgram: unable to create socket\n");
      return -1;
    }

    /* set transmit buffer length (maximum is 55758) */
    optval = 54600;
    //	optval = 16384;
    optlen = sizeof(optval);
    status = P_Setsockopt(datagramFD, SOL_SOCKET, SO_SNDBUF, (caddr_t)&optval, optlen);
    if (status < 0) {
      logMsg("eth29SendDgram: unable to set socket options\n");
      return -1;
    }

    /* set up remote host socket address */
    memcpy((char*)&datagramDestSa.sin_addr,(char*)&dest_addr,sizeof(dest_addr));
    datagramDestSa.sin_family = AF_INET;
    datagramDestSa.sin_port = dest_port;

    datagramDestIP = dest_addr;
    datagramDestPort = dest_port;
  }

  udp->magic = htonl(MAGIC);

  totalsize = length;
  udp->totalsize = htonl(totalsize);

  udp->type = 0;

  dataPtr = buffer;

  pagenum = 0;

  udp->pagenum = htonl(pagenum);
  udp->pages = htonl(totalsize / MAXPACKET + 1);

  for(remain=totalsize; remain>0; remain-=dataSize)
    {

      dataSize = (remain > MAXPACKET) ? MAXPACKET : remain; // data, NOT INCLUDING the header 
      assemblySize = dataSize + sizeof(UDPHEADER);

      udp->pagesize = htonl(assemblySize);
      udp->sequence_num = htonl(sequence_num++);
        
      /* copy the data into the assembly buffer */
      memcpy(assemblyDataPtr, dataPtr, dataSize);
      dataPtr += dataSize;

      /* transmit data */
      status = P_Sendto(datagramFD, assemblyBuf, assemblySize, 0, 
			(caddr_t)&datagramDestSa, 
			sizeof(datagramDestSa));

      if (status != assemblySize) {
	logMsg("eth29SendDgram: unable to Sendto on socket\n");
	return -1;
      }

      udp->pagenum = htonl(pagenum++);
    }

  return(length);

}



/*---------------------------------------------------------------------------*
Function:	main entry point routine
Parameters:	none
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29(void)
{
  int status = 0;
  u_long ip_addr;					       /* IP address */
  char if_name[32];				   /* interface name */
  struct ifreq ifr;		      /* interface request structure */
#if defined ETH29_RX || defined ETH29_FIRMWARE_DBG
  u_long optval;					     /* option value */
  int optlen = 0;					    /* option length */
#endif
#ifdef ETH29_RX
  struct sockaddr_in sa;			  /* internet socket address */
  struct sockaddr rem_sa;			    /* remote socket address */
#endif
  u_char *buf_p;					   /* buffer pointer */
  int i;

  printd("*** ETH29 test application V%s (%s %s) ***\n",
	 ETH29_APP_VER, __DATE__, __TIME__);

  /* set ETH29 addresses */
  addr.vme_base = (u_char *)ETH29_VME_BASE;
  addr.mpram_base = (u_char *)ETH29_MPRAM_BASE;
  addr.reset_cell = (u_char *)ETH29_RESET_CELL;
  addr.mbox_cell = (u_char *)ETH29_MBOX_CELL;
  addr.nif = (NIF *)ETH29_NIF_ADDR;

  /* print NIF */
  print_nif();

  /* initialize local mbuf */
  loc_m.m_next = NULL;
  loc_m.m_off = (u_long)&(loc_m.m_dat) - (u_long)&loc_m;
  loc_m.m_len = 0;
  loc_m.m_type = MT_DATA;
  loc_m.m_act = NULL;

  print_mbuf(&loc_m, 0);

  /* initialize rx fifo */
  INIT_FIFO(rx_fifo);

  /* clear receive buffer */
  buf_p = rx_buf;
  printd("buf_p 0x%lx rx_buf 0x%lx size %d\n",
	 (u_long)buf_p, (u_long)rx_buf, sizeof(rx_buf));
  for(i = 0; i < 256; i++)
    *buf_p++ = 0xff;

#ifdef ETH29_RX_STAT_SUPP
  /* clear rx statistics */
  eth29_rx_stat_clear();
#endif

  /* initialize ETH29 */
  eth29_init_l2(ETH29_INT_VEC, ETH29_INT_LEV);

  /* create semaphore */
  SEMCREATE(i_evt_sem);

  /* connect interrupt service routine and enable interrupt */
  printd("  enable isr 0x%lx vector 0x%x level %d\n",
	 (u_long)eth29_isr, ETH29_INT_VEC, ETH29_INT_LEV);
  status = intConnect(INUM_TO_IVEC(ETH29_INT_VEC), eth29_isr, 0);
  //	status = intConnect((VFP *)ETH29_INT_VEC, eth29_isr, 0);
  if(status != 0) {
    printd("eth29: connecting isr 0x%lx to vector 0x%x failed\n",
	   (u_long)eth29_isr, ETH29_INT_VEC);
    return(-1);
  }

  status = sysIntEnable(ETH29_INT_LEV);
  if(status != 0) {
    printd("eth29: enabling irq level %d failed\n",
	   ETH29_INT_LEV);
    return(-1);
  }

  /* open socket for configuration */
  status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, 0);

#ifdef ETH29_FIRMWARE_DBG
  /* set firmware debug level */
  optval = 6;
  optlen = sizeof(optval);
  status = P_Setsockopt(sock_a[0], SOL_SOCKET, SO_DEBUG, &optval, optlen);
#endif

  /* issue ioctl to set IP address */
  ip_addr = inet_addr(ETH29_IP_ADDR);
  if(ip_addr == ETH29_IP_ADDR)
    strcpy(if_name, "lo0");
  else
    strcpy(if_name, "nat0");

  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
  ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip_addr;
  status = P_Ioctl(sock_a[0], SIOCSIFADDR, (caddr_t)&ifr);

  printd("  set if '%s' to IP address %lu.%lu.%lu.%lu / 0x%lx\n",
	 if_name,
	 (ip_addr >> 24) & 0xff,
	 (ip_addr >> 16) & 0xff,
	 (ip_addr >> 8) & 0xff,
	 ip_addr & 0xff,
	 ip_addr);

  /* issue ioctl to read back IP address */
  memset(&ifr, 0x00, sizeof(struct ifreq));
  strcpy(ifr.ifr_name, if_name);
  status = P_Ioctl(sock_a[0], SIOCGIFADDR, (caddr_t)&ifr);

  strcpy(if_name, ifr.ifr_name);
  ip_addr = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
  printd("  read back if '%s' IP address %lu.%lu.%lu.%lu / 0x%lx\n",
	 if_name,
	 (ip_addr >> 24) & 0xff,
	 (ip_addr >> 16) & 0xff,
	 (ip_addr >> 8) & 0xff,
	 ip_addr & 0xff,
	 ip_addr);

  /* close configuration socket */
  status = P_Close(sock_a[0]);

#ifdef ETH29_RX
  /* open socket to receive data */
  status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

  /* set receive buffer length (maximum is 55758) */
  optval = 54600;
  //	optval = 16384;
  optlen = sizeof(optval);
  status = P_Setsockopt(sock_a[0], SOL_SOCKET, SO_RCVBUF, (caddr_t)&optval, optlen);

  /* bind to protocol */
  memset(&sa, 0x00, sizeof(struct sockaddr_in));
  sa.sin_family = AF_INET;
  sa.sin_port = 50000;
  sa.sin_addr.s_addr = ip_addr;
  status = P_Bind(sock_a[0], (caddr_t)&sa, sizeof(struct sockaddr_in));

#ifdef ETH29_RX_STAT_SUPP
  /* start rx rate task */
  TSPAWN("eths", 1, 0x400, eth29_rx_rate_task);
#endif

  /* wait for receive data via interrupt indication */
  while(status >= 0) {
    optval = sizeof(rem_sa);
    status = P_Recvfrom(sock_a[0], rx_buf, 0x10000, 0, (caddr_t)&rem_sa, (int *)&optval);
#ifdef ETH29_RX_STAT_SUPP
    eth29_rx_stat.num_rx_buf++;
    eth29_rx_stat.num_rx_byte += status;
    if(status > eth29_rx_stat.rx_buf_size_max)
      eth29_rx_stat.rx_buf_size_max = status;
#endif
  }
#endif	/* ETH29_RX */

  printd("*** %s ***\n", (status < 0) ? "ERROR" : "done");
  return(status);
}



/*---------------------------------------------------------------------------*
Function:	initialize ETH29
Parameters:	none
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29_init_l2(int interruptVector, int interruptLevel)
{
  struct L2M_init_p initpar;
  int i;
  u_char ieee_addr[6];
  int error = 0;

  eth29InterruptLevel = interruptLevel;

  printd("\neth29_init_l2: initialize ETH29\n");

  /* set L2 initialization parameter and run L2_Init */
  initpar.L_mode = 0;		  /* no spec bits for net controller */
  for(i = 0; i < 6; i++)			       /* IEEE address empty */
    initpar.my_addr[i] = 0x00;
  initpar.multi_addr = 0;	    /* muticast addr recognition unsupported */
  initpar.License = 0;				 /* no license check */
  initpar.vector = 0;				  /* just historical */
  initpar.level = 0;				  /* just historical */

  error = L2_Init(&initpar);
  if(error) {
    printd("eth29_init_l2: L2_Init ERROR 0x%x\n", error);
    if(error != EACCES)
      return(error);
  }

  printd("  L2 basic initialization done\n");

  /* read back IEEE address from slave */
  error = L2_PhysEthID(ieee_addr);
  if(error) {
    printd("eth29_init_l2: L2_PhysEthID ERROR 0x%x\n", error);
    return(error);
  }

  print_ieee(ieee_addr, "  read back ");

  /* set irq vector and level for TCP/IP on slave */
  error = L2_AddVect(COMPROT_TCPIP, interruptVector, interruptLevel);
  if(error) {
    printd("eth29_init_l2: L2_AddVect ERROR 0x%x\n", error);
    return(error);
  }

  printd("  set irq vector 0x%02x level %d in L2\n",
	 interruptVector, interruptLevel);

  return(0);
}



/*---------------------------------------------------------------------------*
Function:	interrupt service routine
Parameters:	dummy: dummy parameter
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

void eth29_isr(int dummy)
{
  NIF *nif = addr.nif;
  struct mbuf *m, *m0, *m1;
  struct nifpar *nifp;
  u_char *src, *dst;
  u_long copy_len, data_len, len;
  u_long timeout = 100000, dummy_cnt = 0;

  /* get and check for mbuf in NIF */
  m = nif->ActS2H;
  if(((u_long)m & ETH29_MPRAM_MASK) == 0) {
    logMsg("[ENOM]\n", 0, 0, 0, 0, 0, 0);
    return;
  }

  /* clear NIF field */
  nif->ActS2H = NULL;

  /* check command */
  ADDR_S2H(m);
  nifp = mtod(m, struct nifpar *);

#ifdef ETH29_INT_DBG
  logMsg("[0x%lx %lu pid 0x%x]\n",
	 (u_long)m, nifp->Command, nifp->PID, 0, 0, 0);
#endif	/* ETH29_INT_DBG */
  switch(nifp->Command) {
  case L2_RCV:
    /*
     * by default unused calls unless somebody
     * wants to implement a raw interface
     */
    break;

  case S_COPYIN:
    /* copy data from H->S */
    src = nifp->Opt.cpdat.Cpfrom;
    dst = nifp->Opt.cpdat.Cpto;
    ADDR_S2H(dst);
    copy_len = nifp->Opt.cpdat.Cplen;
    memcpy(dst, src, copy_len);

    /* clear command, put mbuf into H2S, issue irq */
    nifp->Command = 0;
    while((nif->ActH2S != NULL) && (timeout--))
      dummy_cnt++;
    if(timeout == 0) {
      logMsg("[ET S_COPYIN]\n", 0, 0, 0, 0, 0, 0);
      return;
    }
    nif->ActH2S = m;
    do_mbox(IRQ_SIGNAL);

    break;

  case S_COPYOUT:
    /* get copy values from S and copy from S->H */
    src = nifp->Opt.cpdat.Cpfrom;
    ADDR_S2H(src);
    dst = nifp->Opt.cpdat.Cpto; 
    copy_len = nifp->Opt.cpdat.Cplen; 
    memcpy(dst, src, copy_len);

    /* clear command, put mbuf into H2S, issue irq */
    nifp->Command = 0;
    while((nif->ActH2S != NULL) && (timeout--))
      dummy_cnt++;
    if(timeout == 0) {
      logMsg("[ET S_COPYIN]\n", 0, 0, 0, 0, 0, 0);
      return;
    }
    nif->ActH2S = m;
    do_mbox(IRQ_SIGNAL);
    break;

  case S_COPYOUT_M:
    /* get copy values from S */
    m0 = (struct mbuf *)nifp->Opt.cpdat.Cpfrom;
    dst = nifp->Opt.cpdat.Cpto;
    data_len = nifp->Opt.cpdat.Cplen;
    len = 0;

    /* copy linked list of mbuf's from S->H */
    do {
      m1 = m0;
      ADDR_S2H(m1);
      src = (u_char *)m1 + m1->m_off;
      copy_len = (m1->m_len < data_len) ? m1->m_len : data_len;
      memcpy(dst, src, copy_len);
      data_len -= copy_len;
      dst += copy_len;
      len += copy_len;
      m0 = m1->m_next;
    } while(m0 && (data_len > 0));

    /* clear command, put mbuf into H2S, issue irq */
    nifp->Command = 0;
    nifp->Opt.cpdat.Cplen = len;
    while((nif->ActH2S != NULL) && (timeout--))
      dummy_cnt++;
    if(timeout == 0) {
      logMsg("[ET S_COPYIN]\n", 0, 0, 0, 0, 0, 0);
      return;
    }
    nif->ActH2S = m;
    do_mbox(IRQ_SIGNAL);

    break;

  case S_SELWAKEUP:
    /*
     * by default unused calls unless somebody
     * wants to implement the UNIX select() call
     */
    break;

  default:
    /* put mbuf into fifo and give signal */
    PUT_FIFO(rx_fifo, m);
    SEMGIVE(i_evt_sem);

    break;
  }
}



/*---------------------------------------------------------------------------*
Function:	basic interface routine for communication with slave board
Parameters:	m_cmd: command mbuf
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

long eth29_com_with_slave(struct mbuf *m_cmd)
{
  struct nifpar *nifp;
  struct mbuf *m;
  long result = 0;

  /* handle mbuf over to slave */
  action(m_cmd);

  /* wait for slave answer */
  for(;;) {
    GET_FIFO(rx_fifo, m);
    if(m)
      break;

    SEMTAKE(i_evt_sem);
  }

  /* get the return status */
  nifp = mtod(m, struct nifpar *);
  result = (long)nifp->Status;
  if(result < 0) {
    printd("eth29_com_with_slave(cmd %ld): ERR - status %ld errno 0x%lx\n",
	   nifp->Command & 0xff, result, nifp->Errno);
  }

  /* return transport buffer back to slave */
  put_mbuf(m);

  return(result);
}



/*---------------------------------------------------------------------------*
Function:	transmit data over a socket
Parameters:	loops: number of transmit loops
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29_tx(int loops, int dlen)
{
  u_char *data;
  int i;
  u_long optval;					     /* option value */
  int optlen = 0;					    /* option length */
  struct sockaddr_in sa;		/* internet socket address struct */
  u_long rh_addr = inet_addr(SEND_TO_IP_ADDR);
  int status = 0;
  u_long tx_bytes = 0;
  u_long ticks;
  u_long tx_rate;

  /* check data length */
  if(dlen == 0)
    dlen = 1000;

  /* allocate data buffer */
  data = malloc(dlen);
  if(data == NULL) {
    printd("eth29_tx: ERR - no memory\n");
    return(-1);
  }

  /* set up data buffer */
  for(i = 0; i < dlen; i++)
    data[i] = 0x5a;


  /* open UDP transmit socket */
  sock_a[1] = P_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);	

  /* set transmit buffer length (maximum is 55758) */
  optval = 54600;
  //	optval = 16384;
  optlen = sizeof(optval);
  status = P_Setsockopt(sock_a[1], SOL_SOCKET, SO_SNDBUF, (caddr_t)&optval, optlen);

  /* set up remote host socket address */
  memcpy((char*)&sa.sin_addr,(char*)&rh_addr,sizeof(rh_addr));
  sa.sin_family = AF_INET;
  sa.sin_port = 50000;

  /* save start time */
  ticks = tickGet();

  /* transmit data */
  for(i = 0; i < loops; i++) {
    status = P_Sendto(sock_a[1], data, dlen, 0, (caddr_t)&sa, sizeof(sa));
    if(status < 0)
      break;
    tx_bytes += status;
  }

  /* transmit ENDE indication (for sockrcv utility) */
  status = P_Sendto(sock_a[1], "ENDE", sizeof("ENDE"), 0, (caddr_t)&sa, sizeof(sa));
  if(status > 0)
    tx_bytes += status;

  /* calculate transfer rate */
  ticks = tickGet() - ticks;
  if(ticks != 0)
    tx_rate = (tx_bytes * sysClkRateGet()) / (1000 * ticks);
  else
    tx_rate = (u_long)(-1);

  /* print results */
  printd("eth29_tx: transmit statistics\n");
  printd("  tx buffer: %d (size %d)\n", i, dlen);
  printd("  tx bytes:  %lu\n", tx_bytes);
  printd("  ticks:     %lu = %lu sec (sysClkRate %d)\n",
	 ticks, ticks / sysClkRateGet(), sysClkRateGet());
  printd("  tx rate:   %lu kbit/s = %lu kByte/s %s\n",
	 tx_rate * 8, tx_rate, ticks ? "" : "(invalid)");

  /* close UDP transmit socket */
  status = P_Close(sock_a[1]);

  return(status);
}



/*---------------------------------------------------------------------------*
Function:	set ETH29 debug level
Parameters:	dlevel: debug level
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29_dbg(int dlevel)
{
  int status = 0;

  if((dlevel < 0) || (dlevel > 9)) {
    printd("eth29_dbg: invalid debug level %d\n", dlevel);
    return(-1);
  }

  /* open socket for configuration */
  status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, 0);
  if(status < 0)
    return(status);

  /* set firmware debug level */
  status = P_Setsockopt(sock_a[0], SOL_SOCKET, SO_DEBUG, (caddr_t)&dlevel, sizeof(dlevel));
  if(status < 0)
    return(status);

  /* close configuration socket */
  status = P_Close(sock_a[0]);

  return(status);
}



/*****************************************************************************/
/*	L2 support							     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Function:	Layer 2 basic initialization
Parameters:	initpar: initialization parameter
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int L2_Init(struct L2M_init_p *initpar)
{
  struct mbuf *m;
  struct nifpar *nifp;
  int i;
  u_long error;

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up L2 init command to slaveboard */
  nifp = (struct nifpar*)loc_m.m_dat;	
  nifp->Command = L2_INIT;
  nifp->Status = 0;
  nifp->PID = 0;
  nifp->Opt.init.L_mode = initpar->L_mode;
  nifp->Opt.init.vector = initpar->vector;
  nifp->Opt.init.level  = initpar->level;
  if((initpar->my_addr[0] != 0x00) || \
     (initpar->my_addr[1] != 0x00) || \
     (initpar->my_addr[2] != 0x00) || \
     (initpar->my_addr[3] != 0x00) || \
     (initpar->my_addr[4] != 0x00) || \
     (initpar->my_addr[5] != 0x00)) {
    /* may overwrite onboard IEEE address */
    for(i = 0; i < 6; i++)
      nifp->Opt.init.my_addr[0] = initpar->my_addr[i];
  }
  nifp->Opt.init.License = initpar->License;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  /* handle over command to slave */
  action(m);

  /* wait for slave answer */
  m = wait_sl();
  if(((u_long)m & ETH29_MPRAM_MASK) == 0)
    return(E_TIMEL2);

  nifp = mtod(m, struct nifpar *);
  error = nifp->Status;
  if(error)
    printd("L2_Init: ERROR 0x%lx\n", error);

  /* return mbuf back to slave */
  put_mbuf(m);

  return((int)error);
}



/*---------------------------------------------------------------------------*
Function:	read IEEE address from slave board
Parameters:	ethid: buffer to store IEEE address
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int L2_PhysEthID(u_char *ethid)
{
  struct mbuf *m;
  struct nifpar *nifp;
  int i;
  u_long error;
	
  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up L2 ethid command to slaveboard */
  nifp = (struct nifpar*)loc_m.m_dat;
  memset(nifp, 0x00, sizeof(struct nifpar));
  nifp->Command = L2_ETHID;
  nifp->Status = 0;
  nifp->PID = 0;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  /* handle over command to slave */
  action(m);

  /* wait for slave answer */
  m = wait_sl();
  if(((u_long)m & ETH29_MPRAM_MASK) == 0)
    return(E_TIMEL2);

  nifp = mtod(m, struct nifpar *);
  error = nifp->Status;
  if(error)
    printd("L2_PhysEthID: ERROR 0x%lx\n", error);

  /* save returned IEEE address */
  for(i = 0; i < 6; i++) {
    ethid[i] = nifp->Opt.ethid[i];
  }

  /* return mbuf back to slave */
  put_mbuf(m);

  return((int)error);
}



/*---------------------------------------------------------------------------*
Function:	set up irq vector and level on slave board
Parameters:	proto: protocol for which interrupt parameter are set
		vector: interrupt vector
		level: interrupt level
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int L2_AddVect(u_long proto, u_char vector, u_char level)
{
  struct mbuf *m;
  struct nifpar *nifp;
  u_long error;

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up L2 interrupt vector command to slaveboard */
  nifp = (struct nifpar*)loc_m.m_dat;	
  nifp->Command = L2_VECTOR;
  nifp->Status = 0xff;
  nifp->PID = 0;
  nifp->Opt.addvect.port = proto;
  nifp->Opt.addvect.board_vector = vector;
  nifp->Opt.addvect.irq_level = level;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  /* handle over command to slave */
  action(m);

  /* wait for slave answer */
  m = wait_sl();
  if(((u_long)m & ETH29_MPRAM_MASK) == 0)
    return(E_TIMEL2);

  nifp = mtod(m, struct nifpar *);
  error = nifp->Status;
  if(error)
    printd("L2_Init: ERROR 0x%lx\n", error);

  /* return mbuf back to slave */
  put_mbuf(m);

  return((int)error);
}



/*****************************************************************************/
/*	Socket related							     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Function:	open socket
Parameters:	dom: domain (INET/OSI)
		type: data type (STREAM/DATAGRAM/RAW)
		proto: protocol (TCP/UDP/IP)
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Socket(int dom, int type, int proto)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Socket: domain %d type %d proto %d pid %d\n",
	 dom, type, proto, ETH29_PID);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up 'create socket' call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_SOCKET;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Socket.dom = dom;
  nifp->Opt.Par_Socket.type = type;
  nifp->Opt.Par_Socket.proto = proto;
  nifp->Opt.Par_Socket.uid = ETH29_PID;
  nifp->Opt.Par_Socket.psock = 0;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	close socket
Parameters:	sock: socket identifier
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Close(int sock)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Close: sock %d\n", sock);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up 'close socket' call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;	
  nifp->Command = SOC_CLOSE;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Close.socket = sock;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	shutdown 'full-duplex-connection' socket
Parameters:	sock: socket identifier
		how: 0 => shutdown receive
		     1 => shutdown transmit
		     2 => shutdown receive and transmit
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Shutdown(int sock, int how)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Shutdown: sock %d how %d\n", sock, how);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up 'shutdown socket' call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_SHUTDOWN;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Shutdown.socket = sock;
  nifp->Opt.Par_Shutdown.how = how;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	bind specific name (protocol parameter) to socket
Parameters:	sock: socket identifier
		name: internet socket address structure
		namelen: length of name
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Bind(int sock, caddr_t name, int namelen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Bind: sock %d name 0x%lx namelen %d\n",
	 sock, (u_long)name, namelen);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up bind call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_BIND;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Bind.socket = sock;
  nifp->Opt.Par_Bind.name = name;
  nifp->Opt.Par_Bind.namelen = namelen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	get socket options
Parameters:	sock: socket identifier
		level: protocol level (SOL_SOCKET)
		optname: option name
		optval: place to store option value
		optlen: place to store option length
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Getsockopt(int sock, int level, int optname, caddr_t optval, int *optlen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Getsockopt: sock %d level 0x%x optname 0x%x optval 0x%lx\n",
	 sock, level, optname, (u_long)optval);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up getsockopt call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_GETSOCKOPT;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Getsockopt.socket = sock;
  nifp->Opt.Par_Getsockopt.level = level;
  nifp->Opt.Par_Getsockopt.name = optname;
  nifp->Opt.Par_Getsockopt.val = optval;
  nifp->Opt.Par_Getsockopt.avalsize = optlen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	set socket options
Parameters:	sock: socket identifier
		level: protocol level (SOL_SOCKET)
		optname: option name
		optval: option value to be set
		optlen: option length
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Setsockopt(int sock, int level, int optname, caddr_t optval, int optlen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Setsockopt: sock %d level %d optname %d optval 0x%lx optlen %d\n",
	 sock, level, optname, (u_long)optval, optlen);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up setsockopt call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_SETSOCKOPT;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Setsockopt.socket = sock;
  nifp->Opt.Par_Setsockopt.level = level;
  nifp->Opt.Par_Setsockopt.name = optname;
  nifp->Opt.Par_Setsockopt.val = optval;
  nifp->Opt.Par_Setsockopt.valsize = optlen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	get socket options
Parameters:	sock: socket identifier
		cmd: command
		cmdarg: addtional command arguments
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Ioctl(int sock, u_long cmd, caddr_t cmdarg)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Ioctl: sock %d cmd 0x%lx cmdarg 0x%lx\n",
	 sock, cmd, (u_long)cmdarg);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up ioctl call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_IOCTL;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Ioctl.socket = sock;
  nifp->Opt.Par_Ioctl.cmd = cmd;
  nifp->Opt.Par_Ioctl.cmarg = cmdarg;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	connect to a remote server
Parameters:	sock: socket identifier
		name: internet socket address struct
		namelen: length of internet socket address struct
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Connect(int sock, caddr_t name, int namelen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  printd("P_Connect: sock %d name 0x%lx namelen 0x%x\n",
	 sock, (u_long)name, namelen);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up ioctl call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_CONNECT;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Connect.socket = sock;
  nifp->Opt.Par_Connect.name = name;
  nifp->Opt.Par_Connect.namelen = namelen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}

/*---------------------------------------------------------------------------*
Function:	send data to socket (connected)
Parameters:	sock: socket identifier
		buf: buffer to send
		len: length of buffer
		flags: transmit option flags
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Send(int sock, caddr_t buf, int len, int flags)
{
  struct mbuf *m;
  struct nifpar *nifp;

  PRINTD("P_Send: sock %d buf 0x%lx len %d flags 0x%lx\n",
	 sock, (u_long)buf, len, (u_long)flags);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up ioctl call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_SEND;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Send.socket = sock;
  nifp->Opt.Par_Send.buf = buf;
  nifp->Opt.Par_Send.len = len;
  nifp->Opt.Par_Send.flags = flags;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	send data to socket (not connected)
Parameters:	sock: socket identifier
		buf: buffer to send
		len: length of buffer
		flags: transmit option flags
		to: if !NULL => socket address of remote socket
		tolen: lenght of to
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Sendto(int sock, caddr_t buf, int len, int flags, caddr_t to, int tolen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  PRINTD("P_Sendto: sock %d buf 0x%lx len %d flags 0x%lx to 0x%lx tolen %d\n",
	 sock, (u_long)buf, len, (u_long)flags, (u_long)to, tolen);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up ioctl call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_SENDTO;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Send.socket = sock;
  nifp->Opt.Par_Send.buf = buf;
  nifp->Opt.Par_Send.len = len;
  nifp->Opt.Par_Send.flags = flags;
  nifp->Opt.Par_Sendto.to = to;
  nifp->Opt.Par_Sendto.tolen = tolen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	read data from socket (connected or disconnected)
Parameters:	sock: socket identifier
		buf: buffer to store received data
		len: length of recieve buffer
		flags: receive option flags
		from: if !NULL => socket address of remote socket
		fromlen: lenght of from
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Recvfrom(int sock, caddr_t buf, int len, int flags, caddr_t from, int *fromlen)
{
  struct mbuf *m;
  struct nifpar *nifp;

  PRINTD("P_Recvfrom: sock %d buf 0x%lx len %d flags 0x%x from 0x%lx\n",
	 sock, (u_long)buf, len, flags, (u_long)from);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up 'recvfrom socket' call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_RECVFROM;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Recvfrom.socket = sock;
  nifp->Opt.Par_Recvfrom.buf = buf;
  nifp->Opt.Par_Recvfrom.len = len;
  nifp->Opt.Par_Recvfrom.flags = flags;
  nifp->Opt.Par_Recvfrom.from = from;
  nifp->Opt.Par_Recvfrom.fromlenaddr = fromlen;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  return(eth29_com_with_slave(m));
}



/*---------------------------------------------------------------------------*
Function:	bind socket to protocol
Parameters:	sock: socket identifier
		sockbuf: pointer to socket buffer
		len: data length
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_DropData(int sock, caddr_t sockbuf, int len)
{
  struct mbuf *m;
  struct nifpar *nifp;
  int error;

  PRINTD("P_DropData: sock %d sockbuf 0x%lx len %d\n",
	 sock, (u_long)sockbuf, len);

  /* get mbuf */
  m = get_mbuf();
  if(m == NULL)
    return(E_MEM);

  /* set up bind call parameters */
  nifp = (struct nifpar*)loc_m.m_dat;
  nifp->Command = SOC_DROPDATA;
  nifp->Command |= (COMPROT_TCPIP << 8);
  nifp->PID = ETH29_PID;
  nifp->Opt.Par_Sock_DropData.psockbuf = (u_long)sockbuf;
  nifp->Opt.Par_Sock_DropData.data_len = len;

  /* copy NIF parameter from local mbuf into slave mbuf */
  memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
  m->m_len = sizeof(struct nifpar);

  /* handle over command to slave */
  error = action(m);

  /* no slave answer */
  return(error);
}



/*****************************************************************************/
/*	NIF handling							     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Function:	delay NIF access
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void nif_wait(void)
{
  volatile u_long i, dummy = 0;

  for(i = 0; i < 100000; i++)
    dummy++;
}



/*---------------------------------------------------------------------------*
Function:	get mbuf from slave
Parameters:	none
Input:		none
Output:		none
Return:		!NULL => pointer to mbuf
		NULL => error
*----------------------------------------------------------------------------*/

struct mbuf *get_mbuf(void)
{
  NIF *nif;
  struct mbuf *m;
  int i = 0;
  int ik;

  nif = addr.nif;
  INTLOCK(ik);

  while((*addr.mbox_cell != 0) || (nif->mget == NULL)) {
    i++;
    if(i == NIF_MAX_WAIT) {
      INTUNLOCK(ik);
      printd("get_mbuf: ERROR - mbox 0x%02x mget 0x%lx\n",
	     *addr.mbox_cell, (u_long)nif->mget);
      return(NULL);
    }
    nif_wait();
  }

  m = nif->mget;
  nif->mget = NULL;
  INTUNLOCK(ik);
  do_mbox(IRQ_MGET);
  ADDR_S2H(m);
  PRINTD("get_mbuf: 0x%lx\n", (u_long)m);
  return(m);
}



/*---------------------------------------------------------------------------*
Function:	get page from slave
Parameters:	none
Input:		none
Output:		none
Return:		!NULL => pointer to page
		NULL => error
*----------------------------------------------------------------------------*/

u_char *get_page(void)
{
  NIF *nif;
  u_char *p;
  int i = 0;
  int ik;

  nif = addr.nif;
  INTLOCK(ik);

  while((*addr.mbox_cell != 0) || (nif->pget == NULL)) {
    i++;
    if(i == NIF_MAX_WAIT) {
      INTUNLOCK(ik);
      printd("get_page: ERROR - mbox 0x%02x pget 0x%lx\n",
	     *addr.mbox_cell, (u_long)nif->pget);
      return(NULL);
    }
    nif_wait();
  }

  p = (u_char *)nif->pget;
  INTUNLOCK(ik);
  do_mbox(IRQ_MGET);
  ADDR_S2H(p);
  PRINTD("get_page: 0x%lx\n", (u_long)p);
  return(p);
}



/*---------------------------------------------------------------------------*
Function:	put mbuf back to slave
Parameters:	m: mbuf
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int put_mbuf(struct mbuf *m)
{
  NIF *nif;
  int i = 0;
  int ik;

  nif = addr.nif;
  INTLOCK(ik);

  while((*addr.mbox_cell != 0) || (nif->mput != NULL)) {
    i++;
    if(i == NIF_MAX_WAIT) {
      INTUNLOCK(ik);
      printd("put_mbuf: ERROR - mbox 0x%02x mput 0x%lx\n",
	     *addr.mbox_cell, (u_long)nif->mput);
      return(-1);
    }
    nif_wait();
  }

  nif->mput = m;
  INTUNLOCK(ik);
  do_mbox(IRQ_MPUT);
  PRINTD("put_mbuf: 0x%lx\n", (u_long)m);
  return(0);
}



/*---------------------------------------------------------------------------*
Function:	put page back to slave
Parameters:	p: page
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int put_page(u_char *p)
{
  NIF *nif;
  int i = 0;
  int ik;

  nif = addr.nif;
  INTLOCK(ik);

  while((*addr.mbox_cell != 0) || (nif->pput != NULL)) {
    i++;
    if(i == NIF_MAX_WAIT) {
      INTUNLOCK(ik);
      printd("put_page: ERROR - mbox 0x%02x pput 0x%lx\n",
	     *addr.mbox_cell, (u_long)nif->pput);
      return(-1);
    }
    nif_wait();
  }

  nif->pput = (struct mbuf *)p;
  INTUNLOCK(ik);
  do_mbox(IRQ_MPUT);
  PRINTD("put_page: 0x%lx\n", (u_long)p);
  return(0);
}



/*---------------------------------------------------------------------------*
Function:	handle over mbuf to slave
Parameters:	m: mbuf
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int action(struct mbuf *m)
{
  NIF *nif;
  int i = 0;
  int ik;

  nif = addr.nif;
  INTLOCK(ik);

  while((*addr.mbox_cell != 0) || (nif->ActH2S != NULL)) {
    i++;
    if(i == NIF_MAX_WAIT) {
      INTUNLOCK(ik);
      printd("action: ERROR - mbox 0x%02x ActH2S 0x%lx\n",
	     *addr.mbox_cell, (u_long)nif->ActH2S);
      return(-1);
    }
    nif_wait();
  }

  nif->ActH2S = m;
  INTUNLOCK(ik);
  do_mbox(IRQ_ActH2S);
  PRINTD("action: 0x%lx\n", (u_long)m);
  return(0);
}



/*---------------------------------------------------------------------------*
Function:	issue mail box interrupt
Parameters:	irq_code: interrupt code to be written into mail box
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int do_mbox(u_char irq_code)
{
  /*	PRINTD("%s\n",
	(irq_code == IRQ_MGET) ? "IRQ_MGET" : \
	(irq_code == IRQ_MPUT) ? "IRQ_MPUT" : \
	(irq_code == IRQ_ActH2S) ? "IRQ_ActH2S" : \
	(irq_code == IRQ_ActS2H) ? "IRQ_ActS2H" : \
	(irq_code == IRQ_PGET) ? "IRQ_PGET" : \
	(irq_code == IRQ_PPUT) ? "IRQ_PPUT" : \
	(irq_code == IRQ_SNDRAW) ? "IRQ_SNDRAW" : \
	(irq_code == IRQ_RCVRAW) ? "IRQ_RCVRAW" : \
	(irq_code == IRQ_SIGNAL) ? "IRQ_SIGNAL" : \
	(irq_code == IRQ_RTCOPLEN) ? "IRQ_RTCOPLEN" : \
	"unknown");
  */
  *addr.mbox_cell = irq_code;
  return(0);
}



/*---------------------------------------------------------------------------*
Function:	wait for slave answer in ActS2H
Parameters:	none
Input:		none
Output:		none
Return:		!NULL => pointer to answer mbuf
		NULL => error
*----------------------------------------------------------------------------*/

struct mbuf *wait_sl(void)
{
  NIF *nif = addr.nif;
  u_long i1 = 0;
  struct mbuf *m;

  TSLEEP(20);

  while(nif->ActS2H == NULL) {
    nif_wait();
    i1++;
    if(i1 > NIF_MAX_WAIT) {
      printd("wait_sl: 0x%lx ERROR\n", (u_long)m);
      return(NULL);
    }
  }

  m = nif->ActS2H;
  nif->ActS2H = NULL;
  ADDR_S2H(m);

  PRINTD("wait_sl: 0x%lx\n", (u_long)m);
  return(m);
}



/*****************************************************************************/
/*	miscellaneous							     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Function:	print buffer
Parameters:	vpb: pointer to buffer
		lb: len of buffer
Input:		buffer parameter
Output:		printed buffer
Return:		number of printed character
*----------------------------------------------------------------------------*/

u_int printb(void *vpb, u_int lb)
{
  u_char *pb = vpb;
  u_int i = 0;

  printd("printb: buffer at 0x%08lx of len %u\n",
	 (u_long)pb, lb);	

  while(lb--) {
    printd(" %02x", *pb++);
    if(!(++i%8)) printd("  ");
    if(!(i%16)) printd("\n");
  }
  printd("\n");

  return(i);
}



/*---------------------------------------------------------------------------*
Function:	print NIF contents
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void print_nif(void)
{
  NIF *nif = addr.nif;
  int ik;

  INTLOCK(ik);
  printd("NIF at 0x%08lx\n", (u_long)nif);
  printd("  mget   0x%08lx\n", (u_long)nif->mget);
  printd("  mput   0x%08lx\n", (u_long)nif->mput);
  printd("  ActH2S 0x%08lx\n", (u_long)nif->ActH2S);
  printd("  ActS2H 0x%08lx\n", (u_long)nif->ActS2H);
  printd("  pget   0x%08lx\n", (u_long)nif->pget);
  printd("  pput   0x%08lx\n", (u_long)nif->pput);
  /*	printd("  SndRaw 0x%08lx\n", (u_long)nif->SndRaw);
	printd("  SndRSz 0x%08lx/%lu\n", nif->SndRSz, nif->SndRSz);
	printd("  RcvRaw 0x%08lx\n", (u_long)nif->RcvRaw);
	printd("  RcvRSz 0x%08lx/%lu\n", nif->RcvRSz, nif->RcvRSz);*/
  INTUNLOCK(ik);
}



/*---------------------------------------------------------------------------*
Function:	print mbuf settings
Parameters:	m: mbuf
		mode: options on print_mbuf
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void print_mbuf(struct mbuf *m, int mode)
{
  u_char *pb;

  printd("print_mbuf: 0x%lx\n", (u_long)m);
  printd("  m_next: 0x%lx\n", (u_long)(m->m_next));
  printd("  m_off:  0x%lx/%lu\n", m->m_off, m->m_off);
  printd("  m_len:  %d\n", m->m_len);
  printd("  m_dat:  0x%lx\n", (u_long)(m->m_dat));
  printd("  m_act:  0x%lx\n", (u_long)(m->m_act));

  /* print data */
  if(mode == 1) {
    pb = mtod(m, u_char *);
    printb(pb, m->m_len);
  }

  /* print m->m_dat area */
  if(mode == 2) {
    printb(m->m_dat, MLEN);
  }
}



/*---------------------------------------------------------------------------*
Function:	print IEEE address from buffer
Parameters:	buf: buffer
		txt: additional text to be printed
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void print_ieee(u_char *buf, char *txt)
{
  printd("%sIEEE %02x:%02x:%02x:%02x:%02x:%02x\n",
	 txt ? txt : "",
	 buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]); 
}


#ifdef ETH29_RX_STAT_SUPP
/*---------------------------------------------------------------------------*
Function:	clear receive statistics
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void eth29_rx_stat_clear(void)
{
  u_char *cur = (u_char *)&eth29_rx_stat;
  int i;

  for(i = 0; i < sizeof(ETH29_RX_STAT); i++)
    cur[i] = 0x00;
}



/*---------------------------------------------------------------------------*
Function:	print receive statistics
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void eth29_rx_stat_print(void)
{
  printd("eth29_rx_stat_print: rx stat at 0x%lx\n",
	 (u_long)&eth29_rx_stat);
  printd("  num_rx_buf:      %lu\n", eth29_rx_stat.num_rx_buf);
  printd("  rx_buf_size_max: %lu\n", eth29_rx_stat.rx_buf_size_max);
  printd("  num_rx_byte:     %lu\n", eth29_rx_stat.num_rx_byte);
  printd("  rx_rate_max:     %lu kByte/s\n", eth29_rx_stat.rx_rate_max);
}



/*---------------------------------------------------------------------------*
Function:	print receive data rate
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void eth29_rx_stat_print_rate(void)
{
  static int first_call = 0;
  static u_long num_rx_byte;
  u_long rate;
  u_long seconds = ETH29_RX_RATE_UPDATE_TIME;

  /* initialize variable on first call */
  if(first_call == 0) {
    num_rx_byte = eth29_rx_stat.num_rx_byte;
    first_call = 1;
    return;
  }

  /* calculate rx rate */
  rate = (eth29_rx_stat.num_rx_byte - num_rx_byte) / seconds;
  rate /= 1000;
  printd("%5lu kbit/s %5lu kByte/s\n", 8 * rate, rate);

  if(rate > eth29_rx_stat.rx_rate_max)
    eth29_rx_stat.rx_rate_max = rate;

  /* update local variables */
  num_rx_byte = eth29_rx_stat.num_rx_byte;
}



/*---------------------------------------------------------------------------*
Function:	periodically rx statistics
Parameters:	none
Input:		none
Output:		none
Return:		none
*----------------------------------------------------------------------------*/

void eth29_rx_rate_task(void)
{
  int delay = ETH29_RX_RATE_UPDATE_TIME * sysClkRateGet();
  int count = 0;

  printd("eth29_rx_rate_task: update time %d s sysClkRate %d\n",
	 ETH29_RX_RATE_UPDATE_TIME, sysClkRateGet());

  for(;;) {
    TSLEEP(delay);
    eth29_rx_stat_print_rate();

    if(count == 10) {
      eth29_rx_stat_print();
      count = 0;
    }
    count++;
  }
}
#endif	/* ETH29_RX_STAT_SUPP */


