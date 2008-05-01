/*
 *	$Id$
 *
 *	Module clear_int.c 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1994/05/12  15:07:50  craig
 * Initial revision
 *
 *
 * description:  This program clears an interrupt in the Housekeeping
 *               Processor caused by one of the radar processors.
 *
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

/* Include needed vx-works .h files */

#define scope extern
#include <vxWorks.h>
#include <stdioLib.h>
#include <math.h>
#include <vmevmeDiag.h>
#include <vmevmeAdr.h>
#include <vme_hndshk.h>
#include <tp41vAdr.h>
#include <vmevmeDef.h>
#include <vmevmeGbl.h>
#include <hskpInt.h>

clear_int()
{
char stat;

stat = *fore_remote_status;

if((stat & 0x02) != 0)
  {
      *fore_remote_command = 0x40;
      fore_int_cnt++;
      printf("f");
  }

stat = *aft_remote_status;

if((stat & 0x02) != 0)
  {
      *aft_remote_command = 0x40;
      aft_int_cnt++;
      printf("a");
  }


}
