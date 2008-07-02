
//============================================================================
// Name        : com2lcd.cpp
// Author      : Prerna Bang
// Version     : 
// Copyright   : Your copyright notice
// Description : Connect LCD2041 to the COM2 port of the TS7260.
// For the LCD2041 connections, the VCC and GND are connected to the 5V external power supply. 
// The RS-232 port on the LCD2041 is connected via serial to the 10-pin COM2 port on the TS7260.
// LCD2041 --- COM2
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
	
	int fd = open("/dev/ttyAM1", O_RDWR|O_NOCTTY|O_NDELAY);		// COM2 Port
/*
 * The O_NOCTTY flag tells UNIX that this program doesn't want to be the "controlling terminal" 
 * for that port. If you don't specify this then any input (such as keyboard abort signals and 
 * so forth) will affect your process. 
 * The O_NDELAY flag tells UNIX that this program doesn't care what state the DCD signal 
 * line is in - whether the other end of the port is up and running. If you do not specify this 
 * flag, your process will be put to sleep until the DCD signal line is the space voltage.
*/
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
    // options.c_cflag &= ~(CSIZE|PARENB|CRTSCTS);
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

     // Writing text on the LCD screen
     
     //char Buff[]="Hello Mum and Dad";
     //write(fd, Buff, sizeof(Buff));
     //sleep(1);
     
     //char Buff2[]="Happy";
     //write(fd, Buff2, sizeof(Buff2));
     //sleep(1);
     
     //char Buffb[]="Happy";
     //write(fd, Buffb, sizeof(Buffb));
     //sleep(1);
     
     //write(fd,"Hola", sizeof("Hola"));
     //sleep(1);
    
     char buffa[2];
     buffa[0] = 254; //buffa[0] = 0xFE;
     buffa[1] = 88; //buffa[1] = 0x58;
     // Clear Screen
     write(fd, buffa, 2);
     sleep(1);
     
     //char buffb[2];
     //buffb[0] = 254; //buffb[0] = 0xFE;
     //buffb[1] = 74; //buffb[1] = 0x4A;
     // Underline Cursor On
     //write(fd, buffb, 2);
     //sleep(1);
     
     //char buffc[2];
     //buffc[0] = 254; //buffc[0] = 0xFE;
     //buffc[1] = 75; //buffc[1] = 0x4B;
     // Underline Cursor Off
     //write(fd, buffc, 2);
     //sleep(1);
     
     //char buffd[2];
     //buffd[0] = 254; //buffd[0] = 0xFE;
     //buffd[1] = 83; //buffd[1] = 0x53;
     // Block Cursor On
     //write(fd, buffd, 2);
     //sleep(1);
     
     //char buffe[2];
     //buffe[0] = 254; //buffe[0] = 0xFE;
     //buffe[1] = 84; //buffe[1] = 0x54;
     // Block Cursor Off
     //write(fd, buffe, 2);
     //sleep(1);
     
     char buffint[2];
     buffint[0] = 254; //buffint[0] = 0xFE;
     buffint[1] = 110; //buffint[1] = 0x6E;
     // Initialize Large Numbers
     write(fd, buffint, 2);
     sleep(1);
    
     char buffnum[] = {254, 35, 1, 9};	
     // buffnum[0] = 254; //buffnum[0] = 0xFE;
     // buffnum[1] = 35; //buffnum[1] = 0x23;
     // buffnum[2] = 1; //buffnum[2] = 1;	// Column 
     // buffnum[3] = 5; //buffnum[3] = 5;	// Number
     // Place Large Number
     write(fd, buffnum,4);
     sleep(1);

/*
 * write(int fildes, const void *buf, size_t nbyte)
 * fildes - file descripter
 * The write function returns the number of bytes sent or -1 if an error occurred. 
 * Usually the only error you'll run into is EIO when a MODEM or data link drops the Data 
 * Carrier Detect (DCD) line. This condition will persist until you close the port.
 */
     
     printf("Done\n");
     close(fd);
     return 0;
}


