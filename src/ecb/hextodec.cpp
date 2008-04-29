//Drew Leung
//hextodec.c
//June 26, 2007


//---------------
//hex to dec
//---------------
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


int hextodec(char hex)
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


