
//============================================================================
// Name        : lcd3.cpp
// Author      : Prerna Bang
// Version     : Worked of lcd2
// Copyright   : Your copyright notice
// Description : Displays combination of p, s and t characters on LCD 
//					 - testing out refresh rates and delay times
// LCD is parallel LCD screen and is connected to LCD port on TS7260 board
// LCD is HD 44780 compatible and corresponding commands are used
// Makefile with TS7620 (ARM)
//============================================================================

#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<stdio.h>
#include<fcntl.h>
#include<string.h>

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

void command(unsigned int);
void writechars(unsigned char *);
unsigned int lcdwait(void);
void lcdinit(void);

int main(int argc, char **argv) {
	int i = 0;
	int j = 0;
	
	unsigned char buf2[512];
	
	lcdinit();
	
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

	//unsigned char buf[512];
	while(!feof(stdin)) {
		unsigned char p = 'p';
		printf("i %d\n", i);

		lcdwait();
		
		if (i) {		// i fluctuates between 0 and 1 due to XOR
			command(0xa8); // set DDRAM addr to second row
		} else {
			command(0x2); // return home
			command(0x1); // clear display
		}
		i = i ^ 0x1;		// exclusive OR
		
		buf2[i] = p;
		buf2[j] = 's';
		buf2[8] = 't';
		j++;
		if (j == 10) j = 0;
		writechars(buf2);
		usleep(1000000);	
	}
	
	printf("Done\n");
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
	
	//printf("phdr: %p\n", phdr);
	//printf("phdr: %x\n", *phdr);
	
	//printf("CLEARED, paddr = 0x%x \n", *paddr);

	*paddr = 0x0;  // All of port A to inputs, page 39 Hardware Manual

	*phddr |= 0x38; // bits 3:5 of port H to outputs
	*phdr &= ~0x18; // de-assert EN, de-assert RS
	
	usleep(10);		// in microseconds
	
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
	int i, dat, tries = 0;
	unsigned int ctrl = *phdr;
	
	//printf("CLEARED, paddr = 0x%x, pcddr = 0x%x\n", *paddr, *pcddr);

	*paddr = 0x0;  // All of port A to inputs
	
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
		//printf("dat = 0x%x, *padr = 0x%x, *pcdr = 0x%x\n", dat, *padr, *pcdr);
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

	//printf("SET, *paddr = 0x%x \n", *paddr);	
	*paddr = 0xff; // set port A to outputs

	// step 1, apply RS & WR, send data
	//printf("*padr = 0x%x\n", *padr);
	//printf("cmd = 0x%x, *padr = 0x%x \n", cmd, *padr);
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

	do {
		lcdwait();
		//printf("SET, *paddr = 0x%x, *pcddr = 0x%x\n", *paddr, *pcddr);
		*paddr = 0xff; // set port A to outputs

		// step 1, apply RS & WR, send data
		// printf("dat = 0x%x, *padr = 0x%x \n", *dat, *padr);	
		*padr = *dat++;
		ctrl |= 0x10; // assert RS
		ctrl &= ~0x20; // assert WR
		*phdr = ctrl;

		// step 2
		i = SETUP;
		COUNTDOWN(i);

		// step 3, assert EN
		ctrl |= 0x8;
		*phdr = ctrl;

		// step 4, wait 800 nS
		i = PULSE;
		COUNTDOWN(i);

		// step 5, de-assert EN	
		ctrl &= ~0x8; // de-assert EN
		*phdr = ctrl;

		// step 6, wait
		i = HOLD;
		COUNTDOWN(i);
		
	} while(*dat);
}

