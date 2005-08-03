 /*

                              wait13


Routine for setting up DSP32C for 1 wait state A memory and 
3 wait state B memory. This program is to be used in conjunction
with semi static bus test programs to verify basic functionality of
HSDBA and HSDBB interfaces.

9/23/91
     Modified for DSP32C coding.

*/
.global start, wait
.=0x0000
          r10 = 0x0B   /* A mem = 1, B mem = 3 */
	  pcw = r10 
          ioc = 0x0554
          dauc = 0
          goto start
	  nop
		
gates:    int 4
samples:  int 64
.align 4
scale:    float 2048.
.align 4

          .=0x100

start:    r3 = 0xFFDF00
	  a0 = *r3	      /* clear input FIFO */	
wait:     nop                 /* just idle here until test is over */
	  nop
          goto wait           /* and around again */
	  nop




