//DDS class
//written by Charlie/Drew
//July 5,6, 2007
#include "dds.h"

DDS::DDS(std::string ipAddress, int port, int ddsunit):
    _ether(ipAddress, port),
    //Sets _ddsunit = ddsunit
    _ddsunit(ddsunit)
{}
//////////////////////////////////////////////////////////////////////////////

DDS::~DDS()
{}
//////////////////////////////////////////////////////////////////////////////
void
DDS::Toggle()
{

  _q =0;
  
  //Toggle DS bit to 0 so DDS can grab what is on the Bus
  _Select[1]='C';
  //0000 000(1/0); keep addr, DS=0, W/R=0
  _q = _DDS89; 
  _Select[2]= _q;
  SendToBoard(_Select[1], _Select[2]);

  //Toggle DS bit back to 1; can change address/data
  _Select[1]='C';
  _q = _DDS89 | 0x08;
  _Select[2]= _q;
  SendToBoard(_Select[1], _Select[2]);
}
//////////////////////////////////////////////////////////////////////////////
void
DDS::SendToBoard(unsigned char portletter, unsigned char data)
{

  unsigned char send[5];

  //Template of how to Write to the Ether I0 Ports
  //Write Command
  send[0] ='!';
  send[1] = portletter;
  //Set them as Outputs (0-output, 1-input)
  send[2] = 0x00;
  send[3] = portletter;
  //Data to be sent
  send[4] = data;

  _ether.Output(send,5);
}
//////////////////////////////////////////////////////////////////////////////
void
DDS::Rfinit()
{

  int i;
  _DDS67=0;
  _DDS89=0;
  _q=0;


  //Set Upper Four bits to select a DDS Unit (0x01 to 0x0F)
  SetDDSAddress();

  //////////Set SMC  Register (0x08) to 0x00
  //Set DS=1, W/R=0 before setting SMC register
  _Select[1]='C';
  //combine addr<8:9> with DS=1 and W/R=0; q= 0000 100(0/1)
  //Want to keep addr<8:9> bits while changing the DS bit 
  _q = _DDS89 | 0x08;
  _Select[2]=_q;
  SendToBoard(_Select[1], _Select[2]);

  //Set SMC register address
  _Address[1] = 'B';
  //combine addr<5:7> with register address 0x08
  //Keep addr<5:7> while changing Register Address
  _q = _DDS67 | 0x08; 
  _Address[2]=_q;
  SendToBoard(_Address[1], _Address[2]);

  //Set data being Sent to SMC
  _Data[1] = 'A';
  _Data[2] = 0x00; //for data<0:7>;
  SendToBoard(_Data[1], _Data[2]);

  Toggle();

  //////////Set PIRB Registers (0x07-0x04) to 0x00
  _Address[1] = 'B';

  for(i=0;i<=3;i++)
  {
    _q=0;

    //Register Address 7-> 4
    _Address[2]= 7-i;
    _q = _DDS67 | _Address[2];
    _Address[2] = _q;
    SendToBoard(_Address[1], _Address[2]);

    //Set  Data to 0x00
    _Data[1] = 'A';
    _Data[2] = 0x00; //for data<0:7>;
    SendToBoard(_Data[1], _Data[2]);

    Toggle();
  }

  //////////Set APR Register (0x0C) to 0x00
  //set register address
  _Address[1] = 'B';
  _q = _DDS67 | 0x0C; 
  _Address[2]=_q;
  SendToBoard(_Address[1], _Address[2]);

  _Data[1] = 'A';
  _Data[2] = 0x00; 
  SendToBoard(_Data[1], _Data[2]);

  Toggle();

  //////////Set ALL DDS2 Registers to 0x00
  _Address[1] = 'B';

  for(i=0;i<15;i++)
  {
    _q=0;
    //Address: 16($10)-31($1F) only setting reg address
    _Address[2]= 16+i; 
    _q = _DDS67 | _Address[2];
    _Address[2] = _q;
    SendToBoard(_Address[1], _Address[2]);


    //Set Data
    _Data[1] = 'A';
    _Data[2] = 0x00;
    SendToBoard(_Data[1], _Data[2]);

    Toggle();
  }

  //Reset all Data Address & Unselect DDS
  _Address[1] = 'B';
  _q = _DDS67 | 0x3F;
  _Address[2] = _q;
  SendToBoard(_Address[1], _Address[2]);


  _Data[1] = 'A';
  _Data[2] = 0x00; 
  SendToBoard(_Data[1], _Data[2]);

  return;
}
//////////////////////////////////////////////////////////////////////////////
void
DDS::Rfputval(double freqin, int nrin)
{

  int B=0;
  int T=0;
  int N=0;
  int temp;
  unsigned char Data1;
  unsigned char Data2;
  char T_hex[8];
  char M_hex[2];
  const char *hold;
  _DDS67 = 0;
  _DDS89 = 0;
  _q = 0;
  Data1=0;
  Data2=0;

  //Calculate B N T
  B = 361 - (int)(freqin/30e6);
  N = B + 1;
  T = (int)(pow(2,24)/(B+1.0)*(256.0*181/3 - freqin/703125.0) + 0.5);

  //Convert T and B into hex
  hold = dectohex(T).c_str();
  T_hex[7] = *(hold);
  T_hex[6] = *(hold + 1);
  T_hex[5] = *(hold + 2);
  T_hex[4] = *(hold + 3);
  T_hex[3] = *(hold + 4);
  T_hex[2] = *(hold + 5);
  T_hex[1] = *(hold + 6);
  T_hex[0] = *(hold + 7);

  hold = dectohex(B).c_str();
  M_hex[1] =*(hold + 6);
  M_hex[0] =*(hold + 7);

  //////////Set Upper Four Bits for DDS Address
  SetDDSAddress();
    
  //////////Set AMC Register (0x0A) to either 0x0C or 0x0F
  //Set DS=1, W/R=0 before setting AMC register
  _Select[1]='C';
  _q = _DDS89 | 0x08;
  _Select[2]= _q;
  SendToBoard(_Select[1],_Select[2]);

  //Set register address
  _Address[1] = 'B';
  _q = _DDS67 | 0x0A;
  _Address[2]= _q; ;
  SendToBoard(_Address[1],_Address[2]);

  if(nrin == 1)
  {

    _Data[1] = 'A';
    //Enable Noise Reduction Circuit
    _Data[2] = 0x0C; 
    SendToBoard(_Data[1],_Data[2]);
  }
  else
  {

    _Data[1] = 'A';
    //Disable NRC
    _Data[2] = 0x0F; 
    SendToBoard(_Data[1],_Data[2]);
  }

  Toggle();

  //////////Set PIRA Registers (0x00-0x03)
  _Address[1] = 'B';

  for(temp=3;temp>-1;temp--)
  {
    _q=0;
    Data1=0;
    Data2=0;

    //Address: 0-3
    _Address[2]= temp; 
    _q = _DDS67 | _Address[2];
    _Address[2] = _q;
    SendToBoard(_Address[1],_Address[2]);

    //Set Data to be Desired Frequency in Hex (T value)
    _Data[1] = 'A';
    // MSB xxxx; shift left
    Data1 = (hextodec(T_hex[2*temp+1])) << 4; 
    //xxxx LSB
    Data2 = hextodec(T_hex[2*temp]);  
    _q = Data1 | Data2;
    _Data[2] = _q;
    SendToBoard(_Data[1],_Data[2]);

    Toggle();
  }

  //////////Set AHC Register (0x0E)
  //Set register address
  _Address[1] = 'B';
  _q = _DDS67 | 0x0E;
  _Address[2] = _q; //register address
  SendToBoard(_Address[1],_Address[2]);

  //Set Data to 0x00
  _Data[1] = 'A';
  _Data[2] = 0x00; 
  SendToBoard(_Data[1],_Data[2]);

  Toggle();

  //////////Set M<0:5> bits for PLL Divide by N counter
  _Address[1] = 'B';
  _q = _DDS67 | 0x00;
  _Address[2] = _q;
  SendToBoard(_Address[1],_Address[2]);

  //Set Data to B Value
  _Data[1] = 'A';
  Data1 = (hextodec(M_hex[1])) << 4;
  Data2 = hextodec(M_hex[0]);
  _q = Data1 | Data2;
  _Data[2] = _q;
  SendToBoard(_Data[1],_Data[2]);

  //Set ADDR<5> to 0
  _Address[1] = 'B';
  _q = _DDS67-0x20;
  _Address[2] = _q;
  SendToBoard(_Address[1],_Address[2]);

  Toggle();

  //Reset all Data Address & Unselect DDS
  _Address[1] = 'B';
  _q = _DDS67 | 0x3F;
  _Address[2] = _q;
  SendToBoard(_Address[1],_Address[2]);

  _Data[1] = 'A';
  _Data[2] = 0x00;
  SendToBoard(_Data[1],_Data[2]);

  return;

}
//////////////////////////////////////////////////////////////////////////////
int
DDS::hextodec(char hex)
{
  int temp2=0;

  switch(hex)
  {
  case 'F':
    temp2=15;
    break;
  case 'E':
    temp2=14;
    break;
  case 'D':
    temp2=13;
    break;
  case 'C':
    temp2=12;
    break;
  case 'B':
    temp2=11;
    break;
  case 'A':
    temp2=10;
    break;
  case '9':
    temp2=9;
    break;
  case '8':
    temp2=8;
    break;
  case '7':
    temp2=7;
    break;
  case '6':
    temp2=6;
    break;
  case '5':
    temp2=5;
    break;
  case '4':
    temp2=4;
    break;
  case '3':
    temp2=3;
    break;
  case '2':
    temp2=2;
    break;
  case '1':
    temp2=1;
    break;
  case '0':
    temp2=0;
    break;
  }
  return temp2;
}
//////////////////////////////////////////////////////////////////////////////
std::string
DDS::dectohex(int dec)
{

  char *hexout;
  char  hex[8];
  int temp;
  int temp2;
  long double temp1=0;

  for(temp=0;temp<8;temp++)
  {
    temp1 = dec/16.0;
    dec/=16;
    temp1 = temp1-dec;
    temp1 *= 16;
    temp2 = (int)temp1;
    switch(temp2)
    {
    case 15:
      hex[7-temp]='F';
      break;
    case 14:
      hex[7-temp]='E';
      break;
    case 13:
      hex[7-temp]='D';
      break;
    case 12:
      hex[7-temp]='C';
      break;
    case 11:
      hex[7-temp]='B';
      break;
    case 10:
      hex[7-temp]='A';
      break;
    case 9:
      hex[7-temp]='9';
      break;
    case 8:
      hex[7-temp]='8';
      break;
    case 7:
      hex[7-temp]='7';
      break;
    case 6:
      hex[7-temp]='6';
      break;
    case 5:
      hex[7-temp]='5';
      break;
    case 4:
      hex[7-temp]='4';
      break;
    case 3:
      hex[7-temp]='3';
      break;
    case 2:
      hex[7-temp]='2';
      break;
    case 1:
      hex[7-temp]='1';
      break;
    case 0:
      hex[7-temp]='0';
      break;
    }
  }
  hexout = hex;
  return std::string(hexout);
}
//////////////////////////////////////////////////////////////////////////////
void
DDS:: SetDDSAddress()
{

switch(_ddsunit){
  case 0: case 1: case 2:
  {

    _Address[1]='B';
    //Address: $60 (96) - $E0 (224)
    //      Port C            Port B
    // xxxx xx(0/1)(0/1) (0/1)(0/1) 1x xxxx
    _Address[2] = 96+64*(_ddsunit); 
    SendToBoard(_Address[1], _Address[2]);

    //takes value of port B (includes addr<5>)
    _DDS67 = _Address[2];

    _Address[1] = 'C';
    _Address[2] = 0x00; 
    SendToBoard(_Address[1], _Address[2]);

    _DDS89 = _Address[2];

    //Set all data bits to 0
    _Data[1] = 'A';
    _Data[2] = 0x00;
    SendToBoard( _Data[1], _Data[2]);

    break;
  }

   case 3: case 4: case 5: case 6:
   
  {
    _Address[1] = 'B';
    _Address[2] = 32+64*(_ddsunit-3); 
    SendToBoard(_Address[1], _Address[2]);

    _DDS67 = _Address[2];

    _Address[1] = 'C';
    //xxxx xx01
    _Address[2] = 0x01; 
    SendToBoard(_Address[1], _Address[2]);
      
    _DDS89 = _Address[2];

    _Data[1] = 'A';
    _Data[2] = 0x00; 
    SendToBoard(_Data[1], _Data[2]);

    break;
  }

  case 7: case 8: case 9: case 10:
  {

    _Address[1] = 'B';
    _Address[2] = 32+64*(_ddsunit-3); 
    SendToBoard(_Address[1], _Address[2]);

    _DDS67 = _Address[2];

    _Address[1] = 'C';
    //xxxx xx10
    _Address[2] = 0x02; 
    SendToBoard(_Address[1], _Address[2]);

    _DDS89 = _Address[2];

    _Data[1] = 'A';
    _Data[2] = 0x00; 
    SendToBoard(_Data[1], _Data[2]);

    break;
  }

  case 11: case 12: case 13: case 14:
  {

    _Address[1] = 'B';
    _Address[2] = 32+64*(_ddsunit-3);
    SendToBoard(_Address[1], _Address[2]);

    _DDS67 = _Address[2];

    _Address[1] = 'C';
    //xxxx xx11
    _Address[2] = 0x03; 
    SendToBoard(_Address[1], _Address[2]);

    _DDS89 = _Address[2];

    _Data[1] = 'A';
    _Data[2] = 0x00; 
    SendToBoard(_Data[1], _Data[2]);
  }
}
}
///////////////////////////////////////////////////////////////
