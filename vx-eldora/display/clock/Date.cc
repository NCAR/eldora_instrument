/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Date.cc
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

#include "Date.hh"

extern "C" {
#include "string.h"
#include "stdioLib.h"
};

Date::Date(GraphicController *cntlr, int window, unsigned short x, unsigned
	   short y, unsigned short xoff, unsigned short yoff) : 
	   Window(cntlr, window, x, y, DATE_WIDTH, DATE_HEIGHT, xoff, yoff)
{
    lastDay = -1;		// Set no valid day marker.

    setPriority(MAXWDWPRI);

    Point a;

    a.x = 0;
    a.y = 0;

    frect(a,DATE_WIDTH,DATE_HEIGHT,BLACK);

    setTextScale(2,2);

    setTextBackGround(BLACK);
}

void Date::Set(FAST short day, FAST short month, FAST short year)
{
    if (day < 1)
      day = 1;
    else if (day > 31)
      day = 31;

    if (month < 1)
      month = 1;
    else if (month > 12)
      month = 12;

    if (year < 0)
      year = 0;
    else if (year > 1899 && year < 2000)
      year -= 1900;
    else if (year >= 2000)
      year -= 2000;

    lastDay = day;
    lastMonth = month;
    lastYear = year;

    Draw();
}

void Date::Draw(void)
{
    FAST short day = lastDay;
    FAST short month = lastMonth;
    FAST short year = lastYear;

    char tmp[3];

    sprintf(tmp,"%.2d",day);

    tmp[3] = 0;

    char date[10];

    strcpy(date,tmp);

    switch(month)
      {
	case 1:
	  strcat(date,"Jan");
	  break;

	case 2:
	  strcat(date,"Feb");
	  break;

	case 3:
	  strcat(date,"Mar");
	  break;

	case 4:
	  strcat(date,"Apr");
	  break;

	case 5:
	  strcat(date,"May");
	  break;

	case 6:
	  strcat(date,"Jun");
	  break;

	case 7:
	  strcat(date,"Jul");
	  break;

	case 8:
	  strcat(date,"Aug");
	  break;

	case 9:
	  strcat(date,"Sep");
	  break;

	case 10:
	  strcat(date,"Oct");
	  break;

	case 11:
	  strcat(date,"Nov");
	  break;

	case 12:
	  strcat(date,"Dec");
	  break;
      }

    sprintf(tmp,"%.2d",year);

    tmp[3] = 0;

    strcat(date,tmp);

    Point a;

    a.x = 0;
    a.y = 0;

    frect(a,DATE_WIDTH,DATE_HEIGHT,BLACK);

    a.x = 5;

    a.y = 20;

    horText(a,date,WHITE1);
}

void Date::NextDay(void)
{
    FAST short day = lastDay;
    FAST short month = lastMonth;
    FAST short year = lastYear;

    day++;

    switch(month)
      {
	case 2:
	  if (year % 4)
	    {
		if (day > 28)
		  {
		      month++;
		      day = 1;
		  }
	    }
	  else if (day > 29)
	    {
		day = 1;
		month++;
	    }
	  break;

	case 12:
	  if (day > 31)
	    {
		day = 1;
		year++;

		if (year == 100)
		  year = 0;
	    }
	  break;

	case 4:
	case 6:
	case 9:
	case 11:
	  if (day > 30)
	    {
		day = 1;
		month++;
	    }
	  break;

	default:
	  if (day > 31)
	    {
		day = 1;
		month++;
	    }
      }

    lastDay = day;
    lastMonth = month;
    lastYear = year;

    Draw();
}
