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
* Module      : neterrno.h
*
* Description : central TCP/IP header file with error values
*
* Author      : UCB /Heiko Koerte
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
* $ProjectName: //NATTDF/natsrc/tcpip/tcpip.pj $
* $ProjectRevision: null $
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
* --------- Revision 1.1  2000/05/15 10:16:34Z  nat
* --------- Initial revision
*
****************************************************************************
* ?????? ul Special adaption for OS-9.
* ?????? ul Initial version, adapted from UCB.
****************************************************************************
***************************************************************************/

#ifndef _NETERRNO_
#define _NETERRNO_

/*
 * Additional error codes
 */

#define	EPERM		0x801		/* Not owner */
#define	ENOENT		0x802		/* No such file or directory */
#define	ESRCH		0x803		/* No such process */
#define	EINTR		0x804		/* Interrupted system call */
/* #define	EIO		5		/* I/O error */
#define	ENXIO		0x806		/* No such device or address */
/* #define	E2BIG		7		/* Arg list too long */
/* #define	ENOEXEC		8		/* Exec format error */
#define	EBADF		0x809		/* Bad file number */
/* #define	ECHILD		10		/* No children */
#define	EAGAIN		0x80b		/* No more processes */
#define	ENOMEM		0x80c		/* Not enough core */
#define	EACCES		0x80d		/* Permission denied */
#define	EFAULT		0x80e		/* Bad address */
/* #define	ENOTBLK		15		/* Block device required */
/* #define	EBUSY		16		/* Mount device busy */
/* #define	EEXIST		17		/* File exists */
/* #define	EXDEV		18		/* Cross-device link */
/* #define	ENODEV		19		/* No such device */
#define	ENOTDIR		0x814		/* Not a directory*/
/* #define	EISDIR		21		/* Is a directory */
#define	EINVAL		0x816		/* Invalid argument */
#define	ENFILE		0x817		/* File table overflow */
#define	EMFILE		0x818		/* Too many open files */
/* #define	ENOTTY		25		/* Not a typewriter */
/* #define	ETXTBSY		26		/* Text file busy */
/* #define	EFBIG		27		/* File too large */
/* #define	ENOSPC		28		/* No space left on device */
/* #define	ESPIPE		29		/* Illegal seek */
/* #define	EROFS		30		/* Read-only file system */
/* #define	EMLINK		31		/* Too many links */
#define	EPIPE		0x820		/* Broken pipe */

/* math software */
/* #define	EDOM		33		/* Argument too large */
/* #define	ERANGE		34		/* Result too large */

/* non-blocking and interrupt i/o */
/* #define	EWOULDBLOCK	35		/* Operation would block */
/* #define	EDEADLK		EWOULDBLOCK	/* ditto */
/* #define	EINPROGRESS	36		/* Operation now in progress */
/* #define	EALREADY	37		/* Operation already in progress */

/* ipc/network software */

	/* argument errors */
/* #define	ENOTSOCK	38		/* Socket operation on non-socket */
/* #define	EDESTADDRREQ	39		/* Destination address required */
/* #define	EMSGSIZE	40		/* Message too long */
/* #define	EPROTOTYPE	41		/* Protocol wrong type for socket */
/* #define	ENOPROTOOPT	42		/* Protocol not available */
/* #define	EPROTONOSUPPORT	43		/* Protocol not supported */
/* #define	ESOCKTNOSUPPORT	44		/* Socket type not supported */
/* #define	EOPNOTSUPP	45		/* Operation not supported on socket */
/* #define	EPFNOSUPPORT	46		/* Protocol family not supported */
/* #define	EAFNOSUPPORT	47		/* Address family not supported by protocol family */
/* #define	EADDRINUSE	48		/* Address already in use */
/* #define	EADDRNOTAVAIL	49		/* Can't assign requested address */

	/* operational errors */
/* #define	ENETDOWN	50		/* Network is down */
/* #define	ENETUNREACH	51		/* Network is unreachable */
/* #define	ENETRESET	52		/* Network dropped connection on reset */
/* #define	ECONNABORTED	53		/* Software caused connection abort */
/* #define	ECONNRESET	54		/* Connection reset by peer */
/* #define	ENOBUFS		55		/* No buffer space available */
/* #define	EISCONN		56		/* Socket is already connected */
/* #define	ENOTCONN	57		/* Socket is not connected */
/* #define	ESHUTDOWN	58		/* Can't send after socket shutdown */
/* #define	ETOOMANYREFS	59		/* Too many references: can't splice */
/* #define	ETIMEDOUT	60		/* Connection timed out */
/* #define	ECONNREFUSED	61		/* Connection refused */

	/* */
/* #define	ELOOP		62		/* Too many levels of symbolic links */
/* #define	ENAMETOOLONG	63		/* File name too long */

/* should be rearranged */
#define	EHOSTDOWN	0x840		/* Host is down */
#define	EHOSTUNREACH	0x841		/* No route to host */
/* #define	ENOTEMPTY	66		/* Directory not empty */

/* quotas & mush */
/* #define	EPROCLIM	67		/* Too many processes */
/* #define	EUSERS		68		/* Too many users */
/* #define	EDQUOT		69		/* Disc quota exceeded */

#define	ENOHNAME	0x846		/* No hostname in socket descriptor */

#endif _NETERRNO_
