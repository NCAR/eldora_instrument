/***************************************************************************
****************************************************************************
*                                                                          *
*     NNNNNN         N                A             TTTTTTT TTTTTTTTTT     *
*     N NNNNN        N               AAA                  T TTTTT          *
*     NN NNNNN       N              AAAAA                 T TTTTT          *
*     N N NNNNN      N             A AAAAA                T TTTTT          *
*     N  N NNNNN     N            A A AAAAA               T TTTTT          *
*     N   N NNNNN    N           A   A AAAAA              T TTTTT          *
*     N    N NNNNN   N          A     A AAAAA             T TTTTT          *
*     N     N NNNNN  N         AAAAAAAAA AAAAA            T TTTTT          *
*     N      N NNNNN N        A         A AAAAA           T TTTTT          *
*     N       N NNNNNN       A           A AAAAA          T TTTTT          *
*     N        N NNNNN  OO  A             A AAAAA  OO     T TTTTT     OO   *
*                       OO                         OO                 OO   *
*                                                                          *
*     Gesellschaft fuer Netzwerk- und Automatisierungstechnologie m.b.H    *
*        Kamillenweg 22, D-53757 Sankt Augustin, Tel.: ++2241/3989-0       *
*                                                                          *
****************************************************************************
****************************************************************************
*
* Module      : nif.h
*
* Description : Definition of the Network InterFace structure
*
* Author      : H.Laufkoetter
*
****************************************************************************
****************************************************************************
*
*                    Copyright (c) by N.A.T. GmbH
*
*	All rights reserved. Copying, compilation, modification,
*	distribution or any other use whatsoever of this material
*	is strictly prohibited except in accordance with a Software
*	License Agreement with N.A.T. GmbH.
*
****************************************************************************
****************************************************************************
*
* Module's Description Record:
* ============================
*
* $ProjectName: G:/tcpip/tcpip.pj $
* $ProjectRevision: 1.2 $
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
* --------- Revision 1.3  2001/10/15 08:03:24Z  heiko
* 011012 hk Added new member psock to struct Par_Socket as we do return
*		the address of the socket structure (SENSIS problem).
*	    Invented new struct Par_Sock_DropData for NIF (SENSIS problem)
*		and struct nifpar.
* --------- Revision 1.2  2001/02/09 10:41:39Z  heiko
* 010207 hk Renamed struct meminfo members to reveal their real meaning
*		according to a change in ok1.h.
* 001030 hk From AMD29K code: added defintions for L2 modes and L2C_addt,
*		removed defintin for L2_PHYS_ETHID (code now taken by
*		L2_ADDTYPE
* 990503 hk Added new COMPROT_DHCPC for DHCP client
* 990429 hk Added prototype and struct mpd entry for new call L2_RemType().
* 981217 hk Added prototype and struct mpd entry for new call
* 980515 hl Added MULTI_IF to be switched on in all files of L2 which make
*		use of mbuf management in a MP/MultiInterface environment.
* 961030 hk Added COMPROT_ISDN definition
* 960815 hl DECNET action structures added
* 951211 hl Added DMA support
* 951013 hl Added calls and structures for CAM support
* 940815 hk Added support for sendx(), SysReqMem(), SysRetMem()
* 930107 hl Added Multi Protocol Driver definitions
* 920808 hl Added level in L2M_init_p for VLAN module
* 910524 hl Initial version
****************************************************************************
***************************************************************************/

#ifndef __NIF__
#define __NIF__

typedef volatile struct nif_struct {
	void	*mget;
	void	*mput;
	void	*ActH2S;
	void	*ActS2H;
	void	*pget;
	void	*pput;
} NIF;

/*------------ Multi Protocoll Driver entry points definition --------------*/
#ifndef SMARTBOARD
typedef struct mpd {
	long 	a6stat,
		(*L2_Init)(),
		(*L2_Stop)(),
		(*L2_Send)(),
		(*L2_Send_fb)(),
		(*L2_Putnet)(),
		(*L2_Receive)(),
		(*L2_AttProto)(),
		(*L2_AttPort)(),
		(*L2_AddType)(),
		(*L2_RemType)(),
		(*L2_AddRDispID)(),
		(*L2_Detach)(),
		(*L2_SetDebug)(),
		(*L2_GetHardwareType)(),
		(*get_boardID)(),
		(*m_cat)(),
		(*m_copy)(),
		(*m_copydata)(),
		(*m_pullup)(),
		(*m_adj)(),
		(*m_get)(),
		(*m_getclr)(),
		(*m_clget)(),
		(*m_free)(),
		(*m_freem)(),
		(*m_getpag)(),
		(*m_putpag)(),
		(*get_log_boardID)(),
		(*m_clfree)();		/* hk 23.12.93 wasn't here before */
	} MPMOD;

#if defined MP || defined MULTI_IF
#ifndef USE_FUNCTION_LABELS
#define L2_Init(p1) 		callex(mpd->L2_Init,mpd->a6stat,p1)
#define L2_Stop()		callex(mpd->L2_Stop,mpd->a6stat)
#define L2_AttProto(p1,p2,p3,p4,p5) \
 				callex(mpd->L2_AttProto,mpd->a6stat,p1,p2,p3,p4,p5)
#define L2_AttPort(p1,p2,p3,p4,p5) \
 				callex(mpd->L2_AttPort,mpd->a6stat,p1,p2,p3,p4,p5)
#define L2_AddType(p1,p2)	callex(mpd->L2_AddType,mpd->a6stat,p1,p2)
#define L2_AddRDispID(p1,p2,p3)	callex(mpd->L2_AddRDispID,mpd->a6stat,p1,p2,p3)
#define L2_Detach(p1)		callex(mpd->L2_Detach,mpd->a6stat,p1)
#define L2_Send(p1,p2,p3,p4)	callex(mpd->L2_Send,mpd->a6stat,p1,p2,p3,p4)
#define L2_Send_fb(p1,p2,p3,p4)	callex(mpd->L2_Send_fb,mpd->a6stat,p1,p2,p3,p4)
#define L2MB_putnet(p1)		callex(mpd->L2_Putnet,mpd->a6stat,p1)
#define L2_Receive(p1)		callex(mpd->L2_Receive,mpd->a6stat,p1)
#define L2_SetDebug(p1)		callex(mpd->L2_SetDebug,mpd->a6stat,p1)
#define get_boardID(p1)		callex(mpd->get_boardID,mpd->a6stat,p1)
#define get_log_boardID(p1)	callex(mpd->get_log_boardID,mpd->a6stat,p1)
#endif


#define m_cat(p1,p2)		callex(mpd->m_cat,mpd->a6stat,p1,p2)
#define m_copy(p1,p2,p3)	((struct mbuf *)callex(mpd->m_copy,mpd->a6stat,p1,p2,p3))
#define m_copydata(p1,p2,p3,p4)	callex(mpd->m_copydata,mpd->a6stat,p1,p2,p3,p4)
#define m_pullup(p1,p2)		callex(mpd->m_pullup,mpd->a6stat,p1,p2)
#define m_adj(p1,p2)		callex(mpd->m_adj,mpd->a6stat,p1,p2)
#define m_get(p1,p2)		((struct mbuf*)callex(mpd->m_get,mpd->a6stat,p1,p2))
#define m_getclr(p1,p2)		((struct mbuf*)callex(mpd->m_getclr,mpd->a6stat,p1,p2))
#define m_clget(p1)		callex(mpd->m_clget,mpd->a6stat,p1)
#define m_clfree(p1)		callex(mpd->m_clfree,mpd->a6stat,p1)
#define m_free(p1)		((struct mbuf*)callex(mpd->m_free,mpd->a6stat,p1))
#define m_freem(p1)		callex(mpd->m_freem,mpd->a6stat,p1)
#define m_getpag()		callex(mpd->m_getpag,mpd->a6stat)
#define m_putpag(p1)		callex(mpd->m_putpag,mpd->a6stat,p1)

/*------- multi interface entry calls -------------------------------------*/

#define L2_Init_if(if,p1) 	callex((mpd+if)->L2_Init,(mpd+if)->a6stat,p1)
#define L2_Stop_if(if)		callex((mpd+if)->L2_Stop,(mpd+if)->a6stat)
#define L2_AttProto_if(if,p1,p2,p3,p4,p5) \
				callex((mpd+if)->L2_AttProto,(mpd+if)->a6stat,p1,p2,p3,p4,p5)
#define L2_AttPort_if(if,p1,p2,p3,p4,p5) \
				callex((mpd+if)->L2_AttPort,(mpd+if)->a6stat,p1,p2,p3,p4,p5)
#define L2_AddType_if(if,p1,p2)	callex((mpd+if)->L2_AddType,(mpd+if)->a6stat,p1,p2)
#define L2_RemType_if(if,p1,p2)	callex((mpd+if)->L2_RemType,(mpd+if)->a6stat,p1,p2)
#define L2_AddRDispID_if(if,p1,p2,p3)	callex((mpd+(if))->L2_AddRDispID,(mpd+(if))->a6stat,p1,p2,p3)
#define L2_Detach_if(if,p1)		callex((mpd+if)->L2_Detach,(mpd+if)->a6stat,p1)
#define L2_Send_if(if,p1,p2,p3,p4)	callex((mpd+if)->L2_Send,(mpd+if)->a6stat,p1,p2,p3,p4)
#define L2_Send_fb_if(if,p1,p2,p3,p4)	callex((mpd+if)->L2_Send_fb,(mpd+if)->a6stat,p1,p2,p3,p4)
#define L2MB_putnet_if(if,p1)		callex((mpd+if)->L2_Putnet,(mpd+if)->a6stat,p1)
#define L2_Receive_if(if,p1)		callex((mpd+if)->L2_Receive,(mpd+if)->a6stat,p1)
#define L2_SetDebug_if(if,p1)		callex((mpd+(if))->L2_SetDebug,(mpd+(if))->a6stat,p1)
#define L2_GetHardwareType_if(if)	callex((mpd+if)->L2_GetHardwareType,(mpd+if)->a6stat)
#define get_boardID_if(if,p1)		callex((mpd+(if))->get_boardID,(mpd+if)->a6stat,p1)
#define get_log_boardID_if(if,p1)	callex((mpd+(if))->get_log_boardID,(mpd+if)->a6stat,p1)

extern int callex();
extern MPMOD *mpd;

#endif	/* MP */
#endif /* SMARTBOARD */



/*------------ LAYER 2 - Action Structures ---------------------------------*/
/*
 * mode bit definitions
 */
#define L2_MODE_PROM		0x8000  /* Promiscous rx (all frames)   */
#define L2_MODE_MULTI		0x4000  /* Receive all Multicast frames */
#define L2_USE_DMA		0x2000	/* use DMA for copy operations  */
#define L2_RCV_CHAIN		0x0100  /* use receive buffer chaining  */
#define L2_MODE_FCT		0x0080	/* set SAT bit for BSI RCHN1    */
#define L2_MODE_SAT		0x0040	/* set SAT bit for BSI RCHN1    */
#define L2_MODE_VST		0x0020	/* set VST bit for BSI RCHN1    */
#define L2_MODE_BOSEL		0x0010	/* set BOSEL bit in MAC         */
#define L2_DMA_ALLIGN_QUAD	0x0010	/* align foreign address on quad*/
					/* word boundary		*/
#define L2_DMA_NOMIN		0x0008	/* use DMA mode MBLT		*/
#define L2_DMA_NOREL		0x0004	/* no VMEbus release during DMA	*/
#define L2_DMA_MBLT		0x0002	/* use DMA mode MBLT		*/
#define L2_DMA_BLT		0x0001	/* use DMA mode BLT		*/

struct L2M_init_p {
	u_short	L_mode;			/* Lance operation mode             */
	u_char	my_addr[6];		/* my own ETHERNET ID, overrides the*/
					/* ONBOARD Ethernet ID              */
	u_char	**multi_addr;		/* a pointer to a list of Multicast */
					/* addresses                        */
	u_long	License;		/* the License number of the board  */
	u_char	vector;			/* Bus Interrupt vector             */
	u_char  level;			/* Bus Interrupt Level (for VLAN only*/
	u_short dma_modes;		/* operational modes of opt VME dma */
	u_long	a32_base;		/* base address of master board     */
	u_long  a64_base;		/* hihg lword of master base if A64 */
};

struct L2_attproto {
	u_long	proto;			/* protocol-id to be attached        */
	long	(*ifcall)();		/* if-function, to be called from L2 */
	long	a6stat;			/* static storage ptr for 680xx machines */
};

struct L2C_attp_p {			/* Attach Port Structure            */
	u_long	port;			/* the number of the port           */
#ifdef	TRANSPUTER
	long	ifcall;			/* dummy */
	long	dispcall;		/* dummy */
#else	/* not TRANSPUTER */
	long	(*ifcall)();		/* if-function, to be called from L2 */
	long	(*dispcall)();		/* dispatcher-function, to be called from L2 */
#endif /* not TRANSPUTER */
	long	a6stat;			/* static storage ptr for 680xx machines */
	u_short	type;			/* the Ethernet type to be assigned to*/
};

struct L2C_addt {			/* add Ethernet Type to port structure*/
	u_long	port;			/* the number of the port           */
	u_short type;			/* EThernet type		    */
};

struct L2_rcv {
	UCHAR *addr;
	ULONG len;
	ULONG (*rcv_func)();
	UCHAR *a6stat;
};

struct L2_send {
	UCHAR *addr;
	ULONG len;
	UCHAR ethid[6];
	USHORT type;
};

struct L2_sendm {
	struct mbuf * m;
};

struct L2C_addvect {
	u_long	port;			/* the number of the port */
	u_char	board_vector;		/* IRQ vector to use for the port */
	u_char	irq_level;		/* IRQ level to use for the port */
};

struct L2_camcmd {
	u_short commands[42];
	u_short results[6];
};

struct dma_list {
	UCHAR *addr;
	ULONG len;
};

#define MAX_DMA_LIST 	(11)
#define MAX_S2H_BYTES   160000  /* max. byte count of L2_rcv frames which can */
				/* be stored on the slave */
#define MAX_S2H_FRAMES  320     /* max. no. of frames in the queue */
#define MAX_RCV_CHAIN   8       /* max no of ActS2H which might be chained */

struct L2_send_dma {
	struct dma_list fragments[1];
};

struct L2_add_dma_buf {
	struct dma_list buflist[1];
};

struct L2_rcv_dma {
	u_long	frameLen;
	u_long	dma_space_left;
	struct dma_list fragments[1];
};

struct L2_diag {
	u_long diag_command;
	u_long par1;
	u_long par2;
	u_long par3;
	u_long par4;
};

/*
 * DIAGNOSTIC Subcommands
 */
#define L2_DIAG_DMA_READ	1	/* DMA Read Test */
#define L2_DIAG_DMA_WRITE	2	/* DMA Write Test */
#define L2_DIAG_DMA_RW		3	/* DMA Read/Write Test */

struct meminfo {
	ULONG	com,
		mbuf,
		page,
		sys;
};

struct taskinfo {
	USHORT	id,
		state;
	ULONG	pc;
};

/*---------- TCP/IP - Action Structures -----------------------------------*/

typedef struct BCB {
	u_char	*b_addr;
	short	 b_len;
	short	 b_msglen;
} BCB;

struct S_CopyData {                     /* Copy Data Action */
	u_char	*Cpto;
	u_char	*Cpfrom;
	long	Cplen;
	u_short Cppid;			/* procID of initiating host process */
};

struct S_Selwakeup {                   /* Select-Wake-up-Call Action */
	caddr_t	procd;
	int	socket;
	int	which;
};

struct Par_Socket {                    /* Socket-Call Action */
	int	dom;
	int	type;
	int	proto;
	u_long	uid;
	int	psock;		       /* NAT011012hk */
};

struct Par_Bind {                      /* Bind-Call Action */
	int	socket;
	caddr_t	name;
	int	namelen;
};

struct Par_Listen {                    /* Listen-Call Action */
	int	socket;
	int	backlog;
};

struct Par_Accept {                    /* Accept-Call Action */
	int	socket;
	caddr_t	name;
	int	*anamelen;
};

struct Par_Connect {                   /* Connect-Call Action */
	int	socket;
	caddr_t	name;
	int	namelen;
};

struct Par_Sendto {                   /* Sendto-Call Action */
	int	socket;
	caddr_t	buf;
	int	len;
	int	flags;
	caddr_t	to;
	int	tolen;
};

struct Par_Send {                     /* Send-Call Action */
	int	socket;
	caddr_t	buf;
	int	len;
	int	flags;
};

/* start of changes made for SIAN_ABC */
struct Par_Sendx {                     /* Send-Call Action */
	int	socket;		       /* special for H.SIAN, ABC */
	caddr_t	sbuf;                  /* ptr to slave local memory */
	caddr_t	lbuf;                  /* ptr to master local memory */
	int	len;
	int	flags;
};

struct sysreqmem {
	ULONG memsize;
	UCHAR *memadr;		/* actually contains the address */
};

struct sysretmem {
	ULONG memsize;
	UCHAR *memadr;
};
/* end of changes made for SIAN_ABC */

struct Par_Sendmsg {                  /* Sendmsg-Call Action */
	int	socket;
	caddr_t	msg;
	int	flags;
};

struct Par_Recvfrom {                 /* Receive-From-Call Action */
	int	socket;
	caddr_t	buf;
	int	len;
	int	flags;
	caddr_t	from;
	int	*fromlenaddr;
};

struct Par_Recv {                     /* Receive-Call Action */
	int	socket;
	caddr_t	buf;
	int	len;
	int	flags;
};

struct Par_Recvmsg {                  /* Receive-Message-Call Action */
	int	socket;
	caddr_t	msg;
	int	flags;
};

struct Par_Shutdown {                 /* Shutdown-Call Action */
	int	socket;
	int	how;
};

struct Par_Setsockopt {               /* Set-Socket-Option-Call Action */
	int	socket;
	int	level;
	int	name;
	caddr_t	val;
	int	valsize;
};

struct Par_Getsockopt {               /* Get_Socket-Option-Call Action */
	int	socket;
	int	level;
	int	name;
	caddr_t	val;
	int	*avalsize;
};

struct Par_Getsockname {              /* Get-Socket-Name-Call Action */
	int	socket;
	caddr_t	asa;
	int	*alen;
};

struct Par_Getpeername {              /* Getpeername-Call Action */
	int	socket;
	caddr_t	asa;
	int	*alen;
};

struct Par_Ioctl {                    /* I/O-Control-Call Action */
	int	socket;
	u_long	cmd;
	caddr_t	cmarg;
};

struct Par_Close {                    /* Socket-Close-Call Action */
	int	socket;
};

struct Par_Select {                   /* Select-Call Action */
	int	socket;
	caddr_t	procd;
	int	flag;
	int	clear_or_set;
};

struct Par_Getstataddr {              /* Get-Statistic-information-addr-Call*/
	int	socket;
	int	type;
};

struct Par_set_dbgopt {		     /* used for ISO/OSI */
	int sock;
	u_char dbg_opt;
	u_char dbg_cmd;
};

struct Par_Sock_DropData {		     /* Drop Data from Socket Buffer */
	u_long psockbuf;
	u_long data_len;
};


/*
 * DECNET action structures
 */
struct Par_L5_Init{		/* This function allocates and initializes all memory areas.*/
	char 	*NodeMod;	/* pointer to nodes table module       */
	USHORT	pad1;		/* BYTE padding to avoid alignment problems */
	USHORT  NodeID;		/* my own node ID                      */
	char    *PassWMod;	/* pointer to password module          */
	ULONG	PassWSz;    	/* size of password module             */
	int 	HelloTime;	/* Hello Time in seconds               */
};
struct Par_L5_Stop{          /* This function stops DECNET operation         */
	int dummy;
};


struct Par_L5_Open{            /* This function allocates a port in NSP for  */
                               /* receiving a logical link connect request.  */
	int Port;              /* returned new port ID                       */
	unsigned char pad1;    /* BYTE padding to avoid alignment problems   */
	unsigned char pad2;
	unsigned char pad3;
	unsigned char Objecttyp; /* DECNet Object type (L5) for connection   */
	unsigned int  groupid;   /* returned group/user Id of an             */
};                               /* remote user                              */

struct Par_L5_Connect{		/* This function allocates a port and */
                                /* requests a logical link connection.*/
	int   Port;       	/* where to store the allocated port ID   */
	UCHAR 	pad1,		/* BYTE padding to avoid alignment problems */
		pad2,
		pad3;
	UCHAR Objecttyp;        /* Object number of the remote server     */
	ULONG groupid; 		/* users group/user ID                    */
	char  NodeName[80];     /* destination node name string ptr       */
};

struct Par_L5_Accept{		/* Accept a logical link connection*/
	int Port;
};
struct Par_L5_Reject{		/* Reject a logical link connection*/
	int Port;
	USHORT	pad1;
	USHORT Reason;
	int Size;
	UCHAR Buffer[80];	/* buffer for additional reject info */
};

struct Par_L5_Disconnect{	/* This function initiates the disconnection of */
				/* a logical link that is in running state.     */
	int Port;		/* users Port ID                                */
};

struct Par_L5_Abort{         /* This function aborts the current operation */
                             /* used to abort functions which are in       */
                             /* "sleep" state.                             */
	int Port;            /* Users Port ID                         */
};

struct Par_L5_Write{		/* This function queues a transmit buffer to a port */
   				/* for transmitting normal data on a logical link.  */
	int Port;          	/* Users Port ID                         */
	unsigned char *Buffer;  /* Pointer to users data                 */
	int Size;               /* User data size                        */
};
struct Par_L5_Read{		/* This function queues a receive buffer to  */
				/* a port to receive normal data.            */
	int Port;          	/* users Port ID                             */
	UCHAR *Buffer;		/* where to store the received data          */
	int Size;		/* returned size of the received data*/
};
struct Par_L5_Ready{		/* test if a frame is available */
	int Port;          	/* Users Port ID                   */
	int Count;     	        /* available frame count */
};
struct Par_L5_Signal{		/* set up Signal for Process  */
	int Port;
	USHORT  pad1;
	USHORT Signal;
};

struct Par_L5_ClrSgn{		/* clr wakeup Signal for Process  */
	int Port;
};

struct Par_L5_SetDebug{      /* set LAYER 5 debug Level	   */
	int Level;
};
struct Par_L5_AppBuf{		/* request/return memory space */
	UCHAR *Addr;		/* for application buffer */
	int Size;
};


/*---------- the common structure of any Action via NIF ------------------*/

#define NIFHEADER 16			/* size of common header 	*/

struct nifpar {
	u_long	Command;
	u_long	Status;
	u_long	Errno;
	u_short	PID;
	u_short	pad;			/* align on 4 byte boundary */
	union {
		struct	L2M_init_p init;
		struct	L2C_attp_p attp;
		struct	L2C_addt addt;
		struct	L2C_addvect addvect;
		struct  L2_send L2_send;
/*		struct	L2_rcv L2_rcv;			not used anymore !? */
/*		struct	L2_attproto L2_attproto;	not used anymore !? */
		struct  L2_sendm L2_sendm;
		struct  L2_camcmd L2_camcmd;
		struct	L2_send_dma L2_send_dma;
		struct	L2_add_dma_buf L2_add_dma_buf;
		struct	L2_rcv_dma L2_rcv_dma;
		struct 	L2_diag L2_diag;
		struct	BCB bcb;
		struct	S_CopyData cpdat;
		struct	S_Selwakeup S_Selwakeup;
		struct	Par_Socket Par_Socket;
		struct	Par_Bind Par_Bind;
		struct	Par_Listen Par_Listen;
		struct	Par_Accept Par_Accept;
		struct	Par_Connect Par_Connect;
		struct	Par_Sendto Par_Sendto;
		struct	Par_Send Par_Send;
		struct	Par_Sendmsg Par_Sendmsg;
		struct	Par_Recvfrom Par_Recvfrom;
		struct	Par_Recv Par_Recv;
		struct	Par_Recvmsg Par_Recvmsg;
		struct	Par_Shutdown Par_Shutdown;
		struct	Par_Setsockopt Par_Setsockopt;
		struct	Par_Getsockopt Par_Getsockopt;
		struct	Par_Getsockname Par_Getsockname;
		struct	Par_Getpeername Par_Getpeername;
		struct	Par_Ioctl Par_Ioctl;
		struct	Par_Close Par_Close;
		struct	Par_Select Par_Select;
		struct	Par_Getstataddr Par_Getstataddr;
		u_char	ethid[6];
		u_long	debuglevel;
		struct meminfo meminfo;
		struct taskinfo *taskinfo;
		u_long	license;
		struct sysreqmem sysreqmem;	/* specially for SIAN_ABC */
		struct sysretmem sysretmem;	/* specially for SIAN_ABC */
		struct Par_Sendx Par_Sendx;	/* specially for SIAN_ABC */
		struct Par_set_dbgopt Par_set_dbgopt; /* ISO/OSI dbg opt  */
		struct Par_Sock_DropData Par_Sock_DropData;  /* NAT011012hk */
		struct  Par_L5_Init Par_L5_Init;
		struct Par_L5_Stop Par_L5_Stop;
		struct Par_L5_Open Par_L5_Open;
		struct Par_L5_Connect Par_L5_Connect;
		struct Par_L5_Accept Par_L5_Accept;
		struct Par_L5_Reject Par_L5_Reject;
		struct Par_L5_Disconnect Par_L5_Disconnect;
		struct Par_L5_Abort Par_L5_Abort;
		struct Par_L5_Write Par_L5_Write;
		struct Par_L5_Read Par_L5_Read;
		struct Par_L5_Ready Par_L5_Ready;
		struct Par_L5_Signal Par_L5_Signal;
		struct Par_L5_ClrSgn Par_L5_ClrSgn;
		struct Par_L5_SetDebug Par_L5_SetDebug;
		struct Par_L5_AppBuf Par_L5_AppBuf;
       } Opt;
};

/*-------- nif - COMMANDS ----------------------------------------------*/
/*
 * Commands frome the masterboard to the slaveboard.
 */
#define L2_INIT			1	/* L2 - Init			*/
#define L2_ATTP			2	/* L2 - Attach Port		*/
#define L2_SEND			3	/* L2 - Send Data		*/
#define L2_ETHID		4	/* get the boards Ethernet ID   */
#define L2_DEBUG		5	/* set layer 2 debug level      */
#define L2_DETACH		6	/* detach a port from operation */
#define L2_STOP			7	/* stop Layer 2 operation       */
#define L2_VECTOR		8	/* send IRQ vector and IRQ level*/
					/* for a specified protocol to  */
					/* Layer 2			*/
#define L2_ADDTYPE		9	/* add type to recognice to a port */
#define L2_LOG_ETHID		10	/* get the boards logical	*/
					/* Ethernet ID   		*/

#define SOC_INIT		11	/* Initialize Network Software	*/
#define SOC_SOCKET		12	/* Socket call			*/
#define SOC_BIND		13	/* Bind call			*/
#define SOC_LISTEN		14	/* Listen call			*/
#define SOC_ACCEPT		15	/* Accept call			*/
#define SOC_CONNECT		16	/* Connect call			*/
#define SOC_SENDTO		17	/* Sendto call			*/
#define SOC_SEND		18	/* Send call			*/
#define SOC_SENDMSG		19	/* Sendmsg call			*/
#define SOC_RECVFROM		20	/* Recvfrom call		*/
#define SOC_RECV		21	/* Recv call			*/
#define SOC_RECVMSG		22	/* Recvmsg call			*/
#define SOC_SHUTDOWN		23	/* Shutdown call		*/
#define SOC_GETSOCKOPT		24	/* Getsockopt call		*/
#define SOC_SETSOCKOPT		25	/* Setsockopt call		*/
#define SOC_GETSOCKNAME		26	/* Getsockname call		*/
#define SOC_GETPEERNAME		27	/* Getpeername call		*/
#define SOC_IOCTL		28	/* Ioctl call for sockets	*/
#define SOC_CLOSE		29	/* Close call for sockets	*/
#define SOC_SELECT		30	/* Select call for sockets	*/
#define SOC_GETSTATDATA		31	/* Call for netstat utility	*/
#define SOC_GETSTATADDR		32	/* Call for netstat utility	*/
#define SOC_GETSTAT_AT		33	/* Call for netstat utility	*/
#define SOC_TEST_LICENSE	34	/* Test license number		*/
#define MEMINFO			35	/* get Kernel Memory info       */
#define	GET_LICENSE		36	/* get license number		*/
#define L2_ATTPROTO		37	/* attach protocol		*/
#define TASKINFO		38	/* get Kernel Process info	*/
#define SYS_REQMEM		39	/* get system memory		*/
#define SYS_RETMEM		40	/* return allocated  system mem */
#define SOC_SENDX		41	/* special send call		*/
#define SOC_SET_DBGOPT		42	/* set/del ISO/OSI dbg options  */
#define SOC_DROPDATA		49	/* drop data from socket buffer */

#define L2_CAMCMD		43	/* execute CAM commands         */
#define L2_CAMCMD_NOIRQ		44	/* execute CAM commands, no IRQ */
#define L2_SENDM		45	/* L2-Send Data in a mbuf chain	*/
#define L2_SEND_DMA		46	/* L2-Send with DMA support     */
#define L2_ADD_DMA_BUF		47	/* Suply a receive Buffer List  */
#define L2_DIAG			48	/* Layer 2 Diagnostic functions */

/*
 * Commands from the slaveboard to the masterboard.
 */
#define L2_RCV			101	/* L2 S2H - receive data        */
#define	S_COPYIN		102	/* Copy data from masterboard	*/
					/* to slaveboard		*/
#define	S_COPYOUT		103	/* Copy data from slaveboard	*/
					/* to masterboard		*/
#define S_SELWAKEUP		104	/* Wakeup for select()		*/
#define	S_COPYOUT_M		105	/* Copy data in mbuf chain from */
					/* slaveboard to masterboard	*/
#define L2_RCV_DMA		106	/* L2 S2H - receive with DMA    */

/*
 * DECNET Commands
 */
#define L5_INIT		200
#define L5_STOP		201
#define L5_OPEN		202
#define L5_CONNECT	203
#define L5_ACCEPT	204
#define L5_REJECT	205
#define L5_DISCONNECT	206
#define L5_ABORT	207
#define L5_WRITE	208
#define L5_READ		209
#define L5_READY	210
#define L5_SIGNAL	211
#define L5_CLRSIG	212
#define L5_SETDEBUG	213
#define L5_RQAPPBUF	214
#define L5_RTAPPBUF	215

/*-------- MBox-IRQ - Codes  -------------------------------------------*/

#define IRQ_MGET		1	/* get a mbuf from slave 	*/
#define IRQ_MPUT		2       /* put mbuf back to slave 	*/
#define IRQ_ActH2S		3       /* Action-Host-to-Slave 	*/
#define IRQ_ActS2H		4       /* Action-Slave-to-Host 	*/
#define IRQ_PGET		5       /* put page 			*/
#define IRQ_PPUT		6       /* put page 			*/
#define IRQ_SNDRAW		7       /* send raw data 		*/
#define IRQ_RCVRAW		8       /* receive raw data 		*/
#define IRQ_SIGNAL		9       /* wake up slave process 	*/
#define	IRQ_RTCOPLEN		10	/* return copied data length	*/

/*-------- Protocol numbers for commands to the slaveboard -------------*/
#define COMPROT_L2		0	/* all L2 primitives */
#define	COMPROT_OS9		4	/* Protocol = OS9Net		*/
#define	COMPROT_TCPIP		5	/* Protocol = TCP/IP		*/
#define	COMPROT_DECNET		6	/* Protocol = DECNet		*/
#define	COMPROT_ISO		7	/* Protocol = ISO/TP		*/
#define	COMPROT_CNAPS		8	/* Protocol = private, CNAPS	*/
#define COMPROT_TOKEN		9	/* Protocol = IBM TOKEN RING	*/
#define COMPROT_ISDN		10	/* Protocol = ISDN	        */
#define COMPROT_DHCPC		11	/* Protocol = DHCP (client)     */

#endif       /* __NIF__ */
