/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 BaseWdw.hh
 *	Original Author: Richard E. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1994/03/24  16:20:10  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
#ifndef INCBaseWdw_hh
#define INCBaseWdw_hh


#include "BaseWindow.h"
#include "ComponentDisplay.h"
#include "TextItem.h"
#include "Button.h"
#include "Gauge.h"
#include "Notifier.h"

#include "UIWrapper.hh"
#include "UIIcon.hh"

#include "RecordComm.hh"
#include "StatusWdw.hh"

class BaseWdw : public UIWrapper {
  public:
    BaseWdw(int argc, char **argv);

    int startRpc(char *target);

    void updateTime(int seconds);

    static void update(BaseWdw *);

    void setSys0(StatusWdw &wdw) { sys0 = &wdw; }
    void setSys1(StatusWdw &wdw) { sys1 = &wdw; }

    void setMeter(int percent);

    void doStatus(TapeStatus *status);
  
  private:
    BaseWindow MainWdw;
    ComponentDisplay Ctrl1;
    TextItem Message1;
    Button Zero;
    Button One;
    TextItem Active;
    ComponentDisplay Ctrl2;
    Button RecordOn;
    Button RecordOff;
    Button Log;
    Button Rewind;
    Button Eject;
    ComponentDisplay Ctrl3;
    Gauge Meter;
    Button Quit;

    RecordComm targetSys;

    UIIcon Ok;
    UIIcon Bad;
    UIIcon Eot;
    UIIcon *currIcon;
    
    StatusWdw *sys0;
    StatusWdw *sys1;

    BaseWdw *getThis(UIObject *obj)
      {
          return((BaseWdw *)UIWrapper::getThis(obj));
      }

    // Callbacks:
    void ZeroHandler(UIObject *obj);
    void OneHandler(UIObject *obj);
    void RecordOnHandler(UIObject *obj);
    void RecordOffHandler(UIObject *obj);
    void LogHandler(UIObject *obj);
    void RewindHandler(UIObject *obj);
    void EjectHandler(UIObject *obj);
    void QuitHandler(UIObject *obj);

    void warning()
      {
          Ctrl2.show(FALSE);
          Ctrl2.setBackgroundColor("Red");
          Bad.Attach(MainWdw,"Red");
          currIcon = &Bad;
          Ctrl2.show(TRUE);
      }

    void fine()
      {
          Ctrl2.show(FALSE);
          Ctrl2.setBackgroundColor("Aquamarine");
          Ok.Attach(MainWdw);
          currIcon = &Ok;
          Ctrl2.show(TRUE);
      }          
};
#endif // INCBaseWdw_hh
