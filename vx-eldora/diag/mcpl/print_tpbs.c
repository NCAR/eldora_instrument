/*
 *	$Id$
 *
 *	Module:	print_tpbs.c	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: This module prints out each of the tpbs that command the
 *              the Multi-Crate Pipeline (MCPL).
 *
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "mcplDef.h"
#include "mcplGbl.h"

print_tpbs()
{
int input, *int_pntr, i, j;

do{
    printf("\nPrints TPBs menu choices:\n");
    printf("0) Exit, return to main menu\n");
    printf("1) Print initialization TPB which resides at: %x \n",init);
    printf("2) Print the tpb_to_proc TPB which resides at: %x \n",tpb_to_proc);
    printf("3) Print the receive_stand TPB which resides at: %x \n",
	   receive_stand);
    printf("4) Print the receive_bcast TPB which resides at: %x \n",
	   receive_bcast);
    printf("5) Print the stand_to_proc TPB which resides at: %x \n",
	   stand_to_proc);
    printf("6) Print the bcast_tpb TPB which resides at: %x \n",bcast_tpb);
    printf("7) Print the data which resides at: %x \n",data_start);
    printf("8) Print the mailbox which resides at: %x \n",mailbox);
    scanf("%d",&input);
    getchar();

    switch(input)
      {
	case 1:
	  print_tpb_struct(init);
	  break;
	case 2:
	  print_tpb_struct(tpb_to_proc);
	  break;
	case 3:
	  print_tpb_struct(receive_stand);
	  break;
	case 4:
	  print_tpb_struct(receive_bcast);
	  break;
	case 5:
	  print_tpb_struct(stand_to_proc);
	  break;
	case 6:
	  print_tpb_struct(bcast_tpb);
	  break;
	case 7:
	  for(i=0; i<40; i=i+4)
	    {
		int_pntr = data_start + i;
		printf("\n%8x: ",int_pntr);
		for(j=0; j<4; j++)
		  printf("%8x ",*(int_pntr + j));
	    }
	  break;
	case 8:
	  printf("Mailbox value is: %x\n",*mailbox);
	  break;
	default:
	  break;
      }
}while(input != 0);

}
print_tpb_struct(struct TPB *pntr)
{

printf("Source Start Address: %8x \n",pntr->source_start_address);
printf("Sink Start Address:   %8x \n",pntr->sink_start_address);
printf("Source Attribute:     %4x \n",pntr->source_attribute);
printf("Sink Attribute:       %4x \n",pntr->sink_attribute);
printf("Transfer Length:      %ld \n",pntr->transfer_length);
printf("Completion Status:    %4x \n",pntr->completion_status);
printf("NACK Count:           %d \n",pntr->nack_count);
printf("Link Address:         %8x \n",pntr->link_address);
printf("Link Attribute:       %4x \n",pntr->link_attribute);
printf("MCPL Control Word:    %4x \n",pntr->mcpl_control_word);
printf("Mail Box Address:     %8x \n",pntr->mailbox_address);


}

































