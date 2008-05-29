
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
    printf("(0) Fore Channel 1\n");
    printf("(1) Fore Channel 2\n");
    printf("(2) Fore Channel 3\n");
    printf("(3) Fore Channel 4\n");
    printf("(4) Fore Channel 5\n");
    printf("(5) Fore PREKNOCK \n");
    printf("(6) Fore TESTPULSE\n");
    printf("(7) Aft Channel 1\n");
    printf("(8) Aft Channel 2\n");
    printf("(9) Aft Channel 3\n");
    printf("(10) Aft Channel 4\n");
    printf("(11) Aft Channel 5\n");
    printf("(12) Aft PREKNOCK \n");
    printf("(13) Aft TESTPULSE\n");

    printf("or enter (-1) to exit\n\n");
    printf("Enter Choice; ");
    scanf("%d",&channel);

    if(channel == -1)
    {
      break;
    }

    if(channel < 0 || channel > 13)
    {
      printf("Channel number must be between 0 and 13!\n");
      printf("You entered %d\n",channel);
    }


/*    while(1)
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
    
    } */
    //Set Counter Value (a.k.a Trigger Mux Pulse Divider) to 2 as is done in old ECB code.
    _countval = 2;
    _mux.SetMux(channel, _countval);

  }
}
