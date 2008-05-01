/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 WarningWdw.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1994/03/24  16:20:13  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
#ifndef INCWarningWdw_hh
#define INCWarningWdw_hh


#include "PopUpWindow.h"
#include "ComponentDisplay.h"
#include "TextItem.h"
#include "Button.h"
#include "Notifier.h"

#include "UIWrapper.hh"
#include "UIIcon.hh"

class WarningWdw : public UIWrapper {
  public:
    WarningWdw();

    void show() { WarningPopup.show(TRUE); }
    void unshow() { WarningPopup.show(FALSE); }

  private:
    PopUpWindow WarningPopup;
    ComponentDisplay Ctrl;
    TextItem Warn1;
    TextItem Warn2;
    TextItem Warn3;
    Button Close;

    WarningWdw *getThis(UIObject *obj)
      {
          return((WarningWdw *)UIWrapper::getThis(obj));
      }

    // Callbacks:
    void CloseHandler(UIObject *object);
};
#endif // INCWarningWdw_hh
