//============================================================================
// Name        : diolcd2.cpp
// Author      : Prerna Bang
// Version     : Worked of diotest2 and diolcd1
// Copyright   : Your copyright notice
// Description : Routine to initialize and test the TS-DIO24 with the parallel LCD
// LCD is parallel LCD screen and is connected to LCD port on TS7260 board
// LCD is HD 44780 compatible and corresponding commands are used
// TSD1O24 is an 8 bit I/O expansion board which connects to the PC/104 interface on the TS7260
// The PC/104 interface on the TS7260 is 16 bit, but can be adjusted for 8 bit
// The TSDIO24 is connected to a 16 bit I/O HelloDevice Board which has LEDs and Push Buttons
// Only selected LEDs and push buttons on the HelloDevice are connected to the TSDIO24
//
// The LEDs and Push Buttons are used in this code.
// The corresponding data is displayed on the LCD
// Sleep delays are used to see the lighting on and off effect
// Take note of the Jumper Settings of the TSDIO24
// Makefile with TS7620 (ARM)
//============================================================================

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

#define GPIOBASE	0x80840000		// GPIO  Control Registers Starting Address

#define PC104	0x11E00000		// 8 bit I/O Cycle Starting Address

// Jumper settings for base address selection: JP1 and JP2 are off
#define DIO24_BASE (0x0100)

#define PADR	0
#define PADDR	(0x10 / sizeof(unsigned int))		// 4
#define PAMASK  0x7F  
#define PCMASK  0x01  
#define PHDR	(0x40 / sizeof(unsigned int))		// 10
#define PHDDR	(0x44 / sizeof(unsigned int))		// 11

// Pre-calibrated: delay values for EP9302, with CPU running at 200MHz 
#define SETUP	15
#define PULSE	36
#define HOLD	22

// Assembler Instructions
// A delay loop used by the LCD driver to wait a number of nanoseconds. 
#define COUNTDOWN(x)	asm volatile ( \
  "1:\n"\
  "subs %1, %1, #1;\n"\
  "bne 1b;\n"\
  : "=r" ((x)) : "r" ((x)) \
);

// TS-DI024 (PC/104) => 8 bit read/write to I/O
volatile unsigned char *pc104_start;

volatile unsigned char *dio24_r0;
volatile unsigned char *dio24_r1;
volatile unsigned char *dio24_r2;
volatile unsigned char *dio24_r3;
volatile unsigned char *dio24_r4;
volatile unsigned char *dio24_r5;
volatile unsigned char *dio24_r6;
volatile unsigned char *dio24_r7;

volatile unsigned int *gpio;

volatile unsigned int *phdr;
volatile unsigned int *phddr;
volatile unsigned int *padr;
volatile unsigned int *paddr;

unsigned int num;		// To switch between first and second line of LCD screen

void command(unsigned int);
void writechars(unsigned char *);
unsigned int lcdwait(void);
void lcdinit(void);
void dioinit(void);

/********************************************************************/

int main(int argc, char *argv[])
{
	dioinit();
	printf ("DIO Initialize Done\n");
  
	lcdinit();
	printf ("LCD Initialize Done\n");
	
	printf("argc %d\n", argc);
	if (argc == 2) {
		writechars((unsigned char *)argv[1]);
	}
	if (argc > 2) {
		writechars((unsigned char *)argv[1]);
		lcdwait();
		command(0xa8); // set DDRAM addr to second row
		writechars((unsigned char *)argv[2]);
	}
	if (argc >= 2) return 0;
	
	writechars((unsigned char *)"Press Push Buttons\n");
 	
 	while (1)
 	{
 		//usleep(100000);	// Trail-and-Error, this was a good number for 3 push buttons 
 							// and kind of worked for 8 as well
 							// When writing to the LCD did not really need it
 							// due to the other pauses in other functions
 	 		
 		if (*dio24_r7 == 0xBF){
 			*dio24_r6 = 0x40;
 			writechars((unsigned char *)"Switch 3\n");
 		}
 	 		
 		if (*dio24_r7 == 0xDF){
 			*dio24_r6 = 0x20;
 			writechars((unsigned char *)"Switch 5\n");
 		}
 	 		
 		if (*dio24_r7 == 0xEF){
 			*dio24_r6 = 0x10;
 			writechars((unsigned char *)"Switch 7\n");
 			writechars((unsigned char *)"Exiting..\n");
 			break;
 		}
 	 		
 		if (*dio24_r7 == 0xF7){
 			*dio24_r6 = 0x8;
 			writechars((unsigned char *)"Switch 9\n");
 		}
 	 		
 		if (*dio24_r7 == 0xFB){
 			*dio24_r6 = 0x4;
 			writechars((unsigned char *)"Switch 11\n");
 		}
 	 		
 		if (*dio24_r7 == 0xFD){
 			*dio24_r6 = 0x2;
 			writechars((unsigned char *)"Switch 13\n");
 		}
 	 
 		if (*dio24_r7 == 0xFE){
 			*dio24_r6 = 0x1;
 			writechars((unsigned char *)"Switch 15\n");
 		} 		
 	}
 	 	
 	*dio24_r6 = 0x00; //Port B all off.
 	writechars((unsigned char *)"Port B Off\n");
    
 	writechars((unsigned char *)"Done...\n");
 	
 	printf("Done\n");
	command(0x1); // clear display
  	return 0;
}

void dioinit(void)
{
	int fd = open("/dev/mem", O_RDWR|O_SYNC);		
	  	
	if (fd == -1)			// Error checking purposes
	{
		perror("open /dev/mem");
	    exit(1);
	}

	pc104_start = (unsigned char *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, PC104);

	if (MAP_FAILED == pc104_start)	// Error checking purposes
	{
		perror("mmap");
	    exit(1);
	}

	// Assign the addresses for all of the registers
	//printf("Board Identifier: R0\n");
	dio24_r0 = (unsigned char *) (pc104_start + DIO24_BASE + 0);
	  
	//printf("Reserved: R1\n");
	dio24_r1 = (unsigned char *) (pc104_start + DIO24_BASE + 1);
	  
	//printf("Jumper Status: R2\n");
	dio24_r2 = (unsigned char *) (pc104_start + DIO24_BASE + 2);
	  
	//printf("Interrupt Control: R3\n");
	dio24_r3 = (unsigned char *) (pc104_start + DIO24_BASE + 3);
	  
	//printf("Data Direction: R4\n");
	dio24_r4 = (unsigned char *) (pc104_start + DIO24_BASE + 4);
	  
	//printf("Port A: R5\n");
	dio24_r5 = (unsigned char *) (pc104_start + DIO24_BASE + 5);
	  
	//printf("Port B: R6\n");
	dio24_r6 = (unsigned char *) (pc104_start + DIO24_BASE + 6);
	  
	//printf("Port C: R7\n");
	dio24_r7 = (unsigned char *) (pc104_start + DIO24_BASE + 7);
	  
	if (*dio24_r0 == 0x54)
	{
		printf("Card detected\n");
	}
	else
	{
	      printf("Card not detected\n");
	}
	  
	printf("Jumpers\n");
	// JP3 off => Port C inputs
	// JP4 on => Port B outputs
	*dio24_r2 = 0x08; // (bit 0)JP1 off, JP2 off, JP3 off, JP4 on, (bit 5)JP5 off 
	  
	printf("Interupt Disabled\n");
	*dio24_r3 = 0x00; // disable all interupts

	printf("B for output and C input\n");
	*dio24_r4 = 0x02; // select ports B for output and C for input
	 	
	printf("Port B turn off\n");
	*dio24_r6 = 0x00; //Port B all off.
}

void lcdinit(void) {
	
	int fd = open("/dev/mem", O_RDWR|O_SYNC);

	gpio = (unsigned int *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIOBASE);

	//printf("%x \n", GPIOBASE);
	//printf("Map addr of gpio: %p\n", gpio);

	phdr = &gpio[PHDR];
	padr = &gpio[PADR];
	paddr = &gpio[PADDR];
	phddr = &gpio[PHDDR];

	*paddr = 0x0;  // All of port A to inputs, page 39 Hardware Manual

	*phddr |= 0x38; // bits 3:5 of port H to outputs
	*phdr &= ~0x18; // de-assert EN, de-assert RS
	
	command(0x38); // two rows, 5x7, 8 bit
	usleep(4100);
	command(0x6); // cursor increment mode
	lcdwait();
	command(0x1); // clear display
	lcdwait();
	command(0xc); // display on, blink off, cursor off
	lcdwait();
	command(0x2); // return home
}

unsigned int lcdwait(void) {
	int i;
	int dat;
	int tries = 0;
	unsigned int ctrl = *phdr;
	
	*paddr = 0x0;  // All of port A to inputs

	ctrl = *phdr;
	
	do {
		// step 1, apply RS & WR
		ctrl |= 0x30; // de-assert WR
		ctrl &= ~0x10; // de-assert RS
		*phdr = ctrl;

		// step 2, wait
		i = SETUP;
		COUNTDOWN(i);

		// step 3, assert EN
		ctrl |= 0x8;
		*phdr = ctrl;

		// step 4, wait
		i = PULSE;
		COUNTDOWN(i);

		// step 5, de-assert EN, read result
		dat = *padr;
		ctrl &= ~0x8; // de-assert EN
		*phdr = ctrl;

		// step 6, wait
		i = HOLD;
		COUNTDOWN(i);
		
	} while (dat & 0x80 && tries++ < 1000);
	return dat;
}

void command(unsigned int cmd) {
	int i;
	unsigned int ctrl = *phdr;

	*paddr = 0xff; // set port A to outputs

	// step 1, apply RS & WR, send data
	*padr = cmd;
	ctrl &= ~0x30; // de-assert RS, assert WR
	*phdr = ctrl;

	// step 2, wait
	i = SETUP;
	COUNTDOWN(i);

	// step 3, assert EN
	ctrl |= 0x8;
	*phdr = ctrl;

	// step 4, wait
	i = PULSE;
	COUNTDOWN(i);

	// step 5, de-assert EN	
	ctrl &= ~0x8; // de-assert EN
	*phdr = ctrl;

	// step 6, wait 
	i = HOLD;
	COUNTDOWN(i);
}

void writechars(unsigned char *dat) {
	int i;
	unsigned int ctrl = *phdr;

	num = num ^ 0x1;		// exclusive OR
	
	if (num) {		// i fluctuates between 0 and 1 due to XOR
		command(0xa8); // set DDRAM addr to second row
	} else {
		command(0x2); // return home
		lcdwait();
		command(0x1); // clear display
	}
	
	do {
		lcdwait();
		
		*paddr = 0xff; // set port A to outputs

		// step 1, apply RS & WR, send data
		//printf("dat = 0x%x, *padr = 0x%x \n", *dat, *padr);
	
		*padr = *dat++;

		ctrl |= 0x10; // assert RS
		ctrl &= ~0x20; // assert WR
		*phdr = ctrl;

		// step 2, wait
		i = SETUP;
		COUNTDOWN(i);
		
		// step 3, assert EN
		ctrl |= 0x8;
		*phdr = ctrl;
		
		// step 4, wait
		i = PULSE;
		COUNTDOWN(i);

		// step 5, de-assert EN	
		ctrl &= ~0x8; // de-assert EN
		*phdr = ctrl;
		
		// step 6, wait 
		i = HOLD;
		COUNTDOWN(i);
		
	} while(*dat);
	usleep(200000);			// After trial and error, optimal sleep time
							// Gives code enough time to go through everything 
							// and detect new push button
}

