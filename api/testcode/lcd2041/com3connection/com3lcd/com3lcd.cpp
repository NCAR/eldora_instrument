
//============================================================================
// Name        : com3lcd.cpp
// Author      : Prerna Bang
// Version     : worked of com2lcd
// Copyright   : Your copyright notice
// Description : Connect LCD2041 to the COM3 port of the TS7260.
// For the LCD2041 connections, the VCC and GND are connected to the 5V external power supply. 
// The RS-232 port on the LCD2041 is connected via serial to the 10-pin COM3 port on the TS7260.
// LCD2041 --- COM3
// Pin2(TX) --- Pin2(RXD)
// Pin3(RX) --- Pin2(TXD)
// Pin5(GND) --- Pin5(GND)
//
// This code clears the screen, initializes "large numbers" and places a large number of the screen.
// It also includes in other comments, other commands, ways to send text to the screen, and how to
// write the command code.
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <termios.h>	// contains the definitions used by the terminal I/O interfaces

int main(int argc, char **argv) {
	
	int fd = open("/dev/ttyTS0", O_RDWR|O_NOCTTY|O_NDELAY);	// COM3

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

	for (int i=0; i<20; i++)
	//while (1)
	{
		if (char(dhplace[3]) == 9) 
		{	
			if (char(dtplace[3]) == 9) 
			{
				if (char(oplace[3]) == 9) 
    			{
					if (char(hplace[3]) == 3 && char(tplace[3]) == 5)
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
                	}
     				else if (char(tplace[3]) == 9)
    				{
     					hplace[3]= hplace[3]+1;
                		write(fd, hplace,sizeof(hplace));
    					 
            			tplace[3]= 0;
                		write(fd, tplace,sizeof(tplace));
    					 
            			oplace[3]= 0;
                		write(fd, oplace,sizeof(oplace));
    					 
            			dtplace[3]= 0;
                		write(fd, dtplace,sizeof(dtplace));

            			dhplace[3]= 0;
                		write(fd, dhplace,sizeof(dhplace));
    				}
    				else
    				{
    					tplace[3]= tplace[3]+1;
                		write(fd, tplace,sizeof(tplace));
    					 
            			oplace[3]= 0;
                		write(fd, oplace,sizeof(oplace));
    					 
            			dtplace[3]= 0;
                		write(fd, dtplace,sizeof(dtplace));
    					 
            			dhplace[3]= 0;
                		write(fd, dhplace,sizeof(dhplace));
    				}
    			}
    			else
    			{	
    				oplace[3]= oplace[3]+1;
            		write(fd, oplace,sizeof(oplace));
    				 
        			dtplace[3]= 0;
            		write(fd, dtplace,sizeof(dtplace));
    				 
        			dhplace[3]= 0;
            		write(fd, dhplace,sizeof(dhplace));
    			}
			}
    		else
    		{	
    			dtplace[3]= dtplace[3]+1;
        		write(fd, dtplace,sizeof(dtplace));
        		
    			dhplace[3]= 0;
        		write(fd, dhplace,sizeof(dhplace));      
    		}
		}
    	else
    	{	
    		dhplace[3]= dhplace[3]+1;
    		write(fd, dhplace,sizeof(dhplace));
    	}
		usleep(200000);
  
	}
    
	sleep(2);
    //Clear Screen
    write(fd, clrscr, sizeof(clrscr));
    
    printf("Done\n");
    close(fd);
    return 0;
}


