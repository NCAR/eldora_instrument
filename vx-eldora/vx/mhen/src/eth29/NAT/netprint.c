/*netprint.c****************************************************************
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
*        Goethestr. 2, D-53844 Troisdorf-Sieglar, Tel.: ++2241/408296      *
*                                                                          *
*--------------------------------------------------------------------------*
*                                                                          *
* Module:	netprint.c						   *
*                                                                          *
* Author: 	Helmut Laufkoetter					   *
*                                                                          *
* Rev.  : 	02		Date of creation: ??.??.??		   *
*                                                                          *
*--------------------------------------------------------------------------*
*                                                                          *
* Module description:                                                      *
*	N.A.T. Network debug buffer print utility			   *
*                                                                          *
* Functions:                                                               *
*                                                                          *
*                                                                          *
*--------------------------------------------------------------------------*
*                                                                          *
* Revision History:                                                        *
*									   *
* #  Date	Description					       who *
*--------------------------------------------------------------------------*
* 01 ??.??.??	initial version						hl *
* 02 01.03.94	Help output changed.					ul *
* 03 01.02.97   Each time before opening the device /sock we check if   hk *
*		we could open device /nsock instead. If opening /sock      *
*		fails as well we retur an error. This was done to          *
*		enable a user to use an alternative name "nsock" for       *
*		the socket i.e. because the name "sock" for the N.A.T.     *
*		descriptor is causing name clashes with the descriptor     *
*		for Microware's ISP if it used in parallel to the N.A.T.   *
*		TCP/IP.							   *
****************************************************************************
***************************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <natdefs.h>
#include <errno.h>
#include <modes.h>
#include "/h4/helmut/src/defs/dec.h"
#include "/w4/natsrc/tcpip/include/sys/sockcons.h"

#define std_slavebase 0x801000
#define std_spoolsize 0x3000


#asm
*-----------------------------------------------------------------*
* function putsock : execution of a User call via                 *
*                    OS9-PutStt Routine                           *
*       Parameter  : (Path,Functioncode,PAR1,PAR2,...,PARn        *
*       returns    : Return value in d0, -1 if error              *
*-----------------------------------------------------------------*
putsock:
        movem.l d2-d7,-(sp)
        move.l  28(sp),d2
        move.l  32(sp),d3
        move.l  36(sp),d4
        move.l  40(sp),d5
        move.l  44(sp),d6
        move.l  48(sp),d7
        OS9     I$SetStt
        bcs.s   puts_err
        move.l  #0,errno(a6)
        move.l  d1,d0                   | return value
        movem.l (sp)+,d2-d7
        rts
puts_err:
        move.l  d1,errno(a6)
        moveq.l #-1,d0
        movem.l (sp)+,d2-d7
        rts

#endasm

@_sysedit:	equ	2
#define	EDITION		2

/*
 * print out at system state
 * requires the following
 */
int outpath,inpath,errpath;
UCHAR sbuf[256];

savhex (cp,lp)
	char *cp;
	LONG *lp;
{
	char *cp2;

	cp2 = cp;
	while ( *cp )
		{
		if (!isxdigit (*cp++))
			{
			return (0);
			}
		}
	sscanf (cp2,"%X",lp);
	return (1);
}

void displ (myname)
	char *myname;
{
	sprintf(sbuf,
		"---------- N.A.T. NETPRINT-Utility ----------\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"Copyright: N.A.T. GmbH\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"Syntax   : %s [<opts>] [/<network>]\n", myname);
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"Function : print network debug information\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"Options  :\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"    -a=<address>   base address (hex) of debug buffer\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   (default = $801000)\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"    -s=<size>      size (hex) of debug buffer (default = $3000)\n\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"    <network>      may be one of the following:\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   dec  for DECNet\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   n0   for OS9Net\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   tcp  for TCP/IP\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   iso  for ISO\n");
	write(outpath,sbuf,strlen(sbuf));
	sprintf(sbuf,
		"                   mp   for Debug statements from MP driver\n");
	write(outpath,sbuf,strlen(sbuf));
	return;
}

unsigned char buffer[0x3000];

main(argc,argv)
	int argc;
	char *argv[];
{
	register UCHAR *i;
	register UCHAR *bp;
	ULONG slavebase;
	ULONG spoolsize;
	register short scnt;
	register short acnt;
	register short i1;
	unsigned char space_skip;
	char devname[32];
	int error;
	int path;
	BOOL localbuf;

	inpath=0;		/* my standard Input/Output path numbers */
	outpath=1;

	/* 
	 * the following code is required for OS9 only
	 * when running in system state.
	 * It maps the standard in/out path to the
	 * system path numbers
         */
#ifdef __OS9__ 
 	_get_process_desc(getpid(),sizeof(procid),&pbuf);
 	if(pbuf._pmodul->_mh._mattrev & 0x2000) {
		inpath=pbuf._path[0];		/* get system path no. */
		outpath=pbuf._path[1];		/* get system path no. */
		errpath=pbuf._path[2];		/* get system path no. */
		inpath=dup(inpath);
		outpath=dup(outpath);
		errpath=dup(errpath);
  	}
#endif
	sprintf(sbuf, "netprint: localbuffer AT:0x%x with size=0x%x\n",
		&buffer[0],0x3000);
	write(outpath,sbuf,strlen(sbuf));

	slavebase = (ULONG)(std_slavebase);
	spoolsize = (ULONG)(std_spoolsize);
	scnt = 1;
	acnt = 1;
	localbuf=FALSE;
	for (i1=1;i1<argc;i1++)
		{
		if (strncmp(argv[i1],"-a=",3) == 0 && acnt)
			{
			acnt--;
			if (!savhex (argv[i1]+3,&slavebase))
				{
				displ (argv[0]);
				sprintf (sbuf, "netprint: illegal address '%s'\n",
						argv[i1]+3);
				write(outpath,sbuf,strlen(sbuf));
				exit(errno);
				}
			}
		else if (strncmp(argv[i1],"-s=",3) == 0 && scnt)
			{
			scnt--;
			if (!savhex (argv[i1]+3,&spoolsize))
				{
				displ (argv[0]);
				sprintf (sbuf, "netprint: illegal size '%s'\n",
						argv[i1]+3);
				write(outpath,sbuf,strlen(sbuf));
				exit(errno);
				}
			}
		else if (strncmp(argv[i1],"/dec",4) == 0) {
				strcpy(devname,argv[i1]);
				strcat(devname,"@99");
				if ((path = open (devname, S_IREAD)) == ERROR) {
					exit (_errmsg (errno, 
					"opening device descr. %s failed!!\n",
							devname));
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path,GETDBGBUF,&slavebase,&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if (strncmp(argv[i1],"/n0",4) == 0) {
				strcpy(devname,argv[i1]);
				strcat(devname,"@");
				if ((path = open (devname, S_IREAD)) == ERROR) {
					exit (_errmsg (errno, 
					"opening device descr. %s failed!!\n",
							devname));
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path,GETDBGBUF,&slavebase,&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if (strncmp(argv[i1],"/tcp",4) == 0) {
				path = open ("/nsock", S_IREAD);
				if (path == ERROR) {
					path = open ("/sock", S_IREAD);
					if (path == ERROR) {
						exit (_errmsg (errno, 
						"opening device descr. /tcp failed!!\n"));
					}
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path, TCP_GETDBGBUF, &slavebase,
							&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if ((strncmp(argv[i1],"/nsock",5) == 0) ||
			 (strncmp(argv[i1],"/sock",5) == 0)) {
				path = open ("/nsock", S_IREAD);
				if (path == ERROR) {
					path = open ("/sock", S_IREAD);
					if (path == ERROR) {
						exit (_errmsg (errno, 
						"opening device descr. /tcp failed!!\n"));
					}
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path, TCP_GETDBGBUF, &slavebase,
							&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if (strncmp(argv[i1],"/isdn",4) == 0) {
				if ((path = open ("/isdn", S_IREAD)) == ERROR) {
					exit (_errmsg (errno, "opening device descr. /ifapi failed!!\n"));
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path, 5, buffer,
							spoolsize); 
				if(error != 0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if (strncmp(argv[i1],"/iso",4) == 0) {
				path = open ("/nsock", S_IREAD);
				if (path == ERROR) {
					path = open ("/sock", S_IREAD);
					if (path == ERROR) {
						exit (_errmsg (errno, 
						"opening device descr. /tcp failed!!\n"));
					}
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path, TCP_GETDBGBUF, &slavebase,
							&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else if (strncmp(argv[i1],"/mw_tcp",7) == 0) {
				path = open ("/nsocket", S_IREAD);
				if (path == ERROR) {
					path = open ("/socket", S_IREAD);
					if (path == ERROR) {
						exit (_errmsg (errno, 
						"opening device descr. /tcp failed!!\n"));
					}
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path, TCP_GETDBGBUF, &slavebase,
							&spoolsize); 
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		else {
				strcpy(devname,argv[i1]);
				strcat(devname,"@");
				if ((path = open (devname, S_IREAD)) == ERROR) {
					exit (_errmsg (errno, 
					"opening device descr. %s failed!!\n",
							devname));
				}
				slavebase=(ULONG)buffer;
				spoolsize=0x3000;
				error=putsock(path,GETDBGBUF,&slavebase,&spoolsize); 
				sprintf(sbuf, "slavebase=%x, error=%x\n",slavebase,error);
				write(outpath,sbuf,strlen(sbuf));
				if(slavebase==(ULONG)0)
					exit(_errmsg(errno,
						" ERROR: no Debug Buffer in use !\n"));
				close(path);
				localbuf=TRUE;
			}
		}
	printf("- got buffer\n");

	bp=(UCHAR*)(slavebase);

	if (localbuf)
		bp=(UCHAR*)(*(ULONG*)bp);
	else 
		bp=(UCHAR*)(slavebase + ((*(SHORT*)bp << 16) | *(SHORT*)(bp+2)) |
				 (slavebase & 0xffff0000));


	sprintf(sbuf, "----- printing debug buffer at $%x with size $%x, starting at $%x -----\n",
			slavebase,spoolsize,bp);
	write(outpath,sbuf,strlen(sbuf));
	space_skip=1;				/* skip over leading spaces */

	for (i=(UCHAR*)bp;i<(UCHAR*)(slavebase+spoolsize);i++) 
		if(!(space_skip && (*i==0x20))) {
			sprintf (sbuf,"%c",*i);
			write(outpath,sbuf,strlen(sbuf));
			space_skip=0;
		}

	for (i=(UCHAR*)(slavebase+4);i<(UCHAR*)bp;i++)
		if(!(space_skip && (*i==0x20))) {
			sprintf (sbuf, "%c",*i); 
			write(outpath,sbuf,strlen(sbuf));
			space_skip=0;
		}
	sprintf(sbuf, "\n\014");
	write(outpath,sbuf,strlen(sbuf));
}
