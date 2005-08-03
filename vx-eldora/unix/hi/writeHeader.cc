/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeInit.cc
 *	Original Author: Richard E. K. Neitzel
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

#include <fstream.h>
#include <cstring>
#include <ctime>
#include <unistd.h>
#include <sys/param.h>
#include <netinet/in.h>

#include "Header.hh"

main(int argc, char **argv)
{
  char *flight = argv[1];
  char *file = argv[2];
  char *outfile = "/vxbin/headers/current.hdr";

  Header hdr;

  ifstream input(file);

  input >> hdr;

  WAVEFORM *w = hdr.Waveform();
  
  VOLUME *volume = hdr.Volume();

  strncpy(&volume->flight_num[0],flight,8);

  time_t t;
  time(&t);
  struct tm *gmt = gmtime(&t);

  int year = gmt->tm_year;

  year += 1900;

  volume->year = htons((short)year);
  volume->month = htons((short)gmt->tm_mon + 1);
  volume->day = htons((short)gmt->tm_mday);
  volume->data_set_hour = htons((short)gmt->tm_hour);
  volume->data_set_minute = htons((short)gmt->tm_min);
  volume->data_set_second = htons((short)gmt->tm_sec);

  ofstream output(outfile,ios::out|ios::trunc);

  output << hdr;

  output.close();

  return 0;
}
