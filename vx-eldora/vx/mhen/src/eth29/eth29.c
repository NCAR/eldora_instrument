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
* --------- Revision 1.1.1.1  2003/04/07 19:24:03  martinc
* --------- import
* ---------
* 011025 cg added routine eth29_check_data to check data sent by two different
*           remote systems;
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
#define ETH29_APP_VER	"0.4"

#ifdef DEFINE_BASIC_TYPES

/*
 * missing types
 */
#ifndef _U_CHAR
#define _U_CHAR
typedef unsigned char	u_char;
#endif

#ifndef _U_SHORT
#define _U_SHORT
typedef unsigned short	u_short;
#endif

#ifndef _U_INT
#define _U_INT
typedef unsigned int	u_int;
#endif

#ifndef _U_LONG
#define _U_LONG
typedef unsigned long	u_long;
#endif

#ifndef U_CHAR
#define U_CHAR	unsigned char
#endif

#ifndef U_SHORT
#define U_SHORT	unsigned short
#endif

#ifndef U_INT
#define U_INT	unsigned int
#endif

#ifndef U_LONG
#define U_LONG	unsigned long
#endif

#ifndef UCHAR
#define UCHAR	unsigned char
#endif

#ifndef USHORT
#define USHORT	unsigned short
#endif

#ifndef UINT
#define UINT	unsigned int
#endif

#ifndef ULONG
#define ULONG	unsigned long
#endif

#ifndef _CADDR_T
#define _CADDR_T
typedef void *caddr_t;
#endif

#endif

/*****************************************************************************/
/*	INCLUDES							     */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include "mbuf.h"
#include "nif.h"
#include "socket.h"
#include "neterr.h"
#include "neterrno.h"



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

#ifdef ETH29_INT_DBG
#define PRINTI(fmt,arg...)	printi(fmt, ## arg)
#else
#define PRINTI(fmt,arg...)
#endif

#ifndef printd
//#define printd	printf
#define printd	//
#endif

/*
 * ETH29 configuration
 */
//#define ETH29_POLL_RX		  /* use rx data polling instead of irq ind. */

//#define ETH29_IP_REM_ADDR	0x8493a0f2		/* remote IP address */
#define ETH29_IP                "128.117.80.81"       /* IP address of eth29 interface */
//#define ETH29_LOOP_IP_ADDR	0x7f000001	      /* loopback IP address */
//#define ETH29_IP_ADDR		ETH29_IP_REM_ADDR	       /* IP address */
#define SEND_IP                 "128.117.80.170"     /* address to send datagrams to */

#define DATAGRAM_SIZE           5000

/*
 * ETH29 access
 */
#define ETH29_VME_BASE		0x90000000		 /* VME base address */
//#define ETH29_VME_BASE		0xeac00000		 /* VME base address */

#define ETH29_MPRAM_BASE	ETH29_VME_BASE	       /* MPRAM base address */
#define ETH29_MPRAM_SIZE	0x00400000		       /* MPRAM size */
#define ETH29_MPRAM_MASK	(ETH29_MPRAM_SIZE - 1)	       /* MPRAM mask */
//#define ETH29_RESET_CELL	0xeacffffc	   /* ETH29 reset if written */
#define ETH29_RESET_CELL	(ETH29_VME_BASE + 0xffffc)	   /* ETH29 reset if written */
//#define ETH29_MBOX_CELL		0xeacfffe0			 /* mail box */
#define ETH29_MBOX_CELL		(ETH29_VME_BASE + 0xfffe0)		 /* mail box */
#define ETH29_NIF_ADDR		((ETH29_VME_BASE) + 0x4000)   /* NIF address */

#define ETH29_INT_VEC		0xa0			 /* interrupt vector */
#define ETH29_INT_LEV		2			  /* interrupt level */

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
struct in_addr {
	u_long s_addr;
};



/*
 * Socket address, internet style.
 */
struct sockaddr_in {
	short	sin_family;
	u_short	sin_port;
	struct	in_addr sin_addr;
	char	sin_zero[8];
};



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



/*****************************************************************************/
/*	PRIVATE FUNCTION PROTOTYPES					     */
/*****************************************************************************/

/* PUBLIC */
int eth29(void);
int eth29_init(void);
int eth29_poll_rx(int sock);
int eth29_check_data(u_char *data, int data_count);
void eth29_isr(int dummy);

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
int P_Recvfrom(int sock, caddr_t buf, int len, int flags, caddr_t from, int *fromlen);
int P_Send(int sock, caddr_t buf, int len);
int P_Sendto(int sock, caddr_t buf, int len, int flags, caddr_t from, int *fromlen);
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
void *wait_sem(int timeout);

/* miscellaneous */
int printi(const char *fmt, ...);
u_int printb(void *vpb, u_int lb);
void print_nif(void);
void print_mbuf(struct mbuf *m, int mode);
void print_ieee(u_char *buf, char *txt);



/*****************************************************************************/
/*	PRIVATE GLOBALS							     */
/*****************************************************************************/

/* ETH29 address mapping */
ETH29_ADDR addr;

/* local general purpose mbuf */
struct mbuf loc_m;

/* receive buffer */
u_char dg_buf[DATAGRAM_SIZE];

/* array holding socket parameter */
int sock_a[ETH29_MAX_SOCK];				 /* socket identifer */
u_long so_rcv_a[ETH29_MAX_SOCK];		  /* socket buffer addresses */

/* semaphore to indicate irq and var to save mbuf */
static int sem_flag = 0;
static struct mbuf *m_save = NULL;



/*****************************************************************************/
/*	PUBLIC GLOBALS							     */
/*****************************************************************************/



/*****************************************************************************/
/*	EXTERNAL REFERENCES						     */
/*****************************************************************************/

#include <vxWorks.h>
#include <iv.h>

#ifdef NEVER
/* vxWorks specific */
int intConnect(VFP *vec, VFP isr, int par);	       /* connect isr to irq */
int sysIntEnable(int level);				 /* enable interrupt */
int intLock(void);				      /* disables interrupts */
int intUnlock(int lockKey);			       /* enables interrupts */
int taskDelay(int ticks);				    /* delays a task */

int logMsg(char *fmt,				    /* vxWorks print routine */
	   int arg0,
	   int arg1,
	   int arg2,
	   int arg3,
	   int arg4,
	   int arg5);


#endif

/*****************************************************************************/
/*	PUBLIC FUNCTION DEFINITIONS					     */
/*****************************************************************************/

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
	int nDatagram = 0;
	int status = 0;
	u_long ip_addr;					       /* IP address */
	u_long ip_addr_to;					       /* IP address */
	char if_name[32];				   /* interface name */
	u_long optval;					     /* option value */
	int optlen = 0;					    /* option length */
	struct ifreq ifr;		      /* interface request structure */
	struct sockaddr_in sa;			  /* internet socket address */
#ifndef ETH29_POLL_RX
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

	/* clear receive buffer */
	buf_p = dg_buf;
	printd("buf_p 0x%lx dg_buf 0x%lx size %d\n",
		(u_long)buf_p, (u_long)dg_buf, sizeof(dg_buf));
	for(i = 0; i < 256; i++)
		*buf_p++ = 0xff;

	/* initialize ETH29 */
	eth29_init();

	/* connect interrupt service routine and enable interrupt */
	printd("  enable isr 0x%lx vector 0x%x level %d\n",
		(u_long)eth29_isr, ETH29_INT_VEC, ETH29_INT_LEV);
	//	status = intConnect((VFP *)ETH29_INT_VEC, eth29_isr, 0);
	status = intConnect(INUM_TO_IVEC(ETH29_INT_VEC), eth29_isr, 0);
	if(status != 0) {
		printd("eth29: connecting isr 0x%lx to vector 0x%x failed\n",
			(u_long)eth29_isr, ETH29_INT_VEC);
		return(-1);
	}

	status = sysIntEnable(ETH29_INT_LEV);
	if(status != 0) {
		logMsg("eth29: enabling irq level %d failed\n",
			ETH29_INT_LEV);
		return(-1);
	}

	/* open socket for configuration */
		status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, 0);
	//status = sock_a[0] = P_Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	/* set firmware debug level */
	optval = 9;
	optlen = sizeof(optval);
	//	status = P_Setsockopt(sock_a[0], SOL_SOCKET, SO_DEBUG, &optval, optlen);

	/* issue ioctl to set IP address */
	ip_addr = inet_addr(ETH29_IP);
	strcpy(if_name, "nat0");

	memset(&ifr, 0x00, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, if_name);
	((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
	((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip_addr;
	status = P_Ioctl(sock_a[0], SIOCSIFADDR, &ifr);

	logMsg("  set if '%s' to IP address %lu.%lu.%lu.%lu / 0x%lx\n",
		if_name,
		(ip_addr >> 24) & 0xff,
		(ip_addr >> 16) & 0xff,
		(ip_addr >> 8) & 0xff,
		ip_addr & 0xff,
		ip_addr);

	/* issue ioctl to read back IP address */
	memset(&ifr, 0x00, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, if_name);
	status = P_Ioctl(sock_a[0], SIOCGIFADDR, &ifr);

	strcpy(if_name, ifr.ifr_name);
	ip_addr = ((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr;
	logMsg("  read back if '%s' IP address %lu.%lu.%lu.%lu / 0x%lx\n",
		if_name,
		(ip_addr >> 24) & 0xff,
		(ip_addr >> 16) & 0xff,
		(ip_addr >> 8) & 0xff,
		ip_addr & 0xff,
		ip_addr);

	/* close configuration socket */
	status = P_Close(sock_a[0]);

	/* open socket to send data */
	status = sock_a[0] = P_Socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	//status = sock_a[0] = P_Socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	logMsg("P_Socket returned a status of %d\n", status);

	/* bind to it */
	memset(&sa, 0x00, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = 50000;
	sa.sin_addr.s_addr = ip_addr;
	status = P_Bind(sock_a[0], &sa, sizeof(sa));
	logMsg("P_Bind returned a status of %d\n", status);

	/* enable broadcast */
	//	i = 1;
	// status = P_Setsockopt(sock_a[0], SOL_SOCKET, SO_BROADCAST, &i, sizeof(i));
	// logMsg("P_Setsockopt (SO_BROADCAST) returned %d\n", status);

	/* set up the destination address for sendto */
	ip_addr_to = inet_addr(SEND_IP);
	strcpy(if_name, SEND_IP);

	memset(&ifr, 0x00, sizeof(struct ifreq));
	strcpy(ifr.ifr_name, if_name);
	((struct sockaddr_in *) &ifr.ifr_addr)->sin_family = AF_INET;
	((struct sockaddr_in *) &ifr.ifr_addr)->sin_addr.s_addr = ip_addr_to;

	logMsg("  set destination if '%s' to IP address %lu.%lu.%lu.%lu / 0x%lx\n",
		if_name,
		(ip_addr_to >> 24) & 0xff,
		(ip_addr_to >> 16) & 0xff,
		(ip_addr_to >> 8) & 0xff,
		ip_addr_to & 0xff,
		ip_addr_to);

	memset(&sa, 0x00, sizeof(struct sockaddr_in));
	sa.sin_family = AF_INET;
	sa.sin_port = 50000;
	sa.sin_addr.s_addr = ip_addr_to;
	status = P_Connect(sock_a[0], &sa, sizeof(sa));
	logMsg("P_Connect returned %d\n", status);

	logMsg("starting datagram loop\n");
	/* wait for receive data via interrupt indication */
	while(status >= 0) {
	  status = P_Send(sock_a[0], dg_buf, DATAGRAM_SIZE);
	  
	  if (status != DATAGRAM_SIZE)
	    logMsg("P_Send status is %d\n", status);

	  nDatagram++;
	  if ((nDatagram %100) == 0) {
	    logMsg("\r%d", nDatagram);
	  }
	}

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

int eth29_init(void)
{
	struct L2M_init_p initpar;
	int i;
	u_char ieee_addr[6];
	int error = 0;

	printd("\neth29_init: initialize ETH29\n");

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
		printd("eth29_init: L2_Init ERROR 0x%x\n", error);
		if(error != EACCES)
			return(error);
	}

	printd("  L2 basic initialization done\n");

	/* read back IEEE address from slave */
	error = L2_PhysEthID(ieee_addr);
	if(error) {
		printd("eth29_init: L2_PhysEthID ERROR 0x%x\n", error);
		return(error);
	}

	print_ieee(ieee_addr, "  read back ");

	/* set irq vector and level for TCP/IP on slave */
	error = L2_AddVect(COMPROT_TCPIP, ETH29_INT_VEC, ETH29_INT_LEV);
	if(error) {
		printd("eth29_init: L2_AddVect ERROR 0x%x\n", error);
		return(error);
	}

	printd("  set irq vector 0x%02x level %d in L2\n",
		ETH29_INT_VEC, ETH29_INT_LEV);

	return(0);
}



/*---------------------------------------------------------------------------*
Function:	endless poll on ETH29 for receive
Parameters:	sock: socket identifier
Input:		socket buffer information
Output:		global receive buffer filled with data
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29_poll_rx(int sock)
{
	int ix, loop_count = 0, buf_count = 0, mbuf_count;	  /* counter */
	struct mbuf *m;						     /* mbuf */
	struct sockbuf *so_rcv;			    /* receive socket buffer */
	u_long sb_cc;		  /* receive socket buffer character counter */
	u_char *src, *dst;		      /* copy source and destination */
	u_char *src_save;				    /* temp save src */
	int m_len, copied_len;	 /* mbuf data length, number of copied bytes */

	/*
	 * look for socket buffer address
	 */
	for(ix = 0; ix < ETH29_MAX_SOCK; ix++) {
		if(sock_a[ix] == sock)
			break;
	}	

	if(ix == ETH29_MAX_SOCK) {
		printd("eth29_poll_rx: invalid sock %d\n", sock);
		return(-1);
	}

	printd("eth29_poll_rx: sock %d so_rcv[%d] 0x%lx\n",
		sock, ix, so_rcv_a[ix]);

	so_rcv = (struct sockbuf *)so_rcv_a[ix];
	ADDR_S2H(so_rcv);

	/*
	 * poll on byte counter of socket buffer
	 */
	while(1) {
		sb_cc = so_rcv->sb_cc;

		/*
		 * check if data available: socket buffer character counter
		 * must be greater than UDP header size and locking flags
		 * must not be set
		 */
		if((sb_cc > 16) && ((so_rcv->sb_flags & (SB_CPY | SB_LOCK)) == 0x0000)) {
			PRINTD("\neth29_poll_rx: start copying %lu bytes\n", sb_cc);

			/*
			 * get and check if mbuf list is already linked
			 * to socket buffer (check if we have mbuf list
			 * to avoid copying only the UDP header: at least
			 * we need one user data mbuf)
			 */
			m = so_rcv->sb_mb;
			if(m == NULL) {
				PRINTD("  buf_count %d: no mbuf\n",
					buf_count);
				continue;
			}

			ADDR_S2H(m);

			if(m->m_next == NULL) {
				PRINTD("  buf_count %d: no mbuf list\n",
					buf_count);
				continue;
			}

			/*
			 * prepare copying data -
			 * use global receive buffer
			 */
			so_rcv->sb_flags |= SB_CPY;
			dst = dg_buf;
			copied_len = 0;
			mbuf_count = 0;

			/*
			 * step through the mbuf list
			 */
			while((u_long)m & ETH29_MPRAM_MASK) {
				/*
				 * get mbuf's data and data length
				 */
				ADDR_S2H(m);
				src = mtod(m, u_char *);
				src_save = src;
				m_len = m->m_len;

				/*
				 * copy data from actual mbuf
				 */
				while(m_len--) {
					*dst++ = *src++;
					copied_len++;
				}

				PRINTD("  m[%d] 0x%lx mlen %d m_next 0x%lx m_act 0x%lx mtod 0x%lx copied %d\n",
					mbuf_count,
					(u_long)m, m->m_len,
					(u_long)m->m_next, (u_long)m->m_act,
					(u_long)src_save,
					copied_len);

				mbuf_count++;

				/*
				 * get next mbuf in list
				 */
				m = m->m_next;
			}

			/*
			 * print copy results
			 */
			PRINTD("  buf_count %d => copied %d from %d mbuf (todo %lu)\n",
				buf_count, copied_len, mbuf_count, sb_cc);
			PRINTB(dg_buf, copied_len);

			/*
			 * check copied data
			 */
			eth29_check_data(dg_buf, buf_count);

			/*
			 * inform ETH29 about copy data finish
			 */
			P_DropData(sock, (caddr_t)so_rcv_a[ix], (int)copied_len);

			buf_count++;

			taskDelay(50);
		}

		/* 
		 * polling count
		 */
		loop_count++;
		if(loop_count % 8000000 == 0)
			printd("\n");
		if(loop_count % 1000000 == 0)
			printd("%d ", buf_count);

	}	/* next poll */

	return(0);
}



/*---------------------------------------------------------------------------*
Function:	check received data
Parameters:	data: data buffer
		data_count: data number counter
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int eth29_check_data(u_char *data, int data_count)
{
	u_short family;					   /* address family */
	u_short port;					      /* port number */
	u_long addr;					       /* ip address */
	int ulen;				/* expected user data length */
	u_char udata;			      /* expected user data contents */
	int i;							  /* counter */
	int error_fp = 0;			     /* family or port error */
	int error_a = 0;				 /* ip address error */
	int error_u = 0;				  /* user data error */
	int result = 0;						   /* result */

	/*
	 * first 16 bytes contain socket address:
	 * address family, port and ip address
	 */
	family = (data[0] << 8) + (data[1]);
	port = (data[2] << 8) + (data[3]);
	addr = (data[4] << 24) + (data[5] << 16) + (data[6] << 8) + (data[7]);

	PRINTD("eth29_check_data(%d): fam %d port %d addr %lu.%lu.%lu.%lu\n",
		data_count, family, port,
		(addr >> 24) & 0xff, (addr >> 16) & 0xff,
		(addr >> 8) & 0xff, addr & 0xff);

	if((family != AF_INET) || (port != 50000)) {
		error_fp++;
	}

	/* 132.147.160.110 */
	if(addr == 0x8493a06e) {
		ulen = 300;
		udata = 0x00;
	}

	/* 132.147.160.101 */
	else if(addr == 0x8493a065) {
		ulen = 42;
		udata = 0x01;
	}

	/* unknown sender */
	else {
		printd("eth29_check_data(%d): %lu.%lu.%lu.%lu unknown\n",
			data_count,
			(addr >> 24) & 0xff, (addr >> 16) & 0xff,
			(addr >> 8) & 0xff, addr & 0xff);
		ulen = 0;
		udata = 0xff;
		error_a++;
	}

	/*
	 * leftover bytes contain user data
	 */
	for(i = 16; i < (ulen + 16); i++) {
		if(data[i] != udata) {
			if(error_u < 1)	   /* print at least once what found */
				printd("(%d) %d: exp 0x%02x found 0x%02x\n",
					data_count, i, udata, data[i]);
			error_u++;
		}
	}

	/*
	 * print result
	 */
	if((error_fp) || (error_a) || (error_u)) {
		printd("ERR (%d) %lu.%lu.%lu.%lu fam/port %d addr %d udata %d\n",
			data_count,
			(addr >> 24) & 0xff, (addr >> 16) & 0xff,
			(addr >> 8) & 0xff, addr & 0xff,
			error_fp, error_a, error_u);
		result = -1;
	}

	data_count++;
	return(result);
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
		printi("[ENOM]\n");
		return;
	}

	/* clear NIF field */
	nif->ActS2H = NULL;
	m_save = NULL;

	/* check command */
	ADDR_S2H(m);
	nifp = mtod(m, struct nifpar *);
	PRINTI("[0x%lx %lu]\n", (u_long)m, nifp->Command);

	switch(nifp->Command) {
		case L2_RCV:
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
				printi("[ET S_COPYIN]\n");
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
				printi("[ET S_COPYIN]\n");
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
				printi("[ET S_COPYIN]\n");
				return;
			}
			nif->ActH2S = m;
			do_mbox(IRQ_SIGNAL);

			break;

		case S_SELWAKEUP:
			break;

		default:
			m_save = m;
			sem_flag = 1;
			break;
	}
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;
	u_long sock = 0;
	//	u_long errno = 0;
	struct socket *psock;

	printd("\nP_Socket: domain %d type %d proto %d pid %d\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	sock = nifp->Status;
	errno = nifp->Errno;
	printd("  answer: sock %lu errno 0x%lx\n",
		sock, errno);
	psock = (struct socket *)nifp->Opt.Par_Socket.psock;
	printd("  psock 0x%lx so_rcv 0x%lx\n",
		(u_long)psock, (u_long)&(psock->so_rcv));
	so_rcv_a[sock] = (u_long)&(psock->so_rcv);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return((int)(errno ? -1 : sock));
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Close: sock %d\n", sock);

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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Shutdown: sock %d how %d\n", sock, how);

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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Bind: sock %d name 0x%lx namelen %d\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Getsockopt: sock %d level 0x%x optname 0x%x optval 0x%lx\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	switch(optname) {
		case SO_RCVBUF:
			printd("  SO_RCVBUF: 0x%lx\n", nifp->Status);
			break;
		default:
			printd("  unknown optname: 0x%lx\n", nifp->Status);
	}

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Setsockopt: sock %d level %d optname %d optval 0x%lx optlen %d\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma;
	struct nifpar *nifp;

	printd("\nP_Ioctl: request: sock %d cmd 0x%lx cmdarg 0x%lx\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	ma = wait_sem(200);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: nifp cmd %lu status 0x%lx errno 0x%lx\n",
		nifp->Command, nifp->Status, nifp->Errno);
	printd("  sock %d cmd 0x%lx cmdarg 0x%lx\n",
		nifp->Opt.Par_Ioctl.socket,
		nifp->Opt.Par_Ioctl.cmd,
		(u_long)nifp->Opt.Par_Ioctl.cmarg);
	PRINTB(nifp->Opt.Par_Ioctl.cmarg, sizeof(struct ifreq));

	/* return mbuf back to slave */
	put_mbuf(ma);

	return(nifp->Errno ? -1 : 0);
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
	struct mbuf *m, *ma = NULL;
	struct nifpar *nifp;
	struct sockaddr_in *sa;			  /* internet socket address */
	int i = 1;

	printd("\nP_Recvfrom: sock %d buf 0x%lx len %d flags 0x%x from 0x%lx\n",
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

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	while(((u_long)ma & ETH29_MPRAM_MASK) == 0) {
		ma = wait_sem(50);
		printd("%s", (i++ % 60) ? "." : "\n.");
	}
	PRINTD(" ma 0x%lx\n", (u_long)ma);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* status value returns length of received buffer */
	PRINTB(buf, nifp->Status);

	sa = from;
	printd("  family 0x%x port %d ip %lu.%lu.%lu.%lu\n",
		sa->sin_family, sa->sin_port,
		(sa->sin_addr.s_addr >> 24) & 0xff,
		(sa->sin_addr.s_addr >> 16) & 0xff,
		(sa->sin_addr.s_addr >> 8) & 0xff,
		sa->sin_addr.s_addr & 0xff);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return((nifp->Errno || (nifp->Status < 0)) ? -1 : 0);
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

int P_Sendto(int sock, caddr_t buf, int len, int flags, caddr_t to, int *tolen)
{
	struct mbuf *m, *ma = NULL;
	struct nifpar *nifp;
	struct sockaddr_in *sa;			  /* internet socket address */
	int i = 1;

	/* get mbuf */
	m = get_mbuf();
	if(m == NULL)
		return(E_MEM);

	/* set up 'Sendto socket' call parameters */
	nifp = (struct nifpar*)loc_m.m_dat;
	nifp->Command = SOC_SENDTO;
	nifp->Command |= (COMPROT_TCPIP << 8);
	nifp->PID = ETH29_PID;
	nifp->Opt.Par_Sendto.socket = sock;
	nifp->Opt.Par_Sendto.buf = buf;
	nifp->Opt.Par_Sendto.len = len;
	nifp->Opt.Par_Sendto.flags = flags;
	nifp->Opt.Par_Sendto.to = to;
	nifp->Opt.Par_Sendto.tolen = tolen;

	/* copy NIF parameter from local mbuf into slave mbuf */
	memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
	m->m_len = sizeof(struct nifpar);

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	while(((u_long)ma & ETH29_MPRAM_MASK) == 0) {
		ma = wait_sem(50);
		printd("%s", (i++ % 60) ? "." : "\n.");
	}
	PRINTD(" ma 0x%lx\n", (u_long)ma);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	/* status value returns length of received buffer */
	PRINTB(buf, nifp->Status);

	sa = to;
	printd("  family 0x%x port %d ip %lu.%lu.%lu.%lu\n",
		sa->sin_family, sa->sin_port,
		(sa->sin_addr.s_addr >> 24) & 0xff,
		(sa->sin_addr.s_addr >> 16) & 0xff,
		(sa->sin_addr.s_addr >> 8) & 0xff,
		sa->sin_addr.s_addr & 0xff);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return((nifp->Errno || (nifp->Status < 0)) ? -1 : 0);
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

int P_Send(int sock, caddr_t buf, int len)
{
	struct mbuf *m, *ma = NULL;
	struct nifpar *nifp;
	int i = 1;

	printd("\nP_Send: sock %d buf 0x%lx len %d\n",
		sock, (u_long)buf, len);

	/* get mbuf */
	m = get_mbuf();
	if(m == NULL)
		return(E_MEM);

	/* set up 'Send socket' call parameters */
	nifp = (struct nifpar*)loc_m.m_dat;
	nifp->Command = SOC_SEND;
	nifp->Command |= (COMPROT_TCPIP << 8);
	nifp->PID = ETH29_PID;
	nifp->Opt.Par_Send.socket = sock;
	nifp->Opt.Par_Send.buf = buf;
	nifp->Opt.Par_Send.len = len;
	nifp->Opt.Par_Send.flags = 0;

	/* copy NIF parameter from local mbuf into slave mbuf */
	memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
	m->m_len = sizeof(struct nifpar);

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	while(((u_long)ma & ETH29_MPRAM_MASK) == 0) {
		ma = wait_sem(50);
		printd("%s", (i++ % 60) ? "." : "\n.");
	}
	PRINTD(" ma 0x%lx\n", (u_long)ma);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
	       nifp->Status, nifp->Errno);

	/* status value returns length of received buffer */
	PRINTB(buf, nifp->Status);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return((nifp->Errno || (nifp->Status < 0)) ? -1 : nifp->Status);
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


/*---------------------------------------------------------------------------*
Function:	Connect to a  socket
Parameters:	sock: socket identifier
		to: socket address of remote socket
		to: lenght of from
Input:		none
Output:		none
Return:		0 => ok
		!0 => error
*----------------------------------------------------------------------------*/

int P_Connect(int sock, caddr_t to, int *tolen)
{
	struct mbuf *m, *ma;
	struct nifpar *nifp;
	struct sockaddr_in *sa;
	int i = 1;

	printd("\nP_Connect: sock %d to 0x%lx\n",
		sock, (u_long)to);

	m = get_mbuf();
	if (m == 0) {
	  return(E_MEM);
	}

	/*
	 * Put connect call parameters to the network interface.
	 */
	nifp = (struct nifpar*)loc_m.m_dat;
	nifp->Command = SOC_CONNECT;
	nifp->Command |= (COMPROT_TCPIP << 8);
	nifp->PID = ETH29_PID;
	nifp->Opt.Par_Connect.socket = sock;
	nifp->Opt.Par_Connect.name = to;
	nifp->Opt.Par_Connect.namelen = tolen;

	/* copy NIF parameter from local mbuf into slave mbuf */
	memcpy(m->m_dat, loc_m.m_dat, sizeof(struct nifpar));
	m->m_len = sizeof(struct nifpar);

	/* handle over command to slave */
	action(m);

	/* wait for slave answer */
	while(((u_long)ma & ETH29_MPRAM_MASK) == 0) {
		ma = wait_sem(50);
		printd("%s", (i++ % 60) ? "." : "\n.");
	}
	PRINTD(" ma 0x%lx\n", (u_long)ma);
	if(((u_long)ma & ETH29_MPRAM_MASK) == 0)
		return(E_TIMEL2);

	nifp = mtod(ma, struct nifpar *);
	printd("  answer: status 0x%lx errno 0x%lx\n",
		nifp->Status, nifp->Errno);

	sa = to;
	printd("  family 0x%x port %d ip %lu.%lu.%lu.%lu\n",
		sa->sin_family, sa->sin_port,
		(sa->sin_addr.s_addr >> 24) & 0xff,
		(sa->sin_addr.s_addr >> 16) & 0xff,
		(sa->sin_addr.s_addr >> 8) & 0xff,
		sa->sin_addr.s_addr & 0xff);

	/* return mbuf back to slave */
	put_mbuf(ma);

	return((nifp->Errno || (nifp->Status < 0)) ? -1 : 0);
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
	ik = intLock();

	while((*addr.mbox_cell != 0) || (nif->mget == NULL)) {
		i++;
		if(i == NIF_MAX_WAIT) {
			intUnlock(ik);
			printd("get_mbuf: ERROR - mbox 0x%02x mget 0x%lx\n",
				*addr.mbox_cell, (u_long)nif->mget);
			return(NULL);
		}
		nif_wait();
	}

	m = nif->mget;
	nif->mget = NULL;
	intUnlock(ik);
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
	ik = intLock();

	while((*addr.mbox_cell != 0) || (nif->pget == NULL)) {
		i++;
		if(i == NIF_MAX_WAIT) {
			intUnlock(ik);
			printd("get_page: ERROR - mbox 0x%02x pget 0x%lx\n",
				*addr.mbox_cell, (u_long)nif->pget);
			return(NULL);
		}
		nif_wait();
	}

	p = (u_char *)nif->pget;
	intUnlock(ik);
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
	ik = intLock();

	while((*addr.mbox_cell != 0) || (nif->mput != NULL)) {
		i++;
		if(i == NIF_MAX_WAIT) {
			intUnlock(ik);
			printd("put_mbuf: ERROR - mbox 0x%02x mput 0x%lx\n",
				*addr.mbox_cell, (u_long)nif->mput);
			return(-1);
		}
		nif_wait();
	}

	nif->mput = m;
	intUnlock(ik);
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
	ik = intLock();

	while((*addr.mbox_cell != 0) || (nif->pput != NULL)) {
		i++;
		if(i == NIF_MAX_WAIT) {
			intUnlock(ik);
			printd("put_page: ERROR - mbox 0x%02x pput 0x%lx\n",
				*addr.mbox_cell, (u_long)nif->pput);
			return(-1);
		}
		nif_wait();
	}

	nif->pput = (struct mbuf *)p;
	intUnlock(ik);
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
	ik = intLock();

	while((*addr.mbox_cell != 0) || (nif->ActH2S != NULL)) {
		i++;
		if(i == NIF_MAX_WAIT) {
			intUnlock(ik);
			printd("action: ERROR - mbox 0x%02x ActH2S 0x%lx\n",
				*addr.mbox_cell, (u_long)nif->ActH2S);
			return(-1);
		}
		nif_wait();
	}

	nif->ActH2S = m;
	intUnlock(ik);
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

	taskDelay(20);

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



/*---------------------------------------------------------------------------*
Function:	wait until semaphore is set in eth29_isr()
Parameters:	timeout: maximum time to wait in 1/100 s
Input:		semaphore
Output:		none
Return:		!NULL => pointer to mbuf saved in eth29_isr()
		NULL => error
*----------------------------------------------------------------------------*/

void *wait_sem(int timeout)
{
	int ik;

	ik = intLock();
	//	while((sem_flag == 0) && (timeout--)) {
	while((sem_flag == 0)) {
		intUnlock(ik);
		//		taskDelay(1);
		ik = intLock();
	}
	if(timeout == 0)
		return(NULL);

	sem_flag = 0;
	intUnlock(ik);

	ADDR_S2H(m_save);
	PRINTD("wait_sem: 0x%lx\n", (u_long)m_save);
	return(m_save);
}


/*****************************************************************************/
/*	miscellaneous							     */
/*****************************************************************************/

/*---------------------------------------------------------------------------*
Function:	vxWorks interrupt print routine
		NOTE:	this routine only maps a variable argument list to
			logMsg()
Parameters:	fmt: format string
		... further arguments to be printed
Input:		format string and parameter to be printed
Output:		printed message
Return:		number of printed characters
*----------------------------------------------------------------------------*/

int printi(const char *fmt, ...)
{
	static char buf[1024];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);

	taskDelay(2);

	return(logMsg("%s", (int)buf,0,0,0,0,0));
}



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

	ik = intLock();
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
	intUnlock(ik);
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


