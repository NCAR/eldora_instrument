//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 BaseWdw.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
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

#include "BaseWdw.hh"
#include "tape.image.h"
#include "tape.err.image.h"


BaseWdw::BaseWdw(int argc, char **argv) : Ok(64,64,tapeImageBits),
Bad(64,64,tapeerrImageBits)
{
    cout << argc << " " << argv[0] << endl;
    MainWdw.initUI(argc,argv);
    MainWdw.setWidth(563);
    MainWdw.setHeight(438);
    MainWdw.setBackgroundColor("Aquamarine");
    MainWdw.setLabel("Recording  Control");
    MainWdw.show(TRUE);
    MainWdw.setDisplayFooter(TRUE);
    MainWdw.setResizable(TRUE);
    MainWdw.setIcon(Bad);
    
    Ctrl1.setX(0);
    Ctrl1.setY(0);
    Ctrl1.setExtendToEdge(WIDTH);
    Ctrl1.setHeight(144);
    Ctrl1.setBackgroundColor("Aquamarine");
    Ctrl1.setDisplayBorders(TRUE);
    MainWdw.addDisplay(Ctrl1);

    Message1.setX(116);
    Message1.setY(32);
    Message1.setLabel("View the current status for recording system:");
    Message1.setLabelBold(TRUE);
    Ctrl1.addComponent(Message1);

    Zero.setX(172);
    Zero.setY(68);
    Zero.setLabel("   0   ");
    Ctrl1.addComponent(Zero);

    One.setX(296);
    One.setY(68);
    One.setLabel("   1   ");
    Ctrl1.addComponent(One);

    Active.setX(144);
    Active.setY(112);
    Active.setLabel("Currently active system is system 0");
    Active.setLabelBold(TRUE);
    Ctrl1.addComponent(Active);

    Ctrl2.setX(0);
    Ctrl2.setY(144);
    Ctrl2.setExtendToEdge(WIDTH);
    Ctrl2.setHeight(136);
    Ctrl2.setBackgroundColor("Red");
    Ctrl2.setDisplayBorders(TRUE);
    MainWdw.addDisplay(Ctrl2);

    Record.setX(228);
    Record.setY(24);
    Record.setForegroundColor("Red");
    Record.setLabel("Recording off");
    Ctrl2.addComponent(Record);

    Log.setX(72);
    Log.setY(72);
    Log.setLabel("Record log file");
    Ctrl2.addComponent(Log);

    Rewind.setX(244);
    Rewind.setY(72);
    Rewind.setLabel("Rewind");
    Ctrl2.addComponent(Rewind);

    Eject.setX(404);
    Eject.setY(72);
    Eject.setLabel("Eject");
    Ctrl2.addComponent(Eject);
    
    Ctrl3.setX(0);
    Ctrl3.setY(280);
    Ctrl3.setExtendToEdge(WIDTH);
    Ctrl3.setExtendToEdge(HEIGHT);
    Ctrl3.setBackgroundColor("Aquamarine");
    Ctrl3.setDisplayBorders(TRUE);
    MainWdw.addDisplay(Ctrl3);

    Meter.setX(152);
    Meter.setY(20);
    Meter.setGaugeWidth(200);
    Meter.setTicks(10);
    Meter.setLabel("Percent of recording media used:");
    Meter.setLabelPosition(ABOVE);
    Meter.setOrientation(HORIZONTAL);
    Meter.setDisplayRange(TRUE);
    Meter.setMinValue(0);
    Meter.setMaxValue(100);
    Ctrl3.addComponent(Meter);

    Quit.setX(232);
    Quit.setY(116);
    Quit.setLabel("Quit");
    Ctrl3.addComponent(Quit);

    Zero.setNotifyHandler((UICHandlerFunction)&BaseWdw::ZeroHandler);
    setThis(Zero);
    One.setNotifyHandler((UICHandlerFunction)&BaseWdw::OneHandler);
    setThis(One);
    Record.setNotifyHandler((UICHandlerFunction)&BaseWdw::RecordHandler);
    setThis(Record);
    Log.setNotifyHandler((UICHandlerFunction)&BaseWdw::LogHandler);
    setThis(Log);
    Rewind.setNotifyHandler((UICHandlerFunction)&BaseWdw::RewindHandler);
    setThis(Rewind);
    Eject.setNotifyHandler((UICHandlerFunction)&BaseWdw::EjectHandler);
    setThis(Eject);
    Quit.setNotifyHandler((UICHandlerFunction)&BaseWdw::QuitHandler);
    setThis(Quit);

    recordingState = 0;         // Not recording.
}

void BaseWdw::ZeroHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    sys0->show();
}

void BaseWdw::OneHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    sys1->show();
}

void BaseWdw::RecordHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    TapeStatus *status;
    
    if (recordingState)         // Recording is on.
      {
          status = targetSys.recordOn();
          if (status == NULL)
            {
                NoticePrompt note;
    
                note.setMessage("Recording system failed to accept record command");
                note.addButtonLabel("Noted");
                note.show(obj);
            }
          else
            {
                sys0->update(status);
                sys1->update(status);
                recordingState = 0;
                warning();
            }
      }
    else
      {
          status = targetSys.recordOff();
          if (status == NULL)
            {
                NoticePrompt note;
    
                note.setMessage("Recording system failed to accept record command");
                note.addButtonLabel("Noted");
                note.show(obj);
            }
          else
            {
                sys0->update(status);
                sys1->update(status);
                recordingState = 1;
                fine();
            }
      }
}

void BaseWdw::LogHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    TapeStatus *status = targetSys.log();
    if (status == NULL)
      {
          NoticePrompt note;
    
          note.setMessage("Recording system failed to accept log command");
          note.addButtonLabel("Noted");
          note.show(obj);
      }
    else
      {
          sys0->update(status);
          sys0->update(status);
      }
}

void BaseWdw::RewindHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    TapeStatus *status = targetSys.rewind();
    if (status == NULL)
      {
          NoticePrompt note;
    
          note.setMessage("Recording system failed to accept rewind command");
          note.addButtonLabel("Noted");
          note.show(obj);
      }
    else
      {
          sys0->update(status);
          sys0->update(status);
      }
}

void BaseWdw::EjectHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    TapeStatus *status = targetSys.eject();
    if (status == NULL)
      {
          NoticePrompt note;
    
          note.setMessage("Recording system failed to accept eject command");
          note.addButtonLabel("Noted");
          note.show(obj);
      }
    else
      {
          sys0->update(status);
          sys0->update(status);
      }
}

void BaseWdw::QuitHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    NoticePrompt note;
    
    note.setMessage("Really quit?");
    
    note.setYesAndNoLabels("Quit","Cancel");
    
    if (note.show(obj) == NOTICE_YES)
      exit(0);
}

int BaseWdw::startRpc(char *target)
{
    return(targetSys.start(target));
}

void BaseWdw::updateTime(int seconds)
{
    struct itimerval tvalue;

    tvalue.it_interval.tv_sec = seconds;
    tvalue.it_interval.tv_usec = 0;
  
    tvalue.it_value = tvalue.it_interval;

    notify_set_itimer_func((Notify_client)this,(Notify_func)BaseWdw::update,
			   ITIMER_REAL,&tvalue,NULL);
}

void BaseWdw::setMeter(int percent)
{
    if (percent >= 95)
      Ctrl3.setBackgroundColor("Yellow");
    else
      Ctrl3.setBackgroundColor("Aquamarine");

    Meter.setValue(percent);
}

void BaseWdw::update(BaseWdw *realWdw)
{
    TapeStatus *status = realWdw->targetSys.getStatus();

    if (status == NULL)
      realWdw->warning();
    else
      {
          realWdw->sys0->update(status);
          realWdw->sys1->update(status);

          int percent = 100 - status->eot_warning;

          realWdw->setMeter(percent);                
      }
}
