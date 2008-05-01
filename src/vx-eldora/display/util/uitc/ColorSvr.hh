/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorSvr.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  13:50:06  thor
 * Initial revision
 *
 *
 * description:
 *        
 */
#ifndef INCColorSvrhh
#define INCColorSvrhh

#define OK_RPC

#include "ColorRpc.hh"

#include "Window.hh"
#include "Task.hh"

class ColorSvr : public Window {
  private:
    GraphicController *ctlr;
    Task *rpc;

    void Draw(void);

  public:
    ColorSvr(GraphicController *agc);

    void Redraw(ColorCmd *cblk);
};


#endif // INCColorSvrhh

