//============================================================================
// Name        : com23lcd2.cpp
// Author      : Prerna Bang
// Version     : worked of com23lcd
// Copyright   : Your copyright notice
// Description : Connect one LCD2041 to the COM2 port of the TS7260 and 
// another LCD2041 to the COM3 port.
// The TS-DIO24 is connected to the PS-104 connection on the TS7260. The DIO24 is connected to
// hello board device.
//
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
// as well as elevation synchro and offset constantly.
// It adds the synchro and offset values together and checks to make sure it is within the 
// range from 0.00 to 359.99, otherwise it loops it around.
// It then turns on and off some LEDs on the HelloDevice to show that the positions are 
// being calculated.
// Every 3 Hz (a timer is set for 330000 microseconds), the positions the azimuth position and 
// elevation position are displayed on the LCD screens as large numbers
// The loop is continous but can be aborted by the Ctrl+C or Ctrl+\ command
//============================================================================

#include <stdio.h>		// perror, printf functions
#include <unistd.h>		// ualarm, sleep, usleep, write, getpasgesize functions
#include <stdlib.h>		// srand, rand, exit functions
#include <fcntl.h>		// flags for open function
#include <sys/mman.h>	// flags for mmap function
#include <termios.h>	// contains the definitions used by the terminal I/O interfaces
#include <signal.h>		// signal function

// TS-DI0 24 => 8 bit
// Start for PC/104 8 bit read/write to I/O
static volatile unsigned char *pc104_start;

static volatile unsigned char *dio24_r0;
static volatile unsigned char *dio24_r1;
static volatile unsigned char *dio24_r2;
static volatile unsigned char *dio24_r3;
static volatile unsigned char *dio24_r4;
static volatile unsigned char *dio24_r5;
static volatile unsigned char *dio24_r6;
static volatile unsigned char *dio24_r7;

// commands
char clrscr[2];		// for clearing screen
char curpos[4];		// setting cursor position
char larnum[2];		// initialize large numbers

// unit places for LCD screen
char hplace[4];		// hundreds place
char tplace[4];		// tens place
char oplace[4]; 	// ones place
char dtplace[4]; 	//after decimal, tenths place
char dhplace[4]; 	// hundredths place

double ap;		// azimuth position
double ep;		// elevation position

int fdpc;		// file descriptor for PC-104 connection
int fd2;		// file descriptor for COM2 port
int fd3; 		// file descriptor for COM3 port

#define PC104	0x11E00000		// 8 bit I/O Cycle Starting Address

// jumper settings for base address selection for expansion board
// JP1 and JP2 are off
#define DIO24_BASE (0x0100)

void pc104init(void);	// initializing pc104 expansion board
void com2init(void);	// initializing COM2 port
void com3init(void);	// initializing COM3 port
void lcdsetup(void);	// setting up both LCD screens
void cleanup(int);   	// executed upon an abort
void wakeup(int);    	// executed when alarm (a particular time gap completed) received
double calcrand(int);	// calculate random value given limits and convert to double
void switchthing(int, char *, int);		// updating value of a unit place for LCD

int main(int argc, char **argv) {
	
	//PC-104
	pc104init();
	usleep(1000);
	
	// COM 2
	com2init();
	usleep(1000);
	
	// COM 3
	com3init();
	usleep(1000);
	
	// Setup both LCD screens
	lcdsetup();
	sleep(2);
	
	// to save every unit value
	double hnum, tnum, onum, dtnum, dhnum;
	double as, ao;		// azimuth synchro and azimuth offset
	double es, eo;		// elevation synchro and elevation offset
	
	int i = 0;		// counter for srand and loop

	// abort signals
	signal(SIGQUIT,cleanup);     // Normally Ctrl+C or Ctrl+'\'
	
	wakeup(0);		// to wait till every 3 Hz, which is ~ 330000 microseconds

	while(1)
	//while (i!=10000)		// for shorter loop
	{	
		srand(i);	// constantly changes due to i for different random numbers
		
		// azimuth synchro
		hnum = calcrand(300);
		tnum = calcrand(50);
		onum = calcrand(9);
		dtnum = calcrand(9)*.1;
		dhnum = calcrand(9)*.01;
		as = hnum + tnum + onum + dtnum + dhnum;
		//printf("azimuth synchro: %lf\n", as);
		
		// azimuth offset
		hnum = calcrand(300);
		tnum = calcrand(50);
		onum = calcrand(9);
		dtnum = calcrand(9)*.1;
		dhnum = calcrand(9)*.01;
		ao = hnum + tnum + onum + dtnum + dhnum;
		//printf("azimuth offset: %lf\n", ao);

		// azimuth position
		ap = as + ao;
		if (ap > 359.99)
			ap = ap - 359.99;
		printf("azimuth position: %lf\n", ap);
		// to show that it is calculating position values constantly
		*dio24_r6 = 0x00;		//Port B all off
		if ((i%2)==0)
			*dio24_r6 = 0x80;	//LED D1 on
		else if ((i%3)==0)
			*dio24_r6 = 0x40;	//LED D3 on
		else
			*dio24_r6 = 0x30;	//LED D5 on
		//usleep(1);
		
		// elevation synchro
		hnum = calcrand(300);
		tnum = calcrand(50);
		onum = calcrand(9);
		dtnum = calcrand(9)*.1;
		dhnum = calcrand(9)*.01;
		es = hnum + tnum + onum + dtnum + dhnum;
		//printf("elevation synchro: %lf\n", es);
				
		// elevation offset
		hnum = calcrand(300);
		tnum = calcrand(50);
		onum = calcrand(9);
		dtnum = calcrand(9)*.1;
		dhnum = calcrand(9)*.01;
		eo = hnum + tnum + onum + dtnum + dhnum;
		//printf("elevation offset: %lf\n", eo);

		//elevation position
		ep = es + eo;
		if (ep > 359.99)
			ep = ep - 359.99;
		
		// to show that it is calculating position values constantly
		printf("elevation position: %lf\n", ep);
		*dio24_r6 = 0x00;		// Port B all off
		if ((i%2)==0)
			*dio24_r6 = 0x1;	// LED D15 on
		else if ((i%3)==0)
			*dio24_r6 = 0x2;	// LED D13 on
		else
			*dio24_r6 = 0x4;	// LED D11 on
		
		usleep(1);	// requried for debugging processes

		i++;
		
		if (i >= 2000000)	// eventually when reaches the integer limit, need to reset back to 0
			i = 0;
		
		printf("i: %i\n", i);
	}

	cleanup(0);
	
	return 0;
}

void pc104init(void)
{
	printf("Initializing PC-104 expansion board\n");
	
	fdpc = open("/dev/mem", O_RDWR|O_SYNC);		
		  	
	if (fdpc == -1)			// Error checking purposes
	{
		perror("open /dev/mem");
		exit(1);
	}

	pc104_start = (unsigned char *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fdpc, PC104);

	if (MAP_FAILED == pc104_start)	// Error checking purposes
	{
		perror("mmap");
		exit(1);
	}

	// Assign the addresses for all of the registers
	printf("Assigning addresses for registers\n");
	printf("Board Identifier: R0\n");
	dio24_r0 = (unsigned char *) (pc104_start + DIO24_BASE + 0);
		  
	printf("Reserved: R1\n");
	dio24_r1 = (unsigned char *) (pc104_start + DIO24_BASE + 1);
		  
	printf("Jumper Status: R2\n");
	dio24_r2 = (unsigned char *) (pc104_start + DIO24_BASE + 2);
		  
	printf("Interrupt Control Register: R3\n");
	dio24_r3 = (unsigned char *) (pc104_start + DIO24_BASE + 3);
		  
	printf("Data Direction Register: R4\n");
	dio24_r4 = (unsigned char *) (pc104_start + DIO24_BASE + 4);
		  
	printf("Port A: R5\n");
	dio24_r5 = (unsigned char *) (pc104_start + DIO24_BASE + 5);
		  
	printf("Port B: R6\n");
	dio24_r6 = (unsigned char *) (pc104_start + DIO24_BASE + 6);
		  
	printf("Port C: R7\n");
	dio24_r7 = (unsigned char *) (pc104_start + DIO24_BASE + 7);
		  
	if (*dio24_r0 == 0x54)
	{
		printf("Card detected!!\n");
	}
	else
	{
		printf("Card not detected, check base address, or card not installed\n");
	}
		
	printf("Setting jumpers\n");
	// JP3 on => Port C outputs
	// JP4 on => Port B outputs
	*dio24_r2 = 0x0C; // (bit 0)JP1 off, JP2 off, JP3 on, JP4 on, (bit 5)JP5 off 
		  
	printf("Interupt Disabled\n");
	*dio24_r3 = 0x00; // disable all interupts

	printf("B for output and C output\n");
	*dio24_r4 = 0x03; // select ports B for output and C for output	
		
	printf("Port A off\n");
	*dio24_r5 = 0x00; //Port A not used.

	printf("Port B on\n");
	*dio24_r6 = 0xFF; //Port B on.
		    
	printf("Port C off\n");
	*dio24_r7 = 0xFF; //Port C off.
}

void com2init(void)
{
	printf("Initializing COM2 port\n");
	fd2 = open("/dev/ttyAM1", O_RDWR|O_NOCTTY|O_NDELAY);

	struct termios options;
	tcgetattr(fd2, &options);	// get terminal attributes
	    
	if (tcgetattr(fd2, &options) < 0)
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
	tcflush(fd2, TCIFLUSH);
	    
	if (tcflush(fd2, TCIFLUSH) < 0)
	{
		perror("TCflush error ");
		exit(-1) ;
	}

	// write the settings for the serial port
	tcsetattr(fd2, TCSANOW, &options);
	     
	if (tcsetattr(fd2,TCSANOW,&options) <0)
	{
		perror("Can't set terminal parameters ");
		exit(-1) ;
	}
}

void com3init(void)
{
	printf("Initializing COM3 port\n");
	fd3 = open("/dev/ttyTS0", O_RDWR|O_NOCTTY|O_NDELAY);

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
}

void lcdsetup(void)
{
	// Clear Screen
	clrscr[0] = 254;
	clrscr[1] = 88;
	write(fd2, clrscr, sizeof(clrscr));
	write(fd3, clrscr, sizeof(clrscr));
	usleep(1);

	// set cursor position for decimal point
	curpos[0] = 254;
	curpos[1] = 71;
	curpos[2] = 12;
	curpos[3] = 4;
	write(fd2, curpos, sizeof(curpos));
	write(fd3, curpos, sizeof(curpos));
	usleep(1);
    
	// used for decimal point
	write(fd2, "o", 1);
	write(fd3, "o", 1);
    usleep(1);
    
	// Initialize Large Numbers, which are 3 columns wide, 4 rows tall 
	larnum[0] = 254;
	larnum[1] = 110;
	write(fd2, larnum, sizeof(larnum));
	write(fd3, larnum, sizeof(larnum));
	usleep(1);
     
	// Place Large Number, start off with 000.00
	// Hundreths Place
	hplace[0] = 254;
	hplace[1] = 35;
	hplace[2] = 3;		// leave 2 columns at front empty
	hplace[3] = 0;
	write(fd2, hplace, sizeof(hplace));
	write(fd3, hplace, sizeof(hplace));
    usleep(1);
    
	// Tens Place
	tplace[0] = 254;
	tplace[1] = 35;
	tplace[2] = 6;
	tplace[3] = 0;
	write(fd2, tplace, sizeof(tplace));
	write(fd3, tplace, sizeof(tplace));
	usleep(1);
	
	// Ones place
	oplace[0] = 254;
	oplace[1] = 35;
	oplace[2] = 9;
	oplace[3] = 0;
	write(fd2, oplace, sizeof(oplace));
	write(fd3, oplace, sizeof(oplace));
	usleep(1);

	// After decimal point, tenths place
	dtplace[0] = 254;
	dtplace[1] = 35;
	dtplace[2] = 13;	// extra column left for decimal place
	dtplace[3] = 0;
	write(fd2, dtplace, sizeof(dtplace));
	write(fd3, dtplace, sizeof(dtplace));
	usleep(1);

	// hundreths place
	dhplace[0] = 254;
	dhplace[1] = 35;
	dhplace[2] = 16;
	dhplace[3] = 0;
	write(fd2, dhplace, sizeof(dhplace));
	write(fd3, dhplace, sizeof(dhplace));
	usleep(1);
}

void cleanup(int signum)
{
    printf("Sleep for a 1 second\n");
    sleep(1);
    
    printf("Clearing LCD screens\n");
    //Clear Screen
    write(fd2, clrscr, sizeof(clrscr));
    write(fd3, clrscr, sizeof(clrscr));
    
	printf("Closing file decriptors.\n");
    close(fdpc);
    close(fd2);
    close(fd3);
    
	printf("Exiting after abort received.\n\n");
	exit(1);
}

void wakeup(int signum) 
{
	int aph, apt, apo, apdt, apdh;
	int eph, ept, epo, epdt, epdh;
	
	// write azimuth position to LCD throught COM2
	aph = (int)(ap/100);
	apt = (int)((ap-(aph*100))/10);
	apo = (int)(ap-(aph*100)-(apt*10));
	apdt = (int)((ap-(aph*100)-(apt*10)-apo)*10);
	apdh = (int)((ap-(aph*100)-(apt*10)-apo-(apdt*.1))*100);
	printf("a places: %i, %i, %i, %i, %i\n", aph, apt, apo, apdt, apdh);
	
	switchthing(fd2, hplace, aph);
	switchthing(fd2, tplace, apt);
	switchthing(fd2, oplace, apo);
	switchthing(fd2, dtplace, apdt);
	switchthing(fd2, dhplace, apdh);	

	// write elevation position to LCD throught COM3
	eph = (int)(ep/100);
	ept = (int)((ep-(eph*100))/10);
	epo = (int)(ep-(eph*100)-(ept*10));
	epdt = (int)((ep-(eph*100)-(ept*10)-epo)*10);
	epdh = (int)((ep-(eph*100)-(ept*10)-epo-(epdt*.1))*100);
	printf("e places: %i, %i, %i, %i, %i\n", eph, ept, epo, epdt, epdh);
	
	switchthing(fd3, hplace, eph);
	switchthing(fd3, tplace, ept);
	switchthing(fd3, oplace, epo);
	switchthing(fd3, dtplace, epdt);
	switchthing(fd3, dhplace, epdh);
	
	// Set signal to this function
	signal(SIGALRM, wakeup);
	// Set alarm clock for .33 seconds
	ualarm(330000, 0);
}

double calcrand(int val)
{
	return ((double)(0 + (rand()%(1+(val-0)))));
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
	
	write(ff, place, sizeof(place));
}





