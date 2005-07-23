/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSet.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  13:50:02  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
#ifndef INCColorSethh
#define INCColorSethh

#define OK_RPC

#ifndef WRS
#define UNIX
#endif

#include "ColorRpc.hh"

class ColorSet {
  private:
    ColorCmd cblk;

    int set;

    int index;

    int saved;

    int active;

    long oldRGB;

    CLIENT *client;

  public:
    ColorSet(char *file = NULL);

    int Target(char *target);

    void Set(FAST int newset) { set = newset; index = set * MAX_COLORS_SET; }
    int Set(void) { return(set); }

    void Index(FAST int i) { index = i + (set * MAX_COLORS_SET); }
    int Index(void) { return(index - (set * MAX_COLORS_SET)); }

    void Reset(void);

    void Red(int red);
    int Red(void);

    void Green(int green);
    int Green(void);

    void Blue(int blue);
    int Blue(void);

    int Send(void);

    int Read(char *file);

    int Write(char *file);
};

#endif // INCColorSethh

