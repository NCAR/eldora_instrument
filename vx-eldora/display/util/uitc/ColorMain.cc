/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorMain.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include <stream.h>

#include "ColorWdw.hh"

main(int argc, char **argv)
{
    extern char *optarg;

    char *file = NULL;
    char *target = NULL;

    int opt;

    while ((opt = getopt(argc,argv,"t:f:")) != -1)
      {
	  if (opt == 't')
	    target = optarg;
	  else if (opt == 'f')
	    file = optarg;
      }

    ColorWdw base(argc,argv,file);

    if (target != NULL)
      base.Connect(target);

    Notifier notifier;
    notifier.start ();

    exit (0);
}

