//Stepped Attenuator Menu Class
//Written July 20, 2007

#ifndef SAMENU_H
#define SAMENU_H

#include "sa.h"

/**
	@author Drew
*/
class SAMenu
{
public:

  ///Constructor
  /// @param sa object of SA class
  SAMenu(SA& sa);

  ///Destructor
  ~SAMenu();

protected:

  ///Object of SA Class
  SA& _step;

  ///Setup of Stepped Attenuator Menu
  void OpenMenu();

  ///Value of attenuation
  int _attenval;



};

#endif