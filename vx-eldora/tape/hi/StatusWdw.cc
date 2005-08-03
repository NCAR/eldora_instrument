//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 StatusWdw.cc
//	Original Author: Richard E. Neitzel
//      Copywrited by the National Center for Atmospheric Research
//	Date:		 $Date$
//
// revision history
// ----------------
// $Log$
// Revision 1.1  1994/03/24  16:20:08  thor
// Initial revision
//
//
//
// description:
//        
//
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "StatusWdw.hh"

StatusWdw::StatusWdw(int which) : toascii(buffer,80,ios::out)
{
    StatusPopup.setWidth(512);
    StatusPopup.setHeight(349);
    StatusPopup.setBackgroundColor("Aquamarine");
    if (which == 0)
      StatusPopup.setLabel("Status for System 0");
    else
      StatusPopup.setLabel("Status for System 1");
    StatusPopup.show(FALSE);
    StatusPopup.setDisplayFooter(TRUE);
    StatusPopup.setResizable(TRUE);
    StatusPopup.setPinned(FALSE);

    Ctrl1.setX(0);
    Ctrl1.setY(0);
    Ctrl1.setExtendToEdge(WIDTH);
    Ctrl1.setHeight(128);
    Ctrl1.setBackgroundColor("Aquamarine");
    Ctrl1.setDisplayBorders(FALSE);
    StatusPopup.addDisplay(Ctrl1);

    Current.setMultipleChoice(FALSE);
    Current.setX(108);
    Current.setY(16);
    Current.setDisplayedRows(1);
    Current.setLabelPosition(LEFT);
    Current.setSelectionRequired(TRUE);
    Current.setLabel("Current Recording system:");
    (void) Current.addChoice("Yes");
    (void) Current.addChoice("No");
    Ctrl1.addComponent(Current);

    Drives.setMultipleChoice(FALSE);
    Drives.setX(136);
    Drives.setY(68);
    Drives.setDisplayedRows(1);
    Drives.setLabelPosition(LEFT);
    Drives.setSelectionRequired(TRUE);
    Drives.setLabel("Active Drives:");
    (void) Drives.addChoice("1");
    (void) Drives.addChoice("2");
    (void) Drives.addChoice("3");
    (void) Drives.addChoice("4");
    Ctrl1.addComponent(Drives);

    Ctrl2.setX(0);
    Ctrl2.setY(128);
    Ctrl2.setExtendToEdge(WIDTH);
    Ctrl2.setHeight(180);
    Ctrl2.setBackgroundColor("Aquamarine");
    Ctrl2.setDisplayBorders(FALSE);
    StatusPopup.addDisplay(Ctrl2);

    Hex.setX(52);
    Hex.setY(32);
    Hex.setInputDisplayLength(12);
    Hex.setMaxInputLength(80);
    Hex.setLabel("Status word:");
    Hex.setLabelPosition(LEFT);
    Hex.setReadOnly(TRUE);
    Ctrl2.addComponent(Hex);

    Failures.setX(52);
    Failures.setY(68);
    Failures.setInputDisplayLength(12);
    Failures.setMaxInputLength(80);
    Failures.setLabel("Soft Failures:");
    Failures.setLabelPosition(LEFT);
    Failures.setReadOnly(TRUE);
    Ctrl2.addComponent(Failures);

    Attempts.setX(52);
    Attempts.setY(112);
    Attempts.setInputDisplayLength(12);
    Attempts.setMaxInputLength(80);
    Attempts.setLabel("Attempted Writes:");
    Attempts.setLabelPosition(LEFT);
    Attempts.setReadOnly(TRUE);
    Ctrl2.addComponent(Attempts);

    Ctrl3.setX(0);
    Ctrl3.setY(308);
    Ctrl3.setExtendToEdge(WIDTH);
    Ctrl3.setExtendToEdge(HEIGHT);
    Ctrl3.setBackgroundColor("Aquamarine");
    Ctrl3.setDisplayBorders(FALSE);
    StatusPopup.addDisplay(Ctrl3);

    Clear.setX(208);
    Clear.setY(12);
    Clear.setLabel("Clear Status");
    Clear.setNotifyHandler((UICHandlerFunction)&StatusWdw::ClearHandler);
    setThis(Clear);
    Ctrl3.addComponent(Clear);

    Close.setX(452);
    Close.setY(12);
    Close.setLabel("Close");
    Close.setNotifyHandler((UICHandlerFunction)&StatusWdw::CloseHandler);
    setThis(Close);
    Ctrl3.addComponent(Close);

    whichSys = which;
    totalFailed = 0;
}

void StatusWdw::update(TapeStatus *status)
{
    int which = whichSys;

    if (status->unit == which)
      {
          Current.setSelectedChoice("Yes",TRUE);
          Current.setSelectedChoice("No",FALSE);
      }
    else
      {
          Current.setSelectedChoice("Yes",FALSE);
          Current.setSelectedChoice("No",TRUE);
      }

    u_long drv1;
    u_long drv2;

    if (which == 0)
      {
	drv1 = status->drives0[0];
	drv2 = status->drives0[1];
      }
    else
      {
	drv1 = status->drives1[0];
	drv2 = status->drives1[1];
      }

    for (int i = 1; i <= 4; i++)
      Drives.setSelectedChoice(i,FALSE);

    if (drv1)
      Drives.setSelectedChoice(drv1,TRUE);

    if (drv2)
      Drives.setSelectedChoice(drv2,TRUE);
    
    toascii.seekp(0,ios::beg);

    hex(toascii);
    toascii << status->status[which];

    Hex.setValue(buffer);
    
    memset(buffer,0,sizeof(buffer)); // Force buffer to clear.

    toascii.seekp(0,ios::beg);

    u_long failed = status->failures[which];

    if (failed == 0xffffffff)
      totalFailed = 0;
    
    if ((failed - totalFailed) >= StatusWdw::burst_limit)
      softErr.show();

    totalFailed = failed;

    if (failed >= StatusWdw::soft_limit)
      softErr.show();

    dec(toascii);
    
    toascii << failed;

    Failures.setValue(buffer);

    memset(buffer,0,sizeof(buffer)); // Force buffer to clear.
    
    toascii.seekp(0,ios::beg);

    toascii << status->attempts[which];

    Attempts.setValue(buffer);

    memset(buffer,0,sizeof(buffer)); // Force buffer to clear.
}  
    
        
void StatusWdw::ClearHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    // Do what?
}

void StatusWdw::CloseHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    StatusPopup.show(FALSE);
}
