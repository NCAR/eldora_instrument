
#ifndef TRIGMUXMENU_H
#define TRIGMUXMENU_H

#include "trigmux.h"

/**
	@author Drew
*/
class TrigMuxMenu
{
public:
  ///Constructor
  /// @param trigmux object of TrigMux Class
  TrigMuxMenu(TrigMux& trigmux);

  ///Destructor
  ~TrigMuxMenu();


protected:

  ///Object of TrigMux Class
  TrigMux& _mux;

  ///Setup of TrigMux Menu
  void OpenMenu();

  ///Counter Value
  int _countval;

};

#endif
