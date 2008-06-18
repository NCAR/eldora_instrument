//============================================================================
// Name        : diolcd1.cpp
// Author      : Prerna Bang
// Version     : Worked of diotest1 and lcd4
// Copyright   : Your copyright notice
// Description : Routine to initialize and test the TS-DIO24 with the parallel LCD
// LCD is parallel LCD screen and is connected to LCD port on TS7260 board
// LCD is HD 44780 compatible and corresponding commands are used
// TSD1O24 is an 8 bit I/O expansion board which connects to the PC/104 interface on the TS7260
// The PC/104 interface on the TS7260 is 16 bit, but can be adjusted for 8 bit
// The TSDIO24 is connected to a 16 bit I/O HelloDevice Board which has LEDs and Push Buttons
// Only selected LEDs and push buttons on the HelloDevice are connected to the TSDIO24
//
// Only the LEDs are interacted with and the corresponding data is displayed on the LCD
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

void command(unsigned int);
void writechars(unsigned char *, int j);
unsigned int lcdwait(void);
void lcdinit(void);
void dioinit(void);


/********************************************************************/

int main(int argc, char *argv[])
{
	int i = 0;
	
	dioinit();
	printf ("DIO Initialize Done\n");
  
	lcdinit();
	printf ("LCD Initialize Done\n");
	
	printf("argc %d\n", argc);
	if (argc == 2) {
		writechars((unsigned char *)argv[1], 0);
	}
	if (argc > 2) {
		writechars((unsigned char *)argv[1], 0);
		lcdwait();
		command(0xa8); // set DDRAM addr to second row
		writechars((unsigned char *)argv[2], 0);
	}
	if (argc >= 2) return 0;
  		
	*dio24_r7 = 0x7F; // LED 17
	writechars((unsigned char *)"D17\n", i);
	
	*dio24_r7 = 0xBF; // LED 19
	i = i ^ 0x1;		// exclusive OR
	writechars((unsigned char *)"D19\n", i);
  
    *dio24_r7 = 0xDF; // LED 21
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D21\n", i);
  
    *dio24_r7 = 0xEF; // LED 23
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D23\n", i);
  
    *dio24_r7 = 0xF7; // LED 25
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D25\n", i);
  
	*dio24_r7 = 0xFB; // LED 27
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D27\n", i);

	*dio24_r7 = 0xFD; // LED 29
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D29\n", i);
  
	*dio24_r7 = 0xFE; // LED 31
    i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"D31\n", i);
  
  	*dio24_r7 = 0x00; //Port C all on.
  	i = i ^ 0x1;		// exclusive OR
  	writechars((unsigned char *)"Port C All On\n", i);

	*dio24_r6 = 0x00; //Port B all off.
	i = i ^ 0x1;		// exclusive OR
	writechars((unsigned char *)"Port B All Off\n", i);
   
	*dio24_r7 = 0xFF; //Port C off.
	i = i ^ 0x1;		// exclusive OR
	writechars((unsigned char *)"Port C All Off", i);
  
  	printf("Done\n");
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
	// JP3 on => Port C outputs
	// JP4 on => Port B outputs
	*dio24_r2 = 0x0C; // (bit 0)JP1 off, JP2 off, JP3 on, JP4 on, (bit 5)JP5 off 
	  
	printf("Interupt Disabled\n");
	*dio24_r3 = 0x00; // disable all interupts

	printf("B for output and C output\n");
	*dio24_r4 = 0x03; // select ports B for output and C for output
	    
	usleep(500000);

	printf("Port A\n");
	*dio24_r5 = 0x00; //Port A not used.

	printf("Port B on\n");
	*dio24_r6 = 0xFF; //Port B on.
	  
	printf("Port C off\n");
	*dio24_r7 = 0xFF; //Port C off.	
	
	usleep(500000);
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

void writechars(unsigned char *dat, int j) {
	int i;
	unsigned int ctrl = *phdr;

	if (j) {		// i fluctuates between 0 and 1 due to XOR
		command(0xa8); // set DDRAM addr to second row
	} else {
		command(0x2); // return home
		lcdwait();
		command(0x1); // clear display
		lcdwait();	
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
	usleep(500000);
}

