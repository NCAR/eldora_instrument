//============================================================================
// Name        : ledbut.cpp
// Author      : Prerna Bang
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Blinks the green and red led on the ts-7260 board five times
// Makefile with TS7620 (ARM)
//============================================================================

#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<stdio.h>
#include<fcntl.h>

#define GPIOBASE 0x80840000		// GPIO  Control Registers Starting Address

int main(void)
{
   unsigned int *PEDR;
   unsigned char *start;
   int i;
   
   int fd = open("/dev/mem", O_RDWR);

   start = (unsigned char *)mmap(0, getpagesize(), PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIOBASE);
   
   PEDR = (unsigned int *)(start + 0x20);     // Green and Red LEDs address
  
   // blink 5 times, sleep 1 second so it's visible
   for (i = 0; i < 5; i++) {
      *PEDR = 0xff;		// On
      sleep(1);
      *PEDR = 0x00;		// Off
      sleep(1);
   }
   
   close(fd);
   
   return 0;
}
