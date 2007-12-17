
#include "samenu.h"

SAMenu::SAMenu(SA& sa):
    _step(sa)
{

  OpenMenu();

}

///////////////////////////////////
SAMenu::~SAMenu()
{}

////////////////////////////////////
void
SAMenu::OpenMenu()
{
_attenval = 0;
int atten=0;

  while(1)
  {
    printf("\n");
    printf("The previously programmed value was: %d \n",_attenval);
    printf("Enter value to attenuate from 1-120 (dB) or enter (-1) to exit menu\n");
    scanf("%d",&_attenval);

    if(_attenval == -1)
    {
      break;
    }
    else
    {
      if(_attenval < 0 || _attenval > 121)
      {
        printf("Attenuator value out of range!\n");
        printf("You entered %d\n",_attenval);
      }
      else
      {
        //Change User entered value into attenuator readable value
        atten = _step.ChgAttenVal(_attenval);
        //Set attenuator to attenuator value
        _step.SetAtten(atten);

      }
    }
  }
  return;
}
////////////////////////////////////

////////////////////////////////////

////////////////////////////////////

////////////////////////////////////