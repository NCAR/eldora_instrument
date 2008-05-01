//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 Main.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1994/03/24  16:19:58  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <iostream.h>
#include <stdlib.h>
#include "BaseWdw.hh"

main(int argc, char **argv)
{
    char *target = NULL;
    int tmo = 60;               // Defaults to 1 minute.

    extern char *optarg;
    extern int opterr;

    int opt;

    opterr = 0;
    
    while ((opt = getopt(argc,argv,"t:s:")) != -1)
      {
          if (opt == 't')
            target = optarg;
          else if (opt == 's')
            tmo = atoi(optarg);
      }

    BaseWdw Base(argc,argv);
    
    StatusWdw Sys0(0);
    StatusWdw Sys1(1);
 
    Base.setSys0(Sys0);
    Base.setSys1(Sys1);

    if (target != NULL)
      {
          Base.startRpc(target);
          Base.updateTime(tmo);
      }

    Notifier notifier;

    notifier.start();

    exit(0);
}
