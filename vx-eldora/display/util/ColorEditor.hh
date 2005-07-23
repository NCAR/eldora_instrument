//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          ColorEditor.hh
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
#ifndef _COLOREDITOR_HH
#define _COLOREDITOR_HH

#include "Window.hh"
#include "Mouse.hh"
#include <iostream.h>

class Actor {
  public:
    Actor() {}
};

static Actor junk;

class Callback;

typedef void (Callback::* *CALLBACKFUNC)(Actor &);

class Callback {
  public:
    Callback() : actor(junk) {}

    void setCallback(Actor &a) { actor = a;}

    void callCallback() { doCallback(actor); }
    
  protected:
    virtual void doCallback(Actor &a) {}
    
    Actor &actor;
};

class Box {
  public:
    Box(Window &w, Point p, int height, int width, unsigned char index);

    int contains(Point p);

    Point upperLeft() { return(origin); }

    int width() { return(w); }

    int height() { return(h); }

  protected:
    Window *wdw;
    Point origin;
    int w;
    int h;
};

class Button : public Box, Actor {
  public:
    Button(Window &w, Point p, int height, int width, char *l,
           unsigned char index);

    void label(char *l);

    void setCallback(Callback *c)
      {
          callee = c;
          c->setCallback(*this);
      }

    void doCallback() { callee->callCallback(); }

  protected:
    Callback *callee;

};

class CBox : public Box {
  public:
    CBox(Window w, Point p, int height, int width,
            unsigned char index) : Box(w,p,height,width,index) {}

    int color() { return(offset); }

  private:
    int offset;
};

class ColorEditor : public Callback {
  public:
    ColorEditor(GraphicController &ctlr, char *file = NULL);

    void run();

    ~ColorEditor();
    
  private:
    void doCallback(Actor &a);
    
    void load(Button &j);
    void save(Button &j);
    void quit(Button &j);
    void undo(Button &j);
    void adjustColors(Button &caller);

    void lines(CBox &b, unsigned char color);
    
    void markBox(CBox &b) { lines(b,WHITE1); }

    void unmarkBox(CBox &b) { lines(b,BLACK); }

    void setColors() { agc->setColorMap(currentColors,256); }
    
    void redUp()
      { if ((currentColors[index] & 0xff) != 0xff) currentColors[index]++; }
    void greenUp()
      {
          if ((currentColors[index] & 0xff00) != 0xff00)
            currentColors[index] += 0x100;
      }
    void blueUp()
      {
          if ((currentColors[index] & 0xff0000) != 0xff0000)
            currentColors[index] += 0x10000;
      }
    void redDown()
      { if (currentColors[index] & 0xff) currentColors[index]--; }
    void greenDown()
      { if (currentColors[index] & 0xff00) currentColors[index] -= 0x100; }
    void blueDown()
      {
          if (currentColors[index] & 0xff0000)
            currentColors[index] -= 0x10000;
      }
    
    
    CBox *boxes[96];

    Button *redU;
    Button *redD;
    Button *greenU;
    Button *greenD;
    Button *blueU;
    Button *blueD;
    Button *Load;
    Button *Save;
    Button *Quit;
    Button *Undo;

    GraphicController *agc;
    Window wdw;
    Mouse *mouse;
    
    long oldColors[256];
    long currentColors[256];
    int index;
    int quitingTime;

    friend ostream& operator<<(ostream &os, ColorEditor &edt);
    friend istream& operator>>(istream &is, ColorEditor &edt);
};

#endif // _COLOREDITOR_HH
