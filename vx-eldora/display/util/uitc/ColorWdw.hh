/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 ColorWdw.hh
 *	Original Author: Richard E. Neitzel
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
#ifndef INCColorWdwhh
#define INCColorWdwhh

#include "BaseWindow.h"

#include "ComponentDisplay.h"

#include "AlphanumericInput.h"
#include "Button.h"
#include "Slider.h"
#include "ToggleChoice.h"
#include "TextItem.h"

#include "Notifier.h"

#include "ColorMap.h"

#include "UIWrapper.hh"
#include "FileBrowser.hh"

#define UNIX
#define CLIENT_SIDE
#include "ColorSet.hh"

class ColorWdw : public UIWrapper, private ColorSet {
  private:
    BaseWindow Base;
    ComponentDisplay Cntrl;
    AlphanumericInput FileName;
    Button SaveButton;
    Button LoadButton;
    Button BrowseButton;
    Button Quit;
    ToggleChoice ColorBins;
    TextItem RgbMsg;
    Slider RedSlider;
    Slider GreenSlider;
    Slider BlueSlider;
    Button UndoButton;
    Slider ColorIndex;
    TextItem ColorMsg;
    ComponentDisplay TheColor;

    ColorMap Cmap;

    FileBrowser *Browser;

    Boolean active;

    ColorWdw *getThis(UIObject *obj)
      {
          return((ColorWdw *)UIWrapper::getThis(obj));
      }

    void SaveHandler(UIObject *);
    void LoadHandler(UIObject *);
    void BrowseHandler(UIObject *);
    void QuitHandler(UIObject *);
    void ColorBinsHandler(UIObject *);
    void ColorHandler(UIObject *);
    void UndoHandler(UIObject *);
    void ColorIndexHandler(UIObject *);

    void RepaintHandler(UIObject *);

    void Repaint(void);

    void BuildMap(void);
  public:
    ColorWdw(int argc, char **argv, char *file = NULL);

    int Connect(char *target);

    void Load(char *file = NULL);

    void Save(char *file = NULL);

    void Callback(void *ptr);
};
    
#endif // INCColorWdwhh

