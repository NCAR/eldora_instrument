//Written on July 20, 2007

#include "sa.h"

SA::SA(std::string ipAddress, int port, int radar):
   _radar(radar),
   _ether2(ipAddress, port)
{}
//////////////////////////////////////////////////////////////////

SA::~SA()
{}

///////////////////////////////////////////////////////////////////
void
SA::SetAtten(int attenval)
{

  unsigned char Address[2];
  unsigned char Select[2];
  unsigned char Data[2];

  //Set DS bit to 0 first with R/W=0
  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);

  //Fore or Aft Attenuator
  //Set A0-A3
  Address[0]='B';
  //xxxx (0/1)000
  //low-fore, high- aft
  Address[1]= _radar;
  SendToBoard(Address[0], Address[1]);

  //10-10 20-20 30-30 40-40 50-50 60-60 70-70 80-C0 90-D0 100-E0 110-F0
  //Set Attenuator Value
  Data[0]='A';
  Data[1]= attenval;
  SendToBoard(Data[0], Data[1]);


  //Set DS bit to 1 first with R/W=0
  Select[0]='C';
  Select[1]= 0x10;
  SendToBoard(Select[0], Select[1]);

  //Set DS bit to 0 first with R/W=0
  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);

  return;

}
//////////////////////////////////////////////////////////////////////////////
void
SA::SendToBoard(unsigned char portletter, unsigned char data)
{

  unsigned char send[5];

  send[0] ='!';
  send[1] = portletter;
  send[2] = 0x00;
  send[3] = portletter;
  send[4] = data;
    
  _ether2.Output(send,5);
}
//////////////////////////////////////////////////////////////////////////////

int
SA::ChgAttenVal(int attenval)
{

  int chgval = 0;
  int d = 0;

  //Brute Force method of changing the values
  if(attenval >= 0 && attenval <= 7 || attenval >= 10 && attenval <= 17 || attenval >= 20 && attenval <= 27 || attenval >= 30 && attenval <= 37
      || attenval >= 40 && attenval <= 47 || attenval >= 50 && attenval <= 57 || attenval >= 60 && attenval <= 67 || attenval >= 70 && attenval <= 77)
  {
    d = attenval/10;
    chgval = attenval + 6 * d;

  }

  if(attenval == 8 || attenval == 9 || attenval == 18 || attenval == 19 || attenval == 28 || attenval == 29 || attenval == 38 || attenval == 39 || attenval == 48 || attenval == 49 || attenval == 58 || attenval == 59 || attenval == 68 || attenval == 69 || attenval == 78 || attenval == 79)
  {
    d = attenval/10;
    chgval = attenval + 6 * d + 4;
  }


  if(attenval >= 80 && attenval <= 87 || attenval >= 90 && attenval <= 97 || attenval >= 100 && attenval <= 107 || attenval >= 110 && attenval <= 117)
  {
    d = attenval/10;
    chgval = attenval + 6 * (d-8) + 112;
  }

  if(attenval == 88 || attenval == 89 || attenval == 98 || attenval == 99 || attenval == 108 || attenval == 109)
  {
    chgval = attenval + 6 * (d-8) + 116;
  }


  if(attenval >= 118 && attenval <= 121)
  {
    chgval = attenval + 134;

  }


  return chgval;
}
