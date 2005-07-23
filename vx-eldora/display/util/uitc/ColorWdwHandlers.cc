/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorWdwHandlers.cc
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/06/19  13:50:10  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "ColorWdw.hh"

#include "NoticePrompt.h"

#include <stream.h>

void ColorWdw::SaveHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    char *f = FileName.getValue(f);

    if (f == NULL || *f == 0)
      {
	  NoticePrompt note;
    
	  note.setMessage("No such file.");
    
	  note.setLabel("Noted");
    
	  note.show(object);
      }
    else
      Save(f);
	
    delete(f);
}

void ColorWdw::LoadHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);
    char *f = FileName.getValue(f);

    if (f == NULL || *f == 0)
      {
	  NoticePrompt note;
    
	  note.setMessage("No such file.");
    
	  note.setLabel("Noted");
    
	  note.show(object);
      }
    else
      Load(f);
	
    delete(f);
}

void ColorWdw::BrowseHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    Browser->Show();
}

void ColorWdw::QuitHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    NoticePrompt note;
    
    note.setMessage("Really quit?");
    
    note.setYesAndNoLabels("Quit","Cancel");
    
    if (note.show(object) == NOTICE_YES)
      exit(0);
}

void ColorWdw::ColorBinsHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    int i = ColorBins.getChoice(NULL);

    --i;

    Set(i);

    Index(0);

    RedSlider.setValue(Red());
    GreenSlider.setValue(Green());
    BlueSlider.setValue(Blue());

    ColorIndex.setValue(0);

    BuildMap();
    Repaint();
}

void ColorWdw::ColorHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    int red = RedSlider.getValue(NULL);
    int green = GreenSlider.getValue(NULL);
    int blue = BlueSlider.getValue(NULL);

    Red(red);
    Green(green);
    Blue(blue);

    char name[8];

    sprintf(name,"color%d",Index());

    Cmap.changeColor(name,red,green,blue);

    TheColor.setPaintColorMap(Cmap);

    Window wdw = TheColor.getXWindow();
    Display *disp = TheColor.getXDisplay();

    XGCValues  gcval;

    gcval.function = GXcopy;
    GC gc = XCreateGC (disp,wdw,GCFunction,&gcval);

    XSetForeground(disp,gc,Cmap.getXColorIndex(name));

    XFillRectangle(disp,wdw,gc,Index()*18,0,18,60);

    XFlush(disp);

    if (active)
      Send();
}

void ColorWdw::UndoHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    Reset();

    RedSlider.setValue(Red());
    GreenSlider.setValue(Green());
    BlueSlider.setValue(Blue());
}

void ColorWdw::ColorIndexHandler(UIObject *object)
{
    object = getObject(this);
    this = getThis(object);

    int i = ColorIndex.getValue(NULL);

    Index(i);

    RedSlider.setValue(Red());
    GreenSlider.setValue(Green());
    BlueSlider.setValue(Blue());
}
