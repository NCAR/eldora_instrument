//Drew Leung
//rfputval
//June 26/27/28/29, 2007

#include <math.h>
#include "etherio24.h"
#include "dds.h"

void rfputval(EtherIO24 ether, DDS dds, int uin, double freqin, int nrin)
	{
	
	int B=0, T=0, N=0, temp;
	unsigned char Address[5],Select[5],Data[5];
	unsigned char DDS67,DDS89,q,Data1,Data2;
	char T_hex[8],M_hex[2];
	char *hold;
	//char hold;

	void Output(unsigned char cmd[]);
	int hextodec(char hex);
	char *dectohex(int dec);
	
	//initialize variables
	DDS67 = 0;
	DDS89=0;
	q=0;
	Data1=0;
	Data2=0;
	
	//Calculate B N T
	B = 361 - (int)(freqin/30e6);
	N = B + 1;
	T = (int)(pow(2,24)/(B+1.0)*(256.0*181/3 - freqin/703125.0) + 0.5);

	//Convert T and B into hex
	hold = dectohex(T);
	T_hex[7] = *(hold);
	T_hex[6] = *(hold + 1);
	T_hex[5] = *(hold + 2);
	T_hex[4] = *(hold + 3);
	T_hex[3] = *(hold + 4);
	T_hex[2] = *(hold + 5);
	T_hex[1] = *(hold + 6);
	T_hex[0] = *(hold + 7);
	
	hold =dectohex(B);
	M_hex[1] =*(hold + 6);
	M_hex[0] =*(hold + 7);
	
		//Sets Upper Four Bits of Address addr<6:9>
		if(uin>=1 && uin<4)
		{
		//choose DDS address
		Address[0] = '!';
		Address[1] = 'B';
		Address[2] = 0x00; 
		Address[3] = 'B'; 
		Address[4] = 96+64*(uin-1); //$60 (96) - $E0 (224)
		printf("1-3 DDS Address = %x\n", Address[4]);
		ether.Output(Address, 5);
		
		DDS67 = Address[4]; //takes value of port B (includes addr<5>)
	
		Address[0] = '!';
		Address[1] = 'C';
		Address[2] = 0x00;
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
		Address[2] = 0x00; 
		Address[3] = 'B'; 
		Address[4] = 32+64*(uin-4); //$20 (32) - $E0 (224)
		printf("4+ DDS Address = %x\n", Address[4]);
		ether.Output(Address, 5);
	
		DDS67 = Address[4];
		
		Address[0] = '!';
		Address[1] = 'C';
		Address[2] = 0x00;
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
	    
		
	//Set AMC Register (0x0A) to either 0x0C or 0x0F
	//Set DS=1, W/R=0 before setting AMC register
	Select[0]='!';
	Select[1]='C';
	Select[2]= 0x00; // only DS and W/R
	Select[3]='C';
	q = DDS89 | 0x08;
	Select[4]= q;
	ether.Output(Select, 5);

	
	//set register address
	Address[0] = '!';
	Address[1] = 'B';
	Address[2] = 0x00; 
	Address[3] = 'B'; 
	q = DDS67 | 0x0A;
	Address[4]= q; ;
	ether.Output(Address, 5);

	if(nrin == 1)
		{
		Data[0] = '!';
		Data[1] = 'A';
		Data[2] = 0x00; 
		Data[3] = 'A'; 
		Data[4] = 0x0C; //Enable NRC
		ether.Output(Data, 5);
		}
	else
		{
		Data[0] = '!';
		Data[1] = 'A';
		Data[2] = 0x00; 
		Data[3] = 'A'; 
		Data[4] = 0x0F; //Disable NRC
		ether.Output(Data, 5);
		}

	dds.Toggle();
	
	//Set PIRA Registers (0x00-0x03)
	Address[0] = '!';
	Address[1] = 'B';
	Address[2] = 0x00; 
	Address[3] = 'B'; 

	for(temp=3;temp>-1;temp--)
		{
		q=0;
		Data1=0;
		Data2=0;
		Address[4]= temp; //just register address
		q = DDS67 | Address[4];
		Address[4] = q;
		ether.Output(Address, 5);
		
		Data[0] = '!';
		Data[1] = 'A';
		Data[2] = 0x00;
		Data[3] = 'A'; 
		Data1 = (hextodec(T_hex[2*temp+1])) << 4; // MSB xxxx; shift left
		Data2 = hextodec(T_hex[2*temp]);  //xxxx LSB
		q = Data1 | Data2;
		Data[4] = q;
		ether.Output(Data, 5);
		
		dds.Toggle();
		}
	
	//Set AHC Register (0x0E) 
	//set register address
	Address[0] = '!';
	Address[1] = 'B';
	Address[2] = 0x00;
	Address[3] = 'B'; 
	q = DDS67 | 0x0E;
	Address[4] = q; //register address
	ether.Output(Address, 5);

    
	Data[0] = '!';
	Data[1] = 'A';
	Data[2] = 0x00; 
	Data[3] = 'A'; 
	Data[4] = 0x00; //for data<0:7>;
	ether.Output(Data, 5);
		
	dds.Toggle();
	
		
	//Set M<0:5> bits for PLL Divide by N counter
	Address[0] = '!';
	Address[1] = 'B';
	Address[2] = 0x00; 
	Address[3] = 'B';
	q = DDS67 | 0x00; 
	Address[4] = q; 
	ether.Output(Address, 5);
	
		
	Data[0] = '!';
	Data[1] = 'A';
	Data[2] = 0x00; 
	Data[3] = 'A';
	Data1 = (hextodec(M_hex[1])) << 4;
	Data2 = hextodec(M_hex[0]);
	q = Data1 | Data2;
	Data[4] = q;
	ether.Output(Data, 5); 
	
	//Set addr<5> to 0
	Address[0] = '!';
	Address[1] = 'B';
	Address[2] = 0x00; 
	Address[3] = 'B'; 
	q = DDS67-0x20;
	Address[4] = q;
	ether.Output(Address, 5);
	
	dds.Toggle();
	
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

	/*
	Select[0]='!';
	Select[1]='C';
	Select[2]=0x00;
	Select[3]='C';
	Select[4]=0x00;
	Output(Select);
	*/
	
	return;
	}
	



