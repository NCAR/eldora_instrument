//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Logger.cc
//      Original Author: Richard E.Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#ifndef UNIX
#define UNIX
#endif

#include <stdio.h>
#include <cstring>
#include "udpSvr.hh"
#include "logger.h"


static char *directory = ".";	// Default directory path.
static char *files[] = {
  "fore.log", "aft.log", "disp1.log", "disp2.log", "record.log", "hskp.log"
};

main(int argc, char **argv)
{
  if (argc > 1)
    directory = argv[1];

  udpSvr svr(loggerPort);

  while (1)
    {
      LOGMSG log;

      memset((void *)&log,0,sizeof(log));

      svr.recvfrom((void *)&log,sizeof(log));

      char buffer[256];
    
      strcpy(buffer,directory);
      strcat(buffer,"/");
      strcat(buffer,files[log.src]);

      FILE *fp = fopen(buffer,"a+");

      fprintf(fp,log.message,log.items[0],log.items[1],
	      log.items[2],log.items[3],log.items[4],log.items[5],
	      log.items[6],log.items[7],log.items[8],log.items[9]);

      fclose(fp);
    }
  return 0;
}
