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

    void warning()
      {
          Ctrl2.setBackgroundColor("Red");
          Record.setForegroundColor("Red");
          Record.setLabel("Recording off");
          Bad.Attach(MainWdw,"Red");
      }

    void fine()
      {
          Ctrl2.setBackgroundColor("Aquamarine");
          Record.setForegroundColor("Black");
          Record.setLabel("Recording on");
          Ok.Attach(MainWdw);
      }

    void setMeter(int percent);
    
  private:
    BaseWindow MainWdw;
    ComponentDisplay Ctrl1;
    TextItem Message1;
    Button Zero;
    Button One;
    TextItem Active;
    ComponentDisplay Ctrl2;
    Button Record;
    Button Log;
    Button Rewind;
    Button Eject;
    ComponentDisplay Ctrl3;
    Gauge Meter;
    Button Quit;

    RecordComm targetSys;

    UIIcon Ok;
    UIIcon Bad;
    
    StatusWdw *sys0;
    StatusWdw *sys1;

    int recordingState;

    BaseWdw *getThis(UIObject *obj)
      {
          return((BaseWdw *)UIWrapper::getThis(obj));
      }

    // Callbacks:
    void ZeroHandler(UIObject *obj);
    void OneHandler(UIObject *obj);
    void RecordHandler(UIObject *obj);
    void LogHandler(UIObject *obj);
    void RewindHandler(UIObject *obj);
    void EjectHandler(UIObject *obj);
    void QuitHandler(UIObject *obj);
};
#endif // INCBaseWdw_hh
