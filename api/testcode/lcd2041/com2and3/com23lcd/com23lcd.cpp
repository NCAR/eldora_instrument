
//============================================================================
// Name        : com23lcd.cpp
// Author      : Prerna Bang
// Version     : worked of com2lcd3 and com3lcd
// Copyright   : Your copyright notice
// Description : Connect one LCD2041 to the COM2 port of the TS7260 and 
// another LCD2041 to the COM3 port.
// COM2 => For Azimuth Position Angle
// COM3 => For Elevation Position Angle
// For the LCD2041 connections, the VCC and GND are connected to the 5V external power supply. 
// The RS-232 port on the LCD2041 is connected via serial to the 10-pin COM2 port on the TS7260.
// LCD2041 --- COM2 and Com3
// Pin2(TX) --- Pin2(RXD)
// Pin3(RX) --- Pin2(TXD)
// Pin5(GND) --- Pin5(GND)
//
// This program randomly calculates every unit place for azimuth synchro and offset 
// as well as elevation synchro and offset every 3 Hz.
// It adds the synchro and offset values together and checks to make sure it is within the 
// range from 0.00 to 359.99, otherwise it loops it around.
// It then displays the azimuth position and elevation position on the LCD screens as large numbers
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <termios.h>	// contains the definitions used by the terminal I/O interfaces

void switchthing(int, char *, int);

int main(int argc, char **argv) {
	
	// COM 2
	int fd = open("/dev/ttyAM1", O_RDWR|O_NOCTTY|O_NDELAY);

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

    
	// COM 3
	int fd3 = open("/dev/ttyTS0", O_RDWR|O_NOCTTY|O_NDELAY);

    struct termios options3;
    tcgetattr(fd3, &options3);	// get terminal attributes
    
    if (tcgetattr(fd3, &options3) < 0)
    {
    	perror("Can't get terminal parameters ");
    	exit(-1) ;
    }
 
    // control options: 8-N-1
    options3.c_cflag &= ~PARENB; 	  // no parity
    options3.c_cflag &= ~CSTOPB;		  // 1 stop bit
    options3.c_cflag &= ~CSIZE;		  // mask character size bits
    options3.c_cflag |= (CS8|CLOCAL|CREAD);			  // 8 data bits, local line, enable receiver
    
    options3.c_cflag &= ~CRTSCTS;  	  // disable hardware flow control
    
    // input options
    options3.c_iflag     = IGNPAR;	  // ignore parity errors
    
    // output options
    options3.c_oflag     = 0;
    
    // line/local options
    options3.c_lflag     = 0;
   
    // control characters
    options3.c_cc[VTIME] = 0;		// time to wait for data
    options3.c_cc[VMIN]  = 0;   		// minimum number of characters to read

    // setting baud rate
    cfsetispeed(&options3, B19200);
    cfsetospeed(&options3, B19200);
    
    // flushes data received but not read
    tcflush(fd3, TCIFLUSH);
    
    if (tcflush(fd3, TCIFLUSH) < 0)
    {
    	perror("TCflush error ");
    	exit(-1) ;
    }

     // write the settings for the serial port
    tcsetattr(fd3, TCSANOW, &options3);
     
    if (tcsetattr(fd3,TCSANOW,&options3) <0)
	{
    	perror("Can't set terminal parameters ");
    	exit(-1) ;
	}

	usleep(100000);
	
	// Initialization
	char clrscr[] = {254, 88};
	// Clear Screen
	write(fd, clrscr, sizeof(clrscr));
	write(fd3, clrscr, sizeof(clrscr));
	usleep(1);

	char curpos[] = {254, 71, 12, 4};
	// set cursor position for decimal point
	write(fd, curpos, sizeof(curpos));
	write(fd3, curpos, sizeof(curpos));
	usleep(1);
    
	// used for decimal point
	write(fd, "o", 1);
	write(fd3, "o", 1);
     
	char larnum[]={254, 110};   
	// Initialize Large Numbers (3 columns wide)
	write(fd, larnum, sizeof(larnum));
	write(fd3, larnum, sizeof(larnum));
	usleep(1);
     
	// Place Large Number, start off with 000.00
	char hplace[] = {254, 35, 3, 0}; // leave 2 columns at front empty
	// Hundreths Place
	write(fd, hplace, sizeof(hplace));
	write(fd3, hplace, sizeof(hplace));
    
	// Tens Place
	char tplace[] = {254, 35, 6, 0}; // need to leave 3 columns between each large number
	write(fd, tplace, sizeof(tplace));
	write(fd3, tplace, sizeof(tplace));
    
	// Ones place
	char oplace[] = {254, 35, 9, 0};
	write(fd, oplace, sizeof(oplace));
	write(fd3, oplace, sizeof(oplace));

	// After decimal point, tenths place
	char dtplace[] = {254, 35, 13, 0};	// leave an extra column for decimal 
	write(fd, dtplace, sizeof(dtplace));
	write(fd3, dtplace, sizeof(dtplace));

	// hundreths place
	char dhplace[] = {254, 35, 16, 0};
	write(fd, dhplace, sizeof(dhplace));
	write(fd3, dhplace, sizeof(dhplace));

	sleep(1);
	
	double dhnum, dtnum, onum, tnum, hnum;
	double as, ao, es, eo;
	double ap, ep;
	int aph, apt, apo, apdt, apdh;
	int eph, ept, epo, epdt, epdh;

	//int i = 0;
	//while (1)
	for (int i = 0; i<10; i++)
	{	
		srand(i);
		
		// AZIMUTH on COM2
		hnum = ((double)(0 + (rand()%(1+(300-0)))));
		tnum = ((double)(0 + (rand()%(1+(50-0)))));
		onum = ((double)(0 + (rand()%(1+(9-0)))));
		dtnum = ((double)(0 + (rand()%(1+(9-0)))))*.1;
		dhnum = ((double)(0 + (rand()%(1+(9-0)))))*.01;
		// azimuth synchro
		as = hnum + tnum + onum + dtnum + dhnum;
		//printf("%lf\n", as);
		
		hnum = ((double)(0 + (rand()%(1+(300-0)))));
		tnum = ((double)(0 + (rand()%(1+(50-0)))));
		onum = ((double)(0 + (rand()%(1+(9-0)))));
		dtnum = ((double)(0 + (rand()%(1+(9-0)))))*.1;
		dhnum = ((double)(0 + (rand()%(1+(9-0)))))*.01;
		// azimuth offset
		ao = hnum + tnum + onum + dtnum + dhnum;
		//printf("%lf\n", ao);

		// azimuth position
		ap = as + ao;
		if (ap > 359.99)	// if greater than 359.99 loop back around
			ap = ap - 359.99;
		printf("azimuth position: %lf\n", ap);
		
		// figuring out each unit value
		aph = (int)(ap/100);
		apt = (int)((ap-(aph*100))/10);
		apo = (int)(ap-(aph*100)-(apt*10));
		apdt = (int)((ap-(aph*100)-(apt*10)-apo)*10);
		apdh = (int)((ap-(aph*100)-(apt*10)-apo-(apdt*.1))*100);
		printf("each unit place: %i, %i, %i, %i, %i\n", aph, apt, apo, apdt, apdh);
		
		// updating each unit value
		switchthing(fd, hplace, aph);
		switchthing(fd, tplace, apt);
		switchthing(fd, oplace, apo);
		switchthing(fd, dtplace, apdt);
		switchthing(fd, dhplace, apdh);
		
		// ELEVATION on COM3
		hnum = ((double)(0 + (rand()%(1+(300-0)))));
		tnum = ((double)(0 + (rand()%(1+(50-0)))));
		onum = ((double)(0 + (rand()%(1+(9-0)))));
		dtnum = ((double)(0 + (rand()%(1+(9-0)))))*.1;
		dhnum = ((double)(0 + (rand()%(1+(9-0)))))*.01;
		// elevation synchro
		es = hnum + tnum + onum + dtnum + dhnum;
		//printf("%lf\n", es);
				
		hnum = ((double)(0 + (rand()%(1+(300-0)))));
		tnum = ((double)(0 + (rand()%(1+(50-0)))));
		onum = ((double)(0 + (rand()%(1+(9-0)))));
		dtnum = ((double)(0 + (rand()%(1+(9-0)))))*.1;
		dhnum = ((double)(0 + (rand()%(1+(9-0)))))*.01;
		// elevation offset
		eo = hnum + tnum + onum + dtnum + dhnum;
		//printf("%lf\n", eo);

		// elevation position
		ep = es + eo;
		if (ep > 359.99)
			ep = ep - 359.99;
		printf("elevation position: %lf\n", ep);

		// figuring out each unit value
		eph = (int)(ep/100);
		ept = (int)((ep-(eph*100))/10);
		epo = (int)(ep-(eph*100)-(ept*10));
		epdt = (int)((ep-(eph*100)-(ept*10)-epo)*10);
		epdh = (int)((ep-(eph*100)-(ept*10)-epo-(epdt*.1))*100);
		printf("each unit place: %i, %i, %i, %i, %i\n", eph, ept, epo, epdt, epdh);
		
		// updating each unit value
		switchthing(fd3, hplace, eph);
		switchthing(fd3, tplace, ept);
		switchthing(fd3, oplace, epo);
		switchthing(fd3, dtplace, epdt);
		switchthing(fd3, dhplace, epdh);
		
		usleep(330000);		// this is around 3 Hz, quite optimal refresh rate for LCD
		
		// i++;		//change for i for srand
	}
    
	sleep(2);
    //Clear Screen
    write(fd, clrscr, sizeof(clrscr));
    write(fd3, clrscr, sizeof(clrscr));
    
    printf("Done\n");
    close(fd);
    close(fd3);
    return 0;
}

void switchthing(int ff, char *place, int num) {
	switch(num)
	{
		case 0:
			place[3] = 0;		
			break;
		case 1:
			place[3] = 1;
			break;
		case 2:
			place[3] = 2;
			break;
		case 3:
			place[3] = 3;
			break;
		case 4:
			place[3] = 4;
			break;
		case 5:
			place[3] = 5;
			break;
		case 6:
			place[3] = 6;
			break;
		case 7:
			place[3] = 7;
			break;
		case 8:
			place[3] = 8;
			break;
		case 9:
			place[3] = 9;
			break;
	}
	// writing each unit value to the corresponding LCD
	write(ff, place, sizeof(place));
}


