/*
 *	$Id$
 *
 *	Module: para_menu.c		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.4  1996/01/25  22:41:26  craig
 * Added the  piezo screw control code
 *
 * Revision 1.3  1995/09/25  17:45:50  martin
 * updated from Craig's current code
 *
 * Revision 1.1  1995/07/11  21:09:07  craig
 * Initial revision
 *
 *
 *
 * description:  This is the debugging/trouble shooting menu for the 
 *               SABL parallel input output board.
 *        
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "tapeDef.h"
#include "paraDef.h"
#include "paraGbl.h"

void para_menu(void)
{

int cmd, count, i, invalue, j;
unsigned char value;
char charvalue;

count = 20;

/* Just sit in the menu until told to quit */

do{
    printf("\nPARALLEL INPUT/OUTPUT BOARD MENU CHOICES:\n\n");
    printf("0) End Parallel I/O menu\n");
    printf("1) Initialize the Parallel I/O interface\n");
    printf("2) Change number of times to execute command = %d\n",count);
    printf("3) Read input from port A\n");
    printf("4) Read input from port B\n");
    printf("5) Read input from port C\n");
    printf("6) Read input from port F\n");
    printf("7) Set port D bits on, currently = %2x\n",curr_pio_d);
    printf("8) Set port E bits on, currently = %2x\n",curr_pio_e);
    printf("9) Set port D bits off, currently = %2x\n",curr_pio_d);
    printf("10) Set port E bits off, currently = %2x\n",curr_pio_e);
 
/* read and process the command */

    scanf(" %d",&cmd);
    getchar();

    switch(cmd)
      {
	case 1:	/* Initialize the interface */
	  para_init();
	  break;

	case 2:	/* Change the number of time to execute the commands */
	  printf("Enter the number of time to execute the commands\n");
	  scanf(" %d",&count);
	  break;

	case 3:	/* Read port A  */
	  for(i=0; i<count; i++)
	    {
		value = para_in('a');
		printf("Port A value read = %2x\n",value);
	    }
	  break;

	case 4:	/* Read port B  */
	  for(i=0; i<count; i++)
	    {
		value = para_in('b');
		printf("Port B value read = %2x\n",value);
	    }
	  break;

	case 5:	/* Read port C  */
	  for(i=0; i<count; i++)
	    {
		value = para_in('c');
		printf("Port C value read = %2x\n",value);
	    }
	  break;

	case 6:	/* Read port F  */
	  for(i=0; i<count; i++)
	    {
		value = para_in('f');
		printf("Port F value read = %2x\n",value);
	    }
	  break;

	case 7:	/* Set bits in port D */
	  printf("Enter the hexidecimal byte (2 characters)\n");
	  scanf(" %x",&invalue);
	  charvalue = invalue & 0xFF;
	  para_on('d',charvalue);
	  break;

	case 8:	/* Set bits in port E */
	  printf("Enter the hexidecimal byte (2 characters)\n");
	  scanf(" %x",&invalue);
	  charvalue = invalue & 0xFF;
	  para_on('e',charvalue);
	  break;

	case 9:	/* Clear bits in port D */
	  printf("Enter the hexidecimal byte (2 characters)\n");
	  scanf(" %x",&invalue);
	  charvalue = invalue & 0xFF;
	  para_off('d',charvalue);
	  break;

	case 10:  /* Clear bits in port E */
	  printf("Enter the hexidecimal byte (2 characters)\n");
	  scanf(" %x",&invalue);
	  charvalue = invalue & 0xFF;
	  para_off('e',charvalue);
	  break;

	default:
	  break;
      }
}while(cmd != 0);

return;
}
