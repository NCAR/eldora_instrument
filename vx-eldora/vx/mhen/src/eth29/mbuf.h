/*mbuf.h********************************************************************
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
* Module      : mbuf.h
*
* Description : mbuf memory manager definition file
*
* Author      :
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
* Copyright (c) 1982, 1986, 1988 Regents of the University of California.
* All rights reserved.
*
* Redistribution and use in source and binary forms are permitted
* provided that this notice is preserved and that due credit is given
* to the University of California at Berkeley. The name of the University
* may not be used to endorse or promote products derived from this
* software without specific prior written permission. This software
* is provided ``as is'' without express or implied warranty.
*
****************************************************************************
****************************************************************************
*
* Module's Description Record:
* ============================
*
* $ProjectName: G:/ok1v2/ok1v2.pj $
* $ProjectRevision: 1.5 $
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
* --------- Revision 1.3  2002/06/08 14:39:00Z  helau
* 020608 hl removed reference to m_more()
* --------- Revision 1.2  2001/02/22 13:28:40Z  helau
* 010212 hl increased NMBCLUSTERS to 1024 for ETH29_F
*
****************************************************************************
***************************************************************************/

#ifndef	_MBUF_
#define	_MBUF_

#undef MT_DATA

/*
 * Constants related to memory allocator.
 */
#define NATCL 2048



#define	MSIZE		128			/* size of an mbuf */

#define	MCLBYTES	1536
#define	MCLSHIFT	11
#define	MCLOFSET	(MCLBYTES - 1)


#define	MMINOFF		12			/* mbuf header length */
#define	MTAIL		4
#define	MMAXOFF		(MSIZE-MTAIL)		/* offset where data ends */
#define	MLEN		(MSIZE-MMINOFF-MTAIL)	/* mbuf data length */

#ifdef ETH29_F
#define	NMBCLUSTERS	1024
#else
#define	NMBCLUSTERS	256
#endif

#define	NMBPCL		(NATCL/MSIZE)		/* # mbufs per cluster */

/*
 * Macros for type conversion
 */

/* address in mbuf to mbuf head */
#define	dtom(x)		((struct mbuf *)((int)x & ~(MSIZE-1)))

/* mbuf head, to typed data */
#define	mtod(x,t)	((t)((int)(x) + (x)->m_off))

struct mbuf {
	struct	mbuf *m_next;		/* next buffer in chain */
	ULONG	m_off;			/* offset of data */
	short	m_len;			/* amount of data in this mbuf */
	short	m_type;			/* mbuf type (0 == free) */
	UCHAR	m_dat[MLEN];		/* data storage */
	struct	mbuf *m_act;		/* link in higher-level mbuf list */
};

/* mbuf types */
#define	MT_FREE		0	/* should be on free list */
#define	MT_DATA		1	/* dynamic (data) allocation */
#define	MT_HEADER	2	/* packet header */
#define	MT_SOCKET	3	/* socket structure */
#define	MT_PCB		4	/* protocol control block */
#define	MT_RTABLE	5	/* routing tables */
#define	MT_HTABLE	6	/* IMP host tables */
#define	MT_ATABLE	7	/* address resolution tables */
#define	MT_SONAME	8	/* socket name */
#define	MT_SOOPTS	10	/* socket options */
#define	MT_FTABLE	11	/* fragment reassembly header */
#define	MT_RIGHTS	12	/* access rights */
#define	MT_IFADDR	13	/* interface address */
#define MT_FOREIGN      14      /* mbuf contains a foreign (extern) page */

/* flags to m_get */
#define	M_DONTWAIT	0
#define	M_WAIT		1

/* flags to m_pgalloc */
#define	MPG_MBUFS	0		/* put new mbufs on free list */
#define	MPG_CLUSTERS	1		/* put new clusters on free list */

/* length to m_copy to copy all */
#define	M_COPYALL	1000000000

/*
 * m_pullup will pull up additional length if convenient;
 * should be enough to hold headers of second-level and higher protocols.
 */
#define	MPULL_EXTRA	32

/*
 * Mbuf statistics.
 */
struct mbstat {
	ULONG	m_mbufs;	/* mbufs obtained from page pool */
	ULONG	m_clusters;	/* clusters obtained from page pool */
	ULONG	m_spare;	/* spare field */
	ULONG	m_clfree;	/* free clusters */
	ULONG	m_drops;	/* times failed to find space */
	ULONG	m_wait;		/* times waited for space */
	ULONG	m_drain;	/* times drained protocols for space */
	USHORT	m_mtypes[256];	/* type specific mbuf allocations */
};

/*
 * extern	struct	mbstat mbstat;
 * extern	struct	mbuf *mfree, *mclfree;
 * extern	int	mclnext;
 * extern	int	mb_pages;
 */
extern struct	mbuf *m_get(),*m_getclr(),*m_free(),*m_copy(),*m_pullup();
char *m_clalloc();


#define mtocl(m) ((struct mbuf *)(((ULONG)m) & ~(NATCL-1)))

#define ZEROREFCL(m)  *((int*)((ULONG)mtocl(m) + MCLBYTES-4))=0
#define INCREFCL(m) ++(*((int*)((ULONG)mtocl(m) + MCLBYTES-4)))
#define DECREFCL(m) --(*((int*)((ULONG)mtocl(m) + MCLBYTES-4)))
#define REFCL(m)  *((int*)((ULONG)mtocl(m) + MCLBYTES-4))


#endif	/* _MBUF_ */
