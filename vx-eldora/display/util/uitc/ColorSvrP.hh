/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSvrP.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  13:50:07  thor
 * Initial revision
 *
 * Revision 1.1  1991/11/20  20:42:30  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCColorSvrPhh
#define INCColorSvrPhh

static const int AGC = 0xf0000000;
static const int BOXH = 35;
static const int BOXW = 35;
static const int X_WIDTH = 40;
static const int Y_WIDTH = 100;
static const int TEXT_Y = 60;

extern "C" {
#include "stdioLib.h"
#include "rpcLib.h"
};

static char *Labels[] = {
"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14",
"15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26",
"27", "28", "29" "30", 0
};

#endif // INCColorSvrPhh

