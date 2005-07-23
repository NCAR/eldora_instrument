//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          testme.cc
//      Original Author: Rich Neitzel
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

#include <iostream.h>
#include <unistd.h>

void ymajor(int x1, int y1, int x2, int y2);
void xmajor(int x1, int y1, int x2, int y2);

main()
{
  int x1 = -10;
  int y1 =  20;
  int x2 = -2;
  int y2 = 5;                   // Must have y2 < y;

  ymajor(x1,y1,x2,y2);

  x1 = 0;
  y1 = 0;
  x2 = 21;
  y2 = 5;
  
  ymajor(y2,x2,y1,x1);
  exit(0);
}


void ymajor(int x1, int y1, int x2, int y2)
{
  int dx = x2 - x1;
  int dy = y2 - y1;

  int slope = (dx << 16) / dy;

  cout << dx << " " << dy << " " << slope << "\n";

  int x = x1 << 16;

  for (int y = y1; y > y2; y--)
    {
      cout << (x >> 16) << ", " << y << endl;
      x -= slope;
    }

  cout << x2 << ", " << y2 << endl;
}

void xmajor(int x1, int y1, int x2, int y2)
{
  int dx = x1 - x2;
  int dy = y1 - y2;

  int len = dx / dy;
  int rem = dx - (len * dy);

  int total = 32768;
  int inc = (rem << 16) / dy;

  int x = x1;

  for (int y = y1; y < y2; y++)
    {
      total += inc;
      cout << total << "\n";
      if (total < 65536)
        {
          int l = len + x - 1;
          for (int i = x; i <= l ; i++)
            cout << i << ", " << y << "\n";
        }
      else
        {
          int l = len + x;
         for (int i = x; i <= l ; i++)
            cout << i << ", " << y << "\n";
         total -= 65536;
        }
      x += len;
    }

  cout << x << ", " << y2 << "\n";
}

