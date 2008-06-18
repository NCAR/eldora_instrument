//============================================================================
// Name        : diotest2.cpp
// Author      : Prerna Bang
// Version     : Worked off diotest1
// Copyright   : Your copyright notice
// Description : Routine to initialize and test the TS-DIO24 
// TSD1O24 is an 8 bit I/O expansion board which connects to the PC/104 interface on the TS7260
// The PC/104 interface on the TS7260 is 16 bit, but can be adjusted for 8 bit
// The TSDIO24 is connected to a 16 bit I/O HelloDevice Board which has LEDs and Push Buttons
// Only selected LEDs and push buttons on the HelloDevice are connected to the TSDIO24
// In this test, push buttons and LEDs work together.
// When a particular push button is pressed, corressponding LEDs on the other port light up.
// One of the push button is an exit button
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

#define PC104	0x11E00000		// 8 bit I/O Cycle Starting Address

// jumper settings for base address selection
// JP1 and JP2 are off
#define DIO24_BASE (0x0100)

/********************************************************************/

int main(int argc, char *argv[])
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

	//printf("Map addr of pc104_start: %p\n", pc104_start);
	//printf("Map addr of pc104_start: %02x\n", (int) *pc104_start);
 
	// Assign the addresses for all of the registers
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
 	usleep(500000);
 
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
    
 	usleep(500000);
 	
 	printf("Start Push Buttons\n");	
 	
 	while (1)
 	{
 		usleep(100000);	// determined through Trail-and-Error 
 						// this was a good number for 3 push buttons and kind-of worked for 8
 		
 		if (*dio24_r7 == 0xBF){
 			printf("Switch 3: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x40;
 		}
 		
 		if (*dio24_r7 == 0xDF){
 		 	printf("Switch 5: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x20;
 		}
 		
 		if (*dio24_r7 == 0xEF){
 		 	printf("Switch 7, Exiting: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x10;
 		 	break;
 		}
 		
 		if (*dio24_r7 == 0xF7){
 			printf("Switch 9: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x8;
 		}
 		
 		if (*dio24_r7 == 0xFB){
 		 	printf("Switch 11: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x4;
 		}
 		
 		if (*dio24_r7 == 0xFD){
 		 	printf("Switch 13: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x2;
 		}
 
 		if (*dio24_r7 == 0xFE){
 		 	printf("Switch 15: %02x\n",(int) *dio24_r7);
 			*dio24_r6 = 0x1;
 		}
 
 	}
 	
 	printf("Port B turn off\n");
 	*dio24_r6 = 0x00; //Port B all off.
 	
 	printf("Done\n");
 	close(fd);
}
