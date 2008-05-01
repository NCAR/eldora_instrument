/*setdebug.c****************************************************************
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
* Module:	setdebug.c						   *
*                                                                          *
* Author: 	Uwe Linke						   *
*                                                                          *
* Rev.  : 	02		Date of creation: ??.??.??		   *
*                                                                          *
*--------------------------------------------------------------------------*
*                                                                          *
* Module description:                                                      *
*	Set debug levels for protocol layers.				   *
*                                                                          *
* Functions:                                                               *
*                                                                          *
*                                                                          *
*--------------------------------------------------------------------------*
*                                                                          *
* Revision History:                                                        *
*									   *
* #	Date		Description				Author	   *
*--------------------------------------------------------------------------*
* 01	??.??.??	initial version				ul	   *
* 02	01.03.94	Help output changed.			ul	   *
****************************************************************************
***************************************************************************/
#include <stdio.h>
#include <natdefs.h>
#include "/w4/natsrc/tcpip/include/machine/os9/endian.h"
#include "/w4/natsrc/tcpip/include/sys/socket.h"
#include "/w4/natsrc/tcpip/include/sys/ioctl.h"
#include "/w4/natsrc/tcpip/include/sys/sockvar.h"
#include "/w4/natsrc/tcpip/include/net/if.h"
#include "/w4/natsrc/tcpip/include/netinet/tcp.h"
#include "/w4/natsrc/tcpip/include/netinet/in.h"
#include "/w4/natsrc/tcpip/include/netdb.h"
#include <errno.h>

char *myname;				/* pointer to name of this pgm	*/

main(argc,argv)
	int argc;
	char *argv[];
{
	int s;				/* socket descriptor		*/
	int debuglevel, level, optname;
	int optvallen;
	char levelstr[32], option[256];
	int optval;
	struct ifreq ifr;

	myname = argv[0];


	/*
	 * Check that there are two command line arguments
	 */
	if(argc < 2 || argc > 3) {
		usage();
		exit(1);
	}
	if (strcmp(argv[1], "-?") == 0) {
		usage();
		exit(0);
	} else if (strcmp(argv[1], "nat0") == 0) {
		optval = -12;
		debuglevel = atoi(argv[2]);
	} else if (strcmp(argv[1], "nat1") == 0) {
		optval = -13;
		debuglevel = atoi(argv[2]);
	} else if (strcmp(argv[1], "nat2") == 0) {
		optval = -14;
		debuglevel = atoi(argv[2]);
	} else if (strcmp(argv[1], "nat3") == 0) {
		optval = -15;
		debuglevel = atoi(argv[2]);
	} else if (strcmp(argv[1], "lo0") == 0) {
		optval = -1;
		debuglevel = atoi(argv[2]);
	} else if (strcmp(argv[1], "sl0") == 0) {
		optval = -3;
		debuglevel = atoi(argv[2]);
	} else {
		debuglevel = atoi(argv[1]);

		if(debuglevel < 0) {
			usage();
			exit(1);
		} else if (debuglevel < 10) {
			optval = debuglevel;
			level = SOL_SOCKET;
			optname = SO_DEBUG;
		} else if (debuglevel < 20) {
			optval = debuglevel - 10;
			level = 6;
			optname = TCP_DEBUG;
		} else if (debuglevel < 30) {
			optval = debuglevel - 20;
			level = 0;
			optname = IP_DEBUG;
		} else {
			usage();
			exit(1);
		}
	}

	/*
	 * Allocate an open socket
	 */
	if((s = socket(AF_INET, SOCK_STREAM, 6)) < 0) {
		fprintf(stderr, "%s: socket: errno = %d\n", myname, errno);
		exit(1);
	}

	SetDebugBuf(s, 0x3000);

	/*
	 * Set option
	 */
	if (optval >= 0) {
		optvallen = sizeof(optval);
		if (setsockopt(s, level, optname, &optval, optvallen) < 0) {
			fprintf(stderr, "%s: setsockopt: errno = %d\n",
				myname, errno);
		}
	} else {
		switch(optval) {

		case -1:
			strcpy(ifr.ifr_name, "lo0");
			break;
		case -12:
			strcpy(ifr.ifr_name, "nat0");
			break;
		case -13:
			strcpy(ifr.ifr_name, "nat1");
			break;
		case -14:
			strcpy(ifr.ifr_name, "nat2");
			break;
		case -15:
			strcpy(ifr.ifr_name, "nat3");
			break;
		case -3:
			strcpy(ifr.ifr_name, "sl0");
			break;
		}
		ifr.ifr_debug = debuglevel;
		if (ioctl(s, SIOCSIFDEBUG, (caddr_t)&ifr) < 0) {
			fprintf(stderr, "%s: ioctl (SIOCSIFDEBUG), errno=%x\n",
					myname, errno);
			exit(1);
		}
	}

	/*
	 * Get option
	 */
	if (optval >= 0) {
		if (getsockopt(s, level, optname, &optval, &optvallen) < 0) {
			fprintf(stderr,"%s: getsockopt: errno = %d\n",myname,errno);
		} else {
			printf("New Debuglevel for the ");
			switch(level) {

			case SOL_SOCKET:
				printf("Socket ");
				break;

			case 6:
				printf("Transport ");
				break;

			case 0:
				printf("Network ");
				break;
			}
			printf("Layer is %d.\n", optval);
		}
	} else {
		if (ioctl(s, SIOCGIFDEBUG, (caddr_t)&ifr) < 0) {
			fprintf(stderr, "%s: ioctl (SIOCGIFDEBUG)\n");
			exit(1);
		} else {
			printf("New Debuglevel for the ");
			switch(optval) {

			case -1:
				printf("loopback ");
				break;

			case -12:
			case -13:
			case -14:
			case -15:
				printf("NAT ");
				break;

			case -3:
				printf("Serial Line ");
				break;
			}
			printf("driver is %d.", ifr.ifr_debug);
		}
	}
	/*
	 * Close socket
	 */
	close(s);
}


usage()
{
	fprintf(stderr,
		"---------- N.A.T. SETDEBUG-Utility ----------\n");
	fprintf(stderr,
		"Copyright: N.A.T. GmbH\n");
	fprintf(stderr,
		"Syntax   : %s [<options>] [<interface>] <level>\n", myname);
	fprintf(stderr,
		"Function : Set debug level for protocol layer.\n");
	fprintf(stderr,
		"Options  :\n");
	fprintf(stderr,
		"    none\n\n");
	fprintf(stderr,
		"        <level>  1-9  = Socket    Layer Levels\n");
	fprintf(stderr,
		"        <level> 11-19 = Transport Layer Levels\n");
	fprintf(stderr,
		"        <level> 21-29 = Network   Layer Levels\n");
	fprintf(stderr,
		"        <level> 0, 10, 20 turns off debugging\n\n");
	fprintf(stderr,
		"        Possible values for <interface> are:\n");
	fprintf(stderr,
		"            lo0  --- loopback driver\n");
	fprintf(stderr,
		"            nat0 --- NAT driver\n");
	fprintf(stderr,
		"            sl0  --- Serial Line driver\n");
	fprintf(stderr,
		"        <level> 0 turns off debugging\n");
}
