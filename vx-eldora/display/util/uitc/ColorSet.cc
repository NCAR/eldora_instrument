/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSet.cc
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ColorSet.hh"

#include "default_colors.h"

#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

static const int RED_OFF   = 24;
static const int GREEN_OFF = 16;
static const int BLUE_OFF  = 8;

static const int RED_MASK   = 0xffff00;
static const int GREEN_MASK = 0xff00ff00;
static const int BLUE_MASK  = 0xffff0000;


ColorSet::ColorSet(char *file)
{
    if (file == NULL)
      {
	  cblk.index = USE_DEF;

	  bcopy((char *)defaultColors,(char *)&cblk.colors[0],sizeof(long) 
		* 256);
      } else {
	  if (Read(file))
	    {
		perror("Failure in ColorSet constructor");
		exit(1);
	    }

	  cblk.index = DO_ALL;
      }

    index = DO_ALL;

    Set(0);
    Index(0);

    saved = 0;
    
    active = 0;
}

int ColorSet::Target(char *target)
{
#ifndef TEST
    FAST CLIENT *clnt = clnt_create(target,ColorSetProg,ColorSetVers,"udp");

    if (clnt == NULL)
      {
	  clnt_pcreateerror("Failure in ColorSet constructor");
	  return(1);
      }

    client = clnt;

    index = USE_DEF;

    active = 1;
#endif // TEST
    return(0);
}

void ColorSet::Reset(void)
{
    cblk.colors[index] = oldRGB;

    saved = 0;
}

void ColorSet::Red(FAST int red)
{
    if (!saved)
      {
	  oldRGB = cblk.colors[index];
	  saved++;
      }
    
    FAST long color = cblk.colors[index];

    color &= RED_MASK;

    color |= (red << RED_OFF);

    cblk.colors[index] = color;
}

int ColorSet::Red(void)
{
    FAST long color = cblk.colors[index];

    color &= ~RED_MASK;

    color >>= RED_OFF;

    color &= 0xff;		// For machines that replicate the high bit.

    return(color);
}

void ColorSet::Green(FAST int green)
{
    if (!saved)
      {
	  oldRGB = cblk.colors[index];
	  saved++;
      }
    
    FAST long color = cblk.colors[index];

    color &= GREEN_MASK;

    color |= (green << GREEN_OFF);

    cblk.colors[index] = color;
}

int ColorSet::Green(void)
{
    FAST long color = cblk.colors[index];

    color &= ~GREEN_MASK;

    color >>= GREEN_OFF;

    return(color);
}

void ColorSet::Blue(FAST int blue)
{
    if (!saved)
      {
	  oldRGB = cblk.colors[index];
	  saved++;
      }
    
    FAST long color = cblk.colors[index];

    color &= BLUE_MASK;

    color |= (blue << BLUE_OFF);

    cblk.colors[index] = color;
}

int ColorSet::Blue(void)
{
    FAST long color = cblk.colors[index];

    color &= ~BLUE_MASK;

    color >>= BLUE_OFF;

    return(color);
}

int ColorSet::Send(void)
{
#ifndef TEST
    static char res;

    bzero((char *)&res, sizeof(res));

    struct timeval TIMEOUT;

    TIMEOUT.tv_sec = 25;
    TIMEOUT.tv_usec = 0;

    cblk.index = index;

    if (clnt_call(client,ColorSetProc,xdr_ColorCmd,(caddr_t)&cblk,xdr_void,
		  &res,
		  TIMEOUT) != RPC_SUCCESS)
      {
	  return (1);
      }
#endif // TEST

    saved = 0;

    return (0);
}

int ColorSet::Read(char *file)
{
    FAST int fd;

    if ((fd = open(file,O_RDONLY)) < 0)
      {
	  perror("ColorSet read failure");
	  return(1);
      }

    FAST int size = sizeof(long) * 3 * MAX_COLORS_SET;

    if (read(fd,(char *)&cblk.colors[0],size) != size)
      {
	  close(fd);
	  perror("ColorSet read failure");
	  return(1);
      }

    close(fd);

    saved = 0;

    FAST int old = index;

    index = DO_ALL;

    if (active)
      Send();

    index = old;

    return(0);
}

int ColorSet::Write(char *file)
{
    FAST int fd;

    if ((fd = open(file,O_WRONLY | O_CREAT, 0644)) < 0)
      {
	  perror("ColorSet write failure");
	  return(1);
      }

    FAST int size = sizeof(long) * 256;

    if (write(fd,(char *)&cblk.colors[0],size) != size)
      {
	  close(fd);
	  perror("ColorSet read failure");
	  return(1);
      }

    close(fd);

    saved = 0;

    return(0);
}
