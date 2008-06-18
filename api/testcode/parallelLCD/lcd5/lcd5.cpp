//============================================================================
// Name        : lcd5.cpp
// Author      : Prerna Bang
// Version     : Worked of lcd4 
// Copyright   : Your copyright notice
// Description : Display number icnrementation from 0.00 to 359.99 on parallel LCD
// Once reached 359.99, loops back around to 0.00
// Different sleep intervals are tested
//
// LCD is parallel LCD screen and is connected to LCD port on TS7260 board
// LCD is HD 44780 compatible and corresponding commands are used
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

/********************************************************************/

int main(int argc, char *argv[])
{
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
	
	sleep(1);
	char val[7] ={(char)48, (char)48, (char)48, (char)46, (char)48, (char)48};
	// null is 0  <= gives black square on screen
	// end of text is 3
	// space is 32
	//. is 46
	// 48 to 57 is 0 to 9
	
 	while (1)
 	{
 		if (val[5] == '9')
 		{	
 			if (val[4] =='9')
 			{
 				if (val[2] == '9')
 				{
 					if (val[0] == '3' && val[1] == '5')
 					{
 						val[0] = (char)48;
 						val[1] = (char)48;
 						val[2] = (char)48;
 						val[4] = (char)48;
 						val[5] = (char)48;
 					}
 					else if (val[1] == '9')
 					{
 						val[0] = val[0]+1;
 						val[1] = (char)48;
 						val[2] = (char)48;
 						val[4] = (char)48;
 						val[5] = (char)48;	
 					}
 					else
 					{
 						val[1] = val[1]+1;
 						val[2] = (char)48;
 						val[4] = (char)48;
 						val[5] = (char)48;
 					}
 				}
 				else
 				{	
 					val[2] = val[2]+1;
 					val[4] = (char)48;
 					val[5] = (char)48;
 				}
 			}
 			else
 			{	
 				val[4] = val[4]+1;
 				val[5] = (char)48;
 			}
 		}
 		else
 		{	
 			val[5] = val[5]+1;
 		}
	
 		writechars((unsigned char *)val);
 	}
 	 	
    
 	//writechars((unsigned char *)"Done...\n");
 	
 	printf("Done\n");
	command(0x1); // clear display
  	return 0;
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
	
	usleep(5000);
	command(0x38); // 2 rows, 5x7, 8 bit
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
	usleep(1);			
	// with no sleep, entire loop cycle takes 37.123 seconds
	// the incrementation goes by way to fast on the screen - very hard to see
	
	// usleep(1), 11 minutes and 52.623 seconds 
	// the display still gets updated pretty fast, but it is visible
	// last decimal digit updates to quickly for the eye, though it is obvious what is happening

	// usleep(125000), 1 hour, 12 minutes and 27.165 seconds
	// every .01 incrementation can be seen on the LCD screen 
}

