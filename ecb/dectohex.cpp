//dec to hex program
//June 26, 2007

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

char *dectohex(int dec)
	{
	
	char *hexout, hex[8];
	int temp, temp2;
	long double temp1=0;
	
	for(temp=0;temp<8;temp++){
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
		return hexout;
	}


