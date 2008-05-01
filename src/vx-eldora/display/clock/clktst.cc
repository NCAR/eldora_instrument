#include "Clock.hh"
#include "Date.hh"

extern "C" {
#include "timexLib.h"
#include "stdioLib.h"
#include "tickLib.h"
#include "taskLib.h"
#include "memLib.h"
#include "math.h"

void clktst(short,short);
void datetst(short,short);

};

void clktst(short x, short y)
{
    GraphicController agc((void *)0x80000000,1280,1024,2048,250);

    Clock clk(&agc,3,x,y,1024,1792);

    clk.display();

    FAST int j = 10;

    short hour = 0;
    short minute = 0;
    short second = 0;

    Point p;

    p.x = x;
    p.y = y;
    
    for (FAST int i = 0; i < j; i++)
      {
	  clk.update(hour,minute,second);
	  second++;
	  taskDelay(60);
      }

    hour = 23;
    minute = 59;
    second = 50;

    for (i = 0, j = 60; i < j; i++)
      {
	  clk.update(hour,minute,second);

	  second++;
	  
	  if (second == 60)
	    {
		second = 0;
		minute++;
	    }

	  if (minute == 60)
	    {
		minute = 0;
		hour++;
	    }

	  if (hour == 24)
	    hour = 0;

          p.x += 5;

          clk.move(p);
          
	  taskDelay(60);
      }
}
