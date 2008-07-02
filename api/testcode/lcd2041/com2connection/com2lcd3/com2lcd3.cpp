
//============================================================================
// Name        : com2lcd3.cpp
// Author      : Prerna Bang
// Version     : worked of com2lcd2
// Copyright   : Your copyright notice
// Description : Connect LCD2041 to the COM2 port of the TS7260.
// For the LCD2041 connections, the VCC and GND are connected to the 5V external power supply. 
// The RS-232 port on the LCD2041 is connected via serial to the 10-pin COM2 port on the TS7260.
// LCD2041 --- COM2
// Pin2(TX) --- Pin2(RXD)
// Pin3(RX) --- Pin2(TXD)
// Pin5(GND) --- Pin5(GND)
//
// This code places a "large" 5 digit number on the screen.
// It randomly generates a number between 0.00 and 359.99 and updates every 2.8 to 4.4 Hz, 
// depending on the delay chosen. 
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <termios.h>	// contains the definitions used by the terminal I/O interfaces

int calcrand();

int main(int argc, char **argv) {
	
	int fd = open("/dev/ttyAM1", O_RDWR|O_NOCTTY|O_NDELAY);	// COM 2 Port

    struct termios options;
    tcgetattr(fd, &options);	// get terminal attributes
    
    if (tcgetattr(fd, &options) < 0)
    {
      perror("Can't get terminal parameters ");
      exit(-1) ;
    }
 
    // control options: 8-N-1
    options.c_cflag &= ~PARENB; 	  // no parity
    options.c_cflag &= ~CSTOPB;		  // 1 stop bit
    options.c_cflag &= ~CSIZE;		  // mask character size bits
    options.c_cflag |= (CS8|CLOCAL|CREAD);			  // 8 data bits, local line, enable receiver
    
    options.c_cflag &= ~CRTSCTS;  	  // disable hardware flow control
    
    // input options
    options.c_iflag     = IGNPAR;	  // ignore parity errors
    
    // output options
    options.c_oflag     = 0;
    
    // line/local options
    options.c_lflag     = 0;
   
    // control characters
    options.c_cc[VTIME] = 0;		// time to wait for data
    options.c_cc[VMIN]  = 0;   		// minimum number of characters to read

    // setting baud rate
    cfsetispeed(&options, B19200);
    cfsetospeed(&options, B19200);
    
    // flushes data received but not read
    tcflush(fd, TCIFLUSH);
    
    if (tcflush(fd, TCIFLUSH) < 0)
    {
    	perror("TCflush error ");
    	exit(-1) ;
    }

    // write the settings for the serial port
    tcsetattr(fd, TCSANOW, &options);
     
    if (tcsetattr(fd,TCSANOW,&options) <0)
	{
    	perror("Can't set terminal parameters ");
    	exit(-1) ;
	}

	usleep(100000);
     
	char clrscr[] = {254, 88};
	// Clear Screen
	write(fd, clrscr, sizeof(clrscr));
	usleep(1);

	char curpos[] = {254, 71, 12, 4};
	// set cursor position for decimal point
	write(fd, curpos, sizeof(curpos));
	usleep(1);
    
	// used for decimal point
	write(fd, "o", 1);
     
	char larnum[]={254, 110};   
	// Initialize Large Numbers (3 columns wide)
	write(fd, larnum, sizeof(larnum));
	usleep(1);
     
	// Place Large Number, start off with 000.00
	char hplace[] = {254, 35, 3, 0}; // leave 2 columns at front empty
	// Hundreths Place
	write(fd, hplace, sizeof(hplace));
    
	// Tens Place
	char tplace[] = {254, 35, 6, 0}; // need to leave 3 columns between each large number
	write(fd, tplace, sizeof(tplace));
    
	// Ones place
	char oplace[] = {254, 35, 9, 0};
	write(fd, oplace, sizeof(oplace));
     
	// After decimal point, tenths place
	char dtplace[] = {254, 35, 13, 0};	// leave an extra column for decimal 
	write(fd, dtplace, sizeof(dtplace));
    
	// hundreths place
	char dhplace[] = {254, 35, 16, 0};
	write(fd, dhplace, sizeof(dhplace));
	usleep(1);

	int rnum = 0;

	//while (1)
	for (int i = 0; i<20; i++)
	{	
		rnum = calcrand();	// create random number
		printf ("%i\n", rnum);
		
		// randomly assign different values to each digit according to the random number generated
		// Only chnages
		switch(rnum)
		{
			case 0:
				dhplace[3] = 0;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 2;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 3;
				write(fd, oplace,sizeof(oplace));
				tplace[3] = 1;	
				write(fd, tplace,sizeof(tplace));
				hplace[3] = 2;
				write(fd, hplace,sizeof(hplace));
				break;
			case 1:
				dhplace[3] = 1;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 3;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 4;
				write(fd, oplace,sizeof(oplace));
				tplace[3] = 2;
				write(fd, tplace,sizeof(tplace));
				hplace[3] = 3;
				write(fd, hplace,sizeof(hplace));
				break;
			case 2:
				dhplace[3] = 2;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 4;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 5;
				write(fd, oplace,sizeof(oplace));
				tplace[3] = 3;
				write(fd, tplace,sizeof(tplace));
				hplace[3] = 1;
				write(fd, hplace,sizeof(hplace));
				break;
			case 3:
				dhplace[3] = 3;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 5;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 6;
				write(fd, oplace,sizeof(oplace));
				tplace[3] = 4;
				write(fd, tplace,sizeof(tplace));
				break;
			case 4:
				dhplace[3] = 4;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 6;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 7;
				write(fd, oplace,sizeof(oplace));
				tplace[3] = 5;
				write(fd, tplace,sizeof(tplace));
				break;
			case 5:
				dhplace[3] = 5;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 7;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 8;
				write(fd, oplace,sizeof(oplace));
				break;
			case 6:
				dhplace[3] = 6;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 8;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 9;
				write(fd, oplace,sizeof(oplace));
				break;
			case 7:
				dhplace[3] = 7;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 9;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 0;
				write(fd, oplace,sizeof(oplace));
				break;
			case 8:
				dhplace[3] = 8;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 0;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 1;
				write(fd, oplace,sizeof(oplace));
				break;
			case 9:
				dhplace[3] = 9;
				write(fd, dhplace,sizeof(dhplace));
				dtplace[3] = 1;
				write(fd, dtplace,sizeof(dtplace));
				oplace[3] = 2;
				write(fd, oplace,sizeof(oplace));
				break;
		}
		usleep(225000);	// Relates to 4.44 Hz
		// Can range this delay from 2.8 Hz to 5 Hz.
		// Values are:
		// 200,000 -> 5Hz
		// 225,000 -> 4.44Hz
		// 250,000 -> 4Hz
		// 275,000 -> 3.63Hz
		// 300,000 -> 3.33Hz
		// 330,000 -> 3.03Hz	(Optimal)
		// 350,000 -> 2.8Hz
		
		
		if (char(hplace[3]) == 3 && char(tplace[3]) == 5 && char(oplace[3]) == 9 && char(dtplace[3]) == 9 && char(dhplace[3]) == 9)
		{
			hplace[3]= 0;
			write(fd, hplace,sizeof(hplace));
    					 
			tplace[3]= 0;
			write(fd, tplace,sizeof(tplace));
    					 
			oplace[3]= 0;
			write(fd, oplace,sizeof(oplace));
    					 
			dtplace[3]= 0;
			write(fd, dtplace,sizeof(dtplace));
    					 
			dhplace[3]= 0;
			write(fd, dhplace,sizeof(dhplace));
                		 
			usleep(225000);
		}
	}
    
	sleep(2);
    //Clear Screen
    write(fd, clrscr, sizeof(clrscr));
    
    printf("Done\n");
    close(fd);
    return 0;
}

int calcrand()
{
	return (0 + (rand()%(1+(9-0))));
}


