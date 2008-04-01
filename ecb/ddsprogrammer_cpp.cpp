//Drew Leung with help by Charlie Martin
//June 21/22/23/26, 2007

#include <stdio.h>
#include <iostream>
#include "dds.h"
#include "ddsmenu.h"
#include "sa.h"
#include "samenu.h"
#include "trigmuxmenu.h"

using namespace std;

int main()
{

  int ddsunit;
  int numdds=0;
  int device;
  int radarchoice;
  DDS* dds[numdds];
  DDSMenu* menu;
  int d;

  while(1)
  {

    printf("\n");
    cout<<"----------ELDORA RCVR/EXCITER and STEPPED ATTENUATOR CHASSIS PROGRAM------------\n";
    cout<<"\n";
    cout<<"Would you like to program: \n";
    cout<<"(0) Digital Synthesizers\n";
    cout<<"(1) Stepped Attenuators\n";
    cout<<"(2) Trigger Mux?\n\n";
    cout<<"Enter Choice: ";
    cin>> device;
    //Need this function to absorb the character return
    getchar();


    if(device < 0 || device > 2)
    {
      printf("INPUT ERROR! You entered %d\n", device);
      printf("Device Value must be 0, 1, or 2!\n");
    }
    else
    {

      if(device == 0)
      {
        while(1)
        {
          cout<<"\n----------DDS Home Menu-----------------\n\n";
          cout<<"Do you want to program the: \n";
          cout<<"(0) Fore \n";
          cout<<"(1) Aft \n";
          cout<<"Or Enter (-1) to exit\n\n";
          cout<<"Enter Choice: ";
          cin >> radarchoice;
          getchar();
          if(radarchoice < -1 || radarchoice > 1)
          {
            printf("ERROR! You entered %d . Value must be either -1, 0 or 1 !\n",radarchoice);
          }
          else
          {

            if(radarchoice == -1)
            {
              break;
            }
            //Setup of DDS Fore
            if(radarchoice == 0)
            {
              cout<<"\nEnter number of DDS units in the Fore Radar (1-15): ";
              cin >>numdds;
              getchar();
              for(ddsunit=0;ddsunit<numdds;ddsunit++)
              {
                dds[ddsunit] = new DDS("etherio-fore", 2424, ddsunit);

              }

              menu = new DDSMenu(dds, numdds);
            }
            //Setup of DDS Aft
            if(radarchoice ==1)
            {
              cout<<"Enter number of DDS units in the Aft Radar (1-15):";
              cin >>numdds;
              getchar();
              for(ddsunit=0;ddsunit<numdds;ddsunit++)
              {
                dds[ddsunit] = new DDS("etherio-aft", 2424, ddsunit);

              }

              menu = new DDSMenu(dds, numdds);
            }
          }
        }
      }


      if(device == 1)
      {
        while(1)
        {
          cout<<"\n----------STEPPED ATTENUATOR Home Menu-----------------\n\n";
          cout<<"Do you want to program the: \n";
          cout<<"(0) Fore \n";
          cout<<"(1) Aft \n";
          cout<<"Or Enter (-1) to exit\n\n";
          cout<<"Enter Choice: ";

          cin >> radarchoice;
          if(radarchoice < -1 || radarchoice > 1)
          {
            printf("ERROR! You entered %d . Value must be either -1, 0 or 1 !\n",radarchoice);
          }
          else
          {
            if(radarchoice == -1)
            {
              break;
            }
            radarchoice = radarchoice * 8;

            SA sa("etherio-sa", 2424, radarchoice);

            SAMenu menu(sa);


          }
        }
      }

      if(device == 2)
      {
        while(1)
        {
          cout<<"\n----------TRIGGER MUX Home Menu-----------------\n\n";
          cout<<"Do you want to see channels from the:\n";
          cout<<"(0) Fore \n";
          cout<<"(1) Aft \n";
          cout<<"Or Enter (-1) to exit\n\n";
          cout<<"Enter Choice: ";
          cin >> radarchoice;
          if(radarchoice < -1 || radarchoice > 1)
          {
            printf("ERROR! You entered %d . Value must be either -1, 0 or 1 !\n",radarchoice);
          }
          else
          {
            if(radarchoice == -1)
            {
              break;
            }
            radarchoice = radarchoice * 8;

            TrigMux mux("etherio-sa", 2424, radarchoice);

            TrigMuxMenu menu(mux);


          }
        }
      }

    }

  }
  cout<<"Program exiting\n";

}
