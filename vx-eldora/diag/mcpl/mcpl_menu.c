/*
 *	$Id$
 *
 * 	Module:	mcpl_menu	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/05  16:26:21  thor
 * Initial revision
 *
 *
 * description: This function provides the main menu for the MCPL
 * diagnostics
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

/* Definitions for specific processors */
#define RADAR_NODE   6       /* Node number for the radar processor */
#define DISPLAY_NODE 3       /* Node number for the display processor */
#define RECORD_NODE  7       /* Node number for the data recording processor */
#define GRID_NODE    5       /* Node number for the data gridding processor */

mcpl_menu()
{
    long i, j;
    int input;
    short nodeId;

    init_pntrs();

    nodeId = *status_reg;

    nodeId >>= 11;

    nodeId &= 0x1f;
    
    do{
        printf("Note: Setup of MCPL as of March 1994 is: \n");
        printf("Display Processor Number 1:  Node %d Data Address: 10000000\n",
               DISPLAY_NODE);
        printf("Display Processor Number 2:  Node %d Data Address: 10000000\n",
               RADAR_NODE);
        printf("Recording Processor:         Node %d Data Address: 2000000\n",
               RECORD_NODE);
        printf("Fore radar Processor:        Node 4\n");
        printf("Aft radar Processor:         Node 2\n");
        printf("The current node ID is %d.\n",nodeId);
        printf("Enter the number in front of your choice\n");
        printf("0) Quit this program\n");
        printf("1) Reset MCPL in local crate\n");
        printf("2) Initialize MCPL in local crate\n");
        printf("3) Perform standard transmisson to remote crate\n");
        printf("4) Perform broadcast transmission to remote crate(s)\n");
        printf("5) Print TPBs.\n");
        printf("> ");
        scanf("%d",&input);
        getchar();

        switch(input)
          {
            case 1:
                mcpl_reset();
                break;
                
            case 2:
                mcpl_init();
                break;
                
            case 3:
                mcpl_reset();
                mcpl_init();
                stand_xfer();
                break;
                
            case 4:
                mcpl_reset();
                mcpl_init();
                bcast_xfer();
                break;

            case 5:
                print_tpbs();
                break;
                
            default:
                break;
          }

    } while(input != 0);

} /* End of the mcpl_menu function */
