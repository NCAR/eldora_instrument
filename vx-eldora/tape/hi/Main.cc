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
    
    while ((opt = getopt(argc,argv,"t:")) != -1)
      target = optarg;

    BaseWdw Base(argc,argv);
    
    StatusWdw Sys0(0);
    StatusWdw Sys1(1);
 
    Base.setSys0(Sys0);
    Base.setSys1(Sys1);

    if (target != NULL)
      {
          Base.startRpc(target);
      }
          Base.updateTime(tmo);

    Notifier notifier;

    notifier.start();

    exit(0);
}
