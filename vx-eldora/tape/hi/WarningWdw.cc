//  This looks like C, but it's really C++!!
//	$Id$
//
//	Module:		 WarningWdw.cc
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

#include "WarningWdw.hh"

WarningWdw::WarningWdw()
{
    WarningPopup.setWidth(400);
    WarningPopup.setHeight(199);
    WarningPopup.setLabel("Warning!!!!!!");
    WarningPopup.show(FALSE);
    WarningPopup.setDisplayFooter(TRUE);
    WarningPopup.setResizable(TRUE);
    WarningPopup.setPinned(FALSE);

    Ctrl.setX(0);
    Ctrl.setY(0);
    Ctrl.setExtendToEdge(WIDTH);
    Ctrl.setExtendToEdge(HEIGHT);
    Ctrl.setBackgroundColor("Aquamarine");
    Ctrl.setDisplayBorders(FALSE);
    WarningPopup.addDisplay(Ctrl);

    Warn1.setX(156);
    Warn1.setY(48);
    Warn1.setLabel("Warning! ");
    Warn1.setLabelBold(TRUE);
    Ctrl.addComponent(Warn1);

    Warn2.setX(72);
    Warn2.setY(84);
    Warn2.setLabel("Soft Errors exceeded  either maximum");
    Warn2.setLabelBold(TRUE);
    Ctrl.addComponent(Warn2);

    Warn3.setX(124);
    Warn3.setY(120);
    Warn3.setLabel("limit or burst limit.");
    Warn3.setLabelBold(TRUE);
    Ctrl.addComponent(Warn3);

    Close.setX(144);
    Close.setY(152);
    Close.setLabel("Aknowledged");
    Close.setNotifyHandler((UICHandlerFunction)&WarningWdw::CloseHandler);
    setThis(Close);
    Ctrl.addComponent(Close);
}

void WarningWdw::CloseHandler(UIObject *obj)
{
    obj = getObject(this);
    this = getThis(obj);

    WarningPopup.show(FALSE);
}
