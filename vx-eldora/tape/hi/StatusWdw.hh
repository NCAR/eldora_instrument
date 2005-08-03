/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 StatusWdw.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1994/03/24  16:20:12  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
#ifndef INCStatusWdw_hh
#define INCStatusWdw_hh


#include "PopUpWindow.h"
#include "ComponentDisplay.h"
#include "ToggleChoice.h"
#include "AlphanumericInput.h"
#include "Button.h"
#include "Notifier.h"

#include "UIWrapper.hh"
#include "UIIcon.hh"

#include "WarningWdw.hh"

#include <strstream.h>
#ifndef UNIX
#define UNIX
#define OK_RPC
#define CLIENT_SIDE

extern "C" {
#include "tapeControl.h"
};
#endif // UNIX

class StatusWdw : public UIWrapper {
  public:
    StatusWdw(int which);

    void update(TapeStatus *status);

    void show() { StatusPopup.show(TRUE); }

  private:
    PopUpWindow StatusPopup;
    ComponentDisplay Ctrl1;
    ToggleChoice Current;
    ToggleChoice Drives;
    ComponentDisplay Ctrl2;
    AlphanumericInput Hex;
    AlphanumericInput Failures;
    AlphanumericInput Attempts;
    ComponentDisplay Ctrl3;
    Button Clear;
    Button Close;

    WarningWdw softErr;
    
    int whichSys;
    u_long totalFailed;

    char buffer[80];
    ostrstream toascii;

    StatusWdw *getThis(UIObject *obj)
      {
          return((StatusWdw *)UIWrapper::getThis(obj));
      }

    // Callbacks:
    void ClearHandler(UIObject *object);
    void CloseHandler(UIObject *object);

    // These values are strictly a guess for now!
    enum error_limits { soft_limit = 20000, burst_limit = 1500 };
};
#endif // INCStatusWdw_hh
