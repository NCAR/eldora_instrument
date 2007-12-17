//rfinit.c
//Drew Leung; modification of Tom's code
//June 25/26/27/28, 2007

#include "etherio24.h"
#include "dds.h"

//---------------------
//Initialize DDS Modules
//---------------------
void rfinit(EtherIO24 ether,DDS dds, int unit)
{


  unsigned char Address[5];
  unsigned char Select[5];
  unsigned char Data[5];
  int i;

  void Output(unsigned char cmd[]);
  void Toggle();
  unsigned char DDS67;
  unsigned char DDS89;
  unsigned char q;

  DDS67=0;
  DDS89=0;
  q=0;


  //Set Upper Four bits
  if(unit>=1 && unit<4)
  {
    //choose DDS address
    Address[0] = '!';
    Address[1] = 'B';
    Address[2] = 0x00;
    Address[3] = 'B';
    Address[4] = 96+64*(unit-1); //$60 (96) - $E0 (224)
    ether.Output(Address, 5);

    //takes value of port B (includes addr<5>)
    DDS67 = Address[4];

    Address[0] = '!';
    Address[1] = 'C';
    Address[2] = 0xf0;
    Address[3] = 'C';
    Address[4] = 0x00; // 0000 0000
    ether.Output(Address, 5);

    DDS89 = Address[4];

    //make sure all data bits are 0
    Data[0] = '!';
    Data[1] = 'A';
    Data[2] = 0x00;
    Data[3] = 'A';
    Data[4] = 0x00;
    ether.Output(Data, 5);

  }

  else
  {
    Address[0] = '!';
    Address[1] = 'B';
    Address[2] = 0x00; //only setting addr<5:7>
    Address[3] = 'B';
    Address[4] = 32+64*(unit-4); //$20 (32) - $E0 (224)
    ether.Output(Address, 5);

    DDS67 = Address[4];

    Address[0] = '!';
    Address[1] = 'C';
    Address[2] = 0xf0; // 1111 1100
    Address[3] = 'C';
    Address[4] = 0x01; //xxxx xx01
    ether.Output(Address, 5);


    DDS89 = Address[4];

    Data[0] = '!';
    Data[1] = 'A';
    Data[2] = 0x00;
    Data[3] = 'A';
    Data[4] = 0x00; //make sure all data bits are 0
    ether.Output(Data, 5);

  }


  //Set SMC  Register (0x08) to 0x00
  //Set DS=1, W/R=0 before setting SMC register
  Select[0]='!';
  Select[1]='C';
  Select[2]= 0xf0; // only DS and W/R
  Select[3]='C';
  q = DDS89 | 0x08;//combine addr<8:9> with DS=1 and W/R=0; q= 0000 100(0/1)
  Select[4]=q;
  ether.Output(Select, 5);

  //set register address
  Address[0] = '!';
  Address[1] = 'B';
  Address[2] = 0x00;
  Address[3] = 'B';
  q = DDS67 | 0x08; //combine addr<5:7> with register address 0x08
  Address[4]=q;
  ether.Output(Address, 5);


  Data[0] = '!';//port A direction register
  Data[1] = 'A';//port A direction register
  Data[2] = 0x00; //set I/O lines as output
  Data[3] = 'A'; //port A port register
  Data[4] = 0x00; //for data<0:7>;
  ether.Output(Data, 5);

  dds.Toggle(ether);

  //Set PIRB Registers (0x07-0x04) to 0x00
  Address[0] = '!';
  Address[1] = 'B';
  Address[2] = 0x00;
  Address[3] = 'B';

  for(i=0;i<=3;i++)
  {
    q=0;

    Address[4]= 7-i;//set only register address
    q = DDS67 | Address[4];
    Address[4] = q;
    ether.Output(Address, 5);

    //Set to 0x00
    Data[0] = '!';
    Data[1] = 'A';
    Data[2] = 0x00;
    Data[3] = 'A';
    Data[4] = 0x00; //for data<0:7>;
    ether.Output(Data, 5);

    dds.Toggle(ether);
  }


  //Set APR Register (0x0C) to 0x00
  //set register address
  Address[0] = '!';
  Address[1] = 'B';
  Address[2] = 0x00;
  Address[3] = 'B';
  q = DDS67 | 0x0C; //combine addr<5:7> with register address 0x08
  Address[4]=q;
  ether.Output(Address, 5);


  Data[0] = '!';
  Data[1] = 'A';
  Data[2] = 0x00;
  Data[3] = 'A';
  Data[4] = 0x00; //for data<0:7>;
  ether.Output(Data, 5);

  dds.Toggle(ether);

  //Set DDS2 Registers to 0x00
  Address[0] = '!';
  Address[1] = 'B';
  Address[2] = 0x00;
  Address[3] = 'B';

  for(i=0;i<15;i++)
  {
    q=0;
    Address[4]= 16+i; //16($10)-31($1F) only setting reg address
    q = DDS67 | Address[4];
    Address[4] = q;
    ether.Output(Address, 5);


    //Set Data
    Data[0] = '!';
    Data[1] = 'A';
    Data[2] = 0x00;
    Data[3] = 'A';
    Data[4] = 0x00;
    ether.Output(Data, 5);

    dds.Toggle(ether);
  }

  //Reset all Data Address & Unselect DDS
  Address[0] = '!';
  Address[1] = 'B';
  Address[2] = 0x00;
  Address[3] = 'B';
  q = DDS67 | 0x3F;
  Address[4] = q;
  ether.Output(Address, 5);



  Data[0] = '!';
  Data[1] = 'A';
  Data[2] = 0x00;
  Data[3] = 'A';
  Data[4] = 0x00; //for data<0:7>;
  ether.Output(Data, 5);


  return;
}



