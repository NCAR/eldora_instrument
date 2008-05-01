/*
 *	$Id$
 *
 *	Module:	ATOD_HEXD	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/11/09  17:00:21  eric
 * Initial revision
 *
 * Revision 1.1  1991/12/04  16:09:13  eric
 * Initial revision
 *
 *
 * description:
 *
 * subroutine to hex dump the a/d card memory 
 * 
 * Arguments
 *
 * 1. Board
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "stdioLib.h"
#include "ELDRP7.h"

void atod_hexd(bd)
unsigned long bd;

{
/* assign pointers to the various A/D card registers */

    long *atod_qh;
    long *atod_ql;
    long *atod_ih;
    long *atod_il;
    unsigned char *atod_c;
    long k,i,temp;
    
    atod_qh = (long *) (bd + ATOD_QH);
    atod_ql = (long *) (bd + ATOD_QL);
    atod_ih = (long *) (bd + ATOD_IH);
    atod_il = (long *) (bd + ATOD_IL);
    atod_c = (unsigned char *) (bd + ATOD_CH);

  menu: ;
    printf("Select your desired choice\n");
    printf("1) In-Phase, high dynamic range, memory dump\n");
    printf("2) In-Phase, low dynamic range, memory dump\n");
    printf("3) Quaduarature, high dynamic range, memory dump\n");
    printf("4) Quadurature, low dynamic range, memory dump\n");
    printf("5) Choice memory dump\n");
    printf("6) Z memory dump\n");
    printf("7) End routine\n");

    scanf("%d",&temp);

    switch ( temp ) {
      case 1:                  /* inphase memory high dynamic range */
	printf("       In Phase Memory High Dynamic Range\n");
         for(k = 0; k < 0x1000; k = k + 10) {
	     printf("  %8lx",atod_ih + k);
	     for(i = 0; i < 10; i++)
               printf("  %5lx",*(atod_ih + k + i));
	     printf ("\n");
         }
	break;

      case 2:                 /* inphase memory low dynamic range */
	printf("       In Phase Memory Low Dynamic Range\n");
	for(k = 0; k < 0x1000; k = k + 10) {
            printf("  %8lx",atod_il + k);
            for(i = 0; i < 10; i++)
	      printf("  %5lx",*(atod_il + k + i));
	    printf ("\n");
	}
	break;
	
      case 3:                /* quadrature memory high dynamic range */
	printf("       Quadrature Memory High Dynamic Range\n");
	for(k = 0; k < 0x1000; k = k + 10) {
            printf("  %8lx",atod_qh + k);
            for(i = 0; i < 10; i++)
	      printf("  %5lx",*(atod_qh + k + i));
	    printf ("\n");
	}
	break;
	
      case 4:               /* quadrature memory low dynamic range */
	printf("       Quadrature Memory Low Dynamic Range\n");
	for(k = 0; k < 0x1000; k = k + 10) {
            printf("  %8lx",atod_ql + k);
            for(i = 0; i < 10; i++)
	      printf("  %5lx",*(atod_ql + k + i));
	    printf ("\n");
	}
	break;
	
      case 5:
	printf("       Choice memory\n");
	for(k = 1; k < 0x20000; k = k + 48) {
            printf("  %2x",atod_c + k);
            for(i = 0; i < 8; i = i + 2)
	      printf("  %2x",*(atod_c + k + i));
	    printf ("\n");
	}
	break;
	
      case 7:
	return;
	break;
	
      default:
	printf("Illegal option entered, please re-enter\n");
	break;
    }
    goto menu;
}

