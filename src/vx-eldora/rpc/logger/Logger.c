/*
 *	$Id$
 *
 *	Module:		 Logger.c
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 2.4  1994/10/19  15:03:31  thor
 * Added missing svc_run().
 *
 * Revision 2.3  1994/09/07  16:43:33  thor
 * Updated to the new TLIRPC.
 *
 * Revision 2.2  1994/03/22  16:00:14  thor
 * Changed LOG to LOGMSG, to avoid name conflicts.
 *
 * Revision 2.1  1993/09/10  16:42:56  thor
 * New improved version!
 *
 * Revision 2.0  1992/11/02  20:48:31  thor
 * First offical ELDORA release!
 *
 * Revision 1.2  1992/09/14  16:31:52  thor
 * Added new host & made things a little clearer.
 *
 * Revision 1.1  1992/01/16  19:40:11  thor
 * Initial revision
 *
 *
 * description:
 *        This program opens a single event log file & seperate error
 * logs for each target. 
 *
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#ifndef UNIX
#define UNIX
#endif

#ifndef OK_RPC
#define OK_RPC
#endif

#include "logger.h"

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h> /* for pmap_unset */
#include <string.h> /* strcmp */
#include <signal.h>
#ifdef __cplusplus
#include <sysent.h> /* getdtablesize, open */
#endif /* __cplusplus */
#include <unistd.h> /* setsid */
#include <sys/types.h>
#include <memory.h>
#include <stropts.h>
#include <netconfig.h>
#include <sys/resource.h> /* rlimit */
#include <syslog.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

#define _RPCSVC_CLOSEDOWN 120
static int _rpcpmstart;         /* Started by a port monitor ? */
        /* States a server can be in wrt request */

#define _IDLE 0
#define _SERVED 1
#define _SERVING 2

int _rpcsvcstate = _IDLE;         /* Set when a request is serviced */

#ifndef KNOWN_HOSTS
#define KNOWN_HOSTS 6
#endif

static void _msgout(char* msg);
static void closedown(int sig);

static char *directory = ".";	/* Default directory path. */
static char *files[] = {
"fore.log", "aft.log", "disp1.log", "disp2.log", "record.log", "hskp.log"
};

main(FAST int argc, FAST char **argv)
{
    FAST SVCXPRT *transp;
    FAST FILE **fps;
    FAST int i;
    FAST int j = KNOWN_HOSTS;
    FAST pid_t pid;
    char mname[FMNAMESZ + 1];


    if (argc > 1)
      directory = argv[1];

    if (!ioctl(0, I_LOOK, mname) &&
        (!strcmp(mname, "sockmod") || !strcmp(mname, "timod"))) {
        char *netid;
        struct netconfig *nconf = NULL;
        SVCXPRT *transp;
        int pmclose;

        openlog("logger", LOG_PID, LOG_DAEMON);

        if ((netid = getenv("NLSPROVIDER")) == NULL) {
            /* started from inetd */
            pmclose = 1;
        } else {
            if ((nconf = getnetconfigent(netid)) == NULL)
              _msgout("cannot get transport info");

            pmclose = (t_getstate(0) != T_DATAXFER);
        }
        if (strcmp(mname, "sockmod") == 0) {
            if (ioctl(0, I_POP, 0) || ioctl(0, I_PUSH, "timod")) {
                _msgout("could not get the right module");
                exit(1);
            }
        }
        if ((transp = svc_tli_create(0, nconf, NULL, 0, 0)) == NULL) {
            _msgout("cannot create server handle");
            exit(1);
        }
        if (nconf)
          freenetconfigent(nconf);
        if (!svc_reg(transp, Logger, LoggerVers, logger_1, 0)) {
            _msgout("unable to register (Logger, LoggerVers).");
            exit(1);
        }
        if (pmclose) {
            (void) signal(SIGALRM, (SIG_PF) closedown);
            (void) alarm(_RPCSVC_CLOSEDOWN/2);
        }
        svc_run();
        exit(1);
        /* NOTREACHED */
    }       else {
#ifndef RPC_SVC_FG
        int size;
        struct rlimit rl;
        pid = fork();
        if (pid < 0) {
            perror("cannot fork");
            exit(1);
        }
        if (pid)
          exit(0);
        rl.rlim_max = 0;
        getrlimit(RLIMIT_NOFILE, &rl);
        if ((size = rl.rlim_max) == 0)
          exit(1);
        for (i = 0; i < size; i++)
          (void) close(i);
        i = open("/dev/console", 2);
        (void) dup2(i, 1);
        (void) dup2(i, 2);
        setsid();
        openlog("logger", LOG_PID, LOG_DAEMON);
#endif
    }
    if (!svc_create(logger_1, Logger, LoggerVers, "netpath")) {
        _msgout("unable to create (Logger, LoggerVers) for netpath.");
        exit(1);
    }
    svc_run();
    fprintf(stderr, "svc_run returned");
    exit(1);
}

void *logmessage_1_svc(FAST LOGMSG *argp, FAST struct svc_req *req)
{
    FAST FILE *fp;
    char buffer[256];
    
    strcpy(buffer,directory);
    strcat(buffer,"/");
    strcat(buffer,files[argp->src]);

    fp = fopen(buffer,"a+");

    fprintf(fp,argp->message,argp->items[0],argp->items[1],
	    argp->items[2],argp->items[3],argp->items[4],argp->items[5],
	    argp->items[6],argp->items[7],argp->items[8],argp->items[9]);

    fclose(fp);
    return(argp);
}

static void _msgout(char* msg)
{
    syslog(LOG_ERR,msg);
}

static void closedown(int sig)
{
        if (_rpcsvcstate == _IDLE) {
                extern fd_set svc_fdset;
                static int size;
                int i, openfd;
                struct t_info tinfo;

                if (!t_getinfo(0, &tinfo) && (tinfo.servtype == T_CLTS))
                        exit(0);
                if (size == 0) {
                        struct rlimit rl;

                        rl.rlim_max = 0;
                        getrlimit(RLIMIT_NOFILE, &rl);
                        if ((size = rl.rlim_max) == 0) {
                                return;
                        }
                }
                for (i = 0, openfd = 0; i < size && openfd < 2; i++)
                        if (FD_ISSET(i, &svc_fdset))
                                openfd++;
                if (openfd <= 1)
                        exit(0);
        }
        if (_rpcsvcstate == _SERVED)
                _rpcsvcstate = _IDLE;

        (void) signal(SIGALRM, (SIG_PF) closedown);
        (void) alarm(_RPCSVC_CLOSEDOWN/2);
}
