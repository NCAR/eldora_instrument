
#include "trigmux.h"

TrigMux::TrigMux(std::string ipAddress, int port, int radar):
    _radar(radar),
    _ether3(ipAddress, port)
{}

/////////////////////////////////////////////////////////////

TrigMux::~TrigMux()
{}

////////////////////////////////////////////////////////////
void
TrigMux::SetMux(int channelnum, int countervalue)
{

  int channelnumber=0;
  int q=0;
  unsigned char Address[2];
  unsigned char Select[2];
  unsigned char Data[2];



  //Activates Master Reset to Stop Previous Signal
  Address[0]='B';
  //Makes sure that ADDR<3> either stays 0 or 1 for fore or aft
  Address[1]= _radar | 0x03;
  SendToBoard(Address[0], Address[1]);

  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);


  /////////////Select a non-registered Signal First
  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);

  //Set Address Bits to Control Decoder
  Address[0]='B';
  //Seting up for W1, A2-A0 = 001
  Address[1]= _radar | 0x01;
  SendToBoard(Address[0], Address[1]);

  //Set Data Bits
  Data[0]='A';
  //Set M to all 1s
  Data[1]= 0xFF;
  SendToBoard(Data[0], Data[1]);

  //Toggle DS to high to activate W1
  Select[0]='C';
  Select[1]= 0x10;
  SendToBoard(Select[0], Select[1]);

  //Deactivate W1
  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);



  //////////Load Counting Data- Sampling Rate
  //Set Address Bits to Control Decoder
  Address[0]='B';
  //Seting up for W2, A2-A0 = 010
  Address[1]= _radar | 0x02;
  SendToBoard(Address[0], Address[1]);


  //Set Data Bits
  Data[0]='A';
  //0x01-every 2nd pulse, 0x02-every 3rd pulse, etc.
  Data[1] = countervalue;
  SendToBoard(Data[0], Data[1]);


  //Toggle DS to activate W2
  //Latch Data
  Select[0]='C';
  Select[1]= 0x10;
  SendToBoard(Select[0], Select[1]);


  //Set XTR to high , load data from data latches
  //Set W4 to low
  Address[0]='B';
  //Seting up for W4, A2-A0 = 100
  Address[1]= _radar | 0x04;
  SendToBoard(Address[0], Address[1]);

  //Toggle DS back to 0
  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);



  //////////Choose Desired Channel
  channelnumber = channelnum -1;
  //Combines the radar selection with the channel number
  q = _radar | channelnumber;

  Select[0]='C';
  Select[1]= 0x00;
  SendToBoard(Select[0], Select[1]);

  //Set Address Bits to Control Decoder
  Address[0]='B';
  //Seting up for W1, A2-A0 = 001
  //Also sets XTR to low, so it can start count down
  Address[1]= _radar | 0x01;
  SendToBoard(Address[0], Address[1]);

  //Set Data Bits to Select Desired Channel Number
  Data[0]='A';
  //Channel 1-M<0:3>=000, Channel 2-M<0:3>=001, etc.
  //Set M<4:7> to high
  Data[1]= 0xF0 | q;
  SendToBoard(Data[0], Data[1]);

  //Toggle DS to high to activate W1
  Select[0]='C';
  Select[1]= 0x10;
  SendToBoard(Select[0], Select[1]);


  return;

}

/////////////////////////////////////////////
void
TrigMux::SendToBoard(unsigned char portletter, unsigned char data)
{

  unsigned char send[5];

  send[0] ='!';
  send[1] = portletter;
  send[2] = 0x00;
  send[3] = portletter;
  send[4] = data;
  _ether3.Output(send,5);
}
//////////////////////////////////////////////
