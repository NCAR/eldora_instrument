
#include "trigmuxmenu.h"

TrigMuxMenu::TrigMuxMenu(TrigMux& trigmux):
    _mux(trigmux)
{

  OpenMenu();

}

////////////////////////////////////////
TrigMuxMenu::~TrigMuxMenu()
{}

///////////////////////////////////////
void
TrigMuxMenu::OpenMenu()
{

  int channel=0;

  while(1)
  {
    printf("\n");
    printf("----------TRIGGER MUX Channel Menu----------\n\n");
    printf("Which Channel would you like to read: \n");
    printf("(1) Channel 1\n");
    printf("(2) Channel 2\n");
    printf("(3) Channel 3\n");
    printf("(4) Channel 4\n");
    printf("(5) Channel 5\n");
    printf("(6) PREKNOCK \n");
    printf("(7) TESTPULSE\n");
    printf("or enter (-1) to exit\n\n");
    printf("Enter Choice; ");
    scanf("%d",&channel);

    if(channel == -1)
    {
      break;
    }

    if(channel < 1 || channel > 7)
    {
      printf("Channel number must be between 1 and 7!\n");
      printf("You entered %d\n",channel);
    }


    while(1)
    {
      printf("\n");
      printf("----------TRIGGER MUX Counter Value Menu----------\n\n");
      printf("Enter the count value for the Trigger Mux (0-255)\n");
      printf("or enter (-1) to exit:\n\n");
      printf("Enter Count Value: ");
      scanf("%d",&_countval);

      if(_countval == -1)
      {
        break;
      }

      if(_countval < 0 || _countval > 255)
      {
        printf("Attenuator value out of range!\n");
        printf("You entered %d\n",_countval);
      }
      else
      {
        //Set Trigger Mux with User Inputs
        _mux.SetMux(channel, _countval);
      }
    }




  }
}