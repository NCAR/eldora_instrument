//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          ColorEditor.cc
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

#include "ColorEditor.hh"
#include <fstream.h>
#include <string.h>

Box::Box(Window &wndw, Point p, int height, int width, unsigned char index)
{
    wdw = &wndw;
    origin = p;
    w = width;
    h = height;

    wdw->frect(origin,w,h,index);
}

int Box::contains(Point p)
{
    FAST int px = p.x;
    FAST int py = p.y;
    FAST int bx = origin.x;
    FAST int by = origin.y;

    if ((px < bx) || (py < by))
      return(0);

    bx += w;
    by += h;

    if ((px < bx) || (py < by))
      return(0);
}
    
Button::Button(Window &w, Point p, int height, int width, char *l,
               unsigned char index) : Box(w,p,height,width,index)
{
    label(l);
}

void Button::label(char *l)
{
    Point p = origin;

    p.y += h;
    
    wdw->horText(p,l,WHITE1);
}

ColorEditor::ColorEditor(GraphicController &ctlr, char *file) :
wdw(agc,0,0,0,1280,1024,0,0)
{
    agc = &ctlr;

    // Do color tbl.
    if (file != NULL)
        {
            ifstream input(file);

            input >> *this;
        }
    
    Point p;

    p.x = 0;
    p.y = 30;

    FAST int j = 96;

    for (FAST int i = 0; i < j; i++)
        {
            boxes[i] = new CBox(wdw,p,20,20,i);

            if ((i == 31) || (i == 63))
                {
                    p.x = 0;
                    p.y += 50;
                }
            else
              p.x += 30;
        }
    // Create buttons.

    p.x = 0;
    p.y = 200;

    redD = new Button(wdw,p,50,100,"Red <<",BLACK);
    p.x += 110;
    redU = new Button(wdw,p,50,100,"Red >>",BLACK);

    p.x = 0;
    p.y += 70;

    greenD = new Button(wdw,p,50,100,"Green <<",BLACK);
    p.x += 110;
    greenU = new Button(wdw,p,50,100,"Green >>",BLACK);
    
    p.x = 0;
    p.y += 70;

    blueD = new Button(wdw,p,50,100,"Blue <<",BLACK);
    p.x += 110;
    blueU = new Button(wdw,p,50,100,"Blue >>",BLACK);

    redD->setCallback(this);
    greenD->setCallback(this);
    blueD->setCallback(this);

    redU->setCallback(this);
    greenU->setCallback(this);
    blueU->setCallback(this);

    p.x = 0;
    p.y += 70;

    Undo = new Button(wdw,p,50,100,"Undo",BLACK);
    p.x += 110;
    Save = new Button(wdw,p,50,100,"Save",BLACK);
    p.x += 110;
    Load = new Button(wdw,p,50,100,"Load",BLACK);
    p.x += 110;
    Quit = new Button(wdw,p,50,100,"Quit",BLACK);

    Undo->setCallback(this);
    Load->setCallback(this);
    Save->setCallback(this);
    Quit->setCallback(this);

    index = 0;

    markBox(*boxes[0]);

    quitingTime = 0;
}

void ColorEditor::run()
{
    while (!quitingTime)
        {
            mouse->waitOnInterrupt();

            Point cursor = mouse->getXY();

            if (Undo->contains(cursor))
              Undo->doCallback();
            else if (Save->contains(cursor))
              Save->doCallback();
            else if (Load->contains(cursor))
              Load->doCallback();
            else if (Quit->contains(cursor))
              Quit->doCallback();
            else if (redU->contains(cursor))
              redU->doCallback();
            else if (redD->contains(cursor))
              redD->doCallback();
            else if (greenU->contains(cursor))
              greenU->doCallback();
            else if (greenD->contains(cursor))
              greenD->doCallback();
            else if (blueU->contains(cursor))
              blueU->doCallback();
            else if (blueD->contains(cursor))
              blueD->doCallback();
            else
                {
                    unmarkBox(*boxes[index]);

                    FAST CBox *ptr = boxes[0];
                    FAST int j = 96;
                    for (FAST int i = 0; i < j; i++, ptr++)
                        {
                            if (ptr->contains(cursor))
                              index = ptr->color();
                            markBox(*ptr);
                        }
                }
        }
}

void ColorEditor::doCallback(Actor &a)
{
    Button &b = (Button &)a;

    if (&b == Load)
      load(b);
    else if (&b == Save)
      save(b);
    else if (&b == Undo)
      undo(b);
    else if (&b == Quit)
      quit(b);
    else
      adjustColors(b);
}

void ColorEditor::load(Button &j)
{
    cout << "Enter name of file to load: ";

    char name[256];

    cin >> name;

    ifstream input(name);

    input >> *this;

}

void ColorEditor::save(Button &j)
{
    cout << "Enter name of file to save: ";

    char name[256];

    cin >> name;

    ofstream output(name);

    output << *this;

}

void ColorEditor::quit(Button &j)
{
    quitingTime = 1;
}

void ColorEditor::undo(Button &j)
{
    currentColors[index] = oldColors[index];

    setColors();
}

void ColorEditor::adjustColors(Button &caller)
{
    FAST Button *ptr = &caller;

    if (ptr == redU)
      redUp();
    else if (ptr == greenU)
      greenUp();
    else if (ptr == blueU)
      blueUp();
    else if (ptr == redD)
      redDown();
    else if (ptr == greenD)
      greenDown();
    else if (ptr == blueD)
      blueDown();

    oldColors[index] = currentColors[index];

    setColors();
}

void ColorEditor::lines(CBox &box, unsigned char color)
{
    Point b = box.upperLeft();
    int h = box.height();
    int w = box.width();

    --b.x;
    --b.y;

    h++;
    w++;
    
    Point e;

    e.x = b.x + w;

    wdw.line(b,e,color);

    int s = b.y;

    b.y += h;
    e.y += h;

    wdw.line(b,e,color);

    b.y = s;

    s = e.x;

    e.x = b.x;

    wdw.line(b,e,color);

    e.x = s;

    b.x = e.x;

    wdw.line(b,e,color);
}

ColorEditor::~ColorEditor()
{
    delete(redU);
    delete(redD);
    delete(greenU);
    delete(greenD);
    delete(blueU);
    delete(blueD);
    delete(Load);
    delete(Save);
    delete(Undo);
    delete(Quit);

    FAST int j = 96;

    for (FAST int i = 0; i < j; i++)
      delete(boxes[i]);

    wdw.clear();
}

ostream& operator<<(ostream &os, ColorEditor &edt)
{
    FAST long *ptr = edt.currentColors;
    FAST int j = 256;

    for (FAST int i = 0; i < j; i++, ptr++)
      os << *ptr;
}

istream& operator>>(istream &is, ColorEditor &edt)
{
    FAST long *ptr = edt.currentColors;
    FAST int j = 256;

    for (FAST int i = 0; i < j; i++, ptr++)
      is >> *ptr;

    memcpy(edt.oldColors,edt.currentColors,sizeof(edt.oldColors));
}

extern "C" {
    void colors(char *file);
};

void colors(char *file)
{
    GraphicController Agc((void *)0x80000000,1280,1024,2048,250);
    
    ColorEditor editor(Agc,file);

    editor.run();
}
