/*
 *	$Id$
 *
 *	Module:	sum_testpulse	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/09/12  17:21:56  craig
 * Initial revision
 *
 *
 * description: This module sums up the current readings of received testpulse
 *              power and velocity and measured testpulse power.
 *              
 */

static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#define scope extern
#include "hskpAll.h"

void sum_testpulse()
{
tp_sum_count += 1;

fore_dbz_sum += fore_vmehndshk->tpulse_level_proc;
aft_dbz_sum += aft_vmehndshk->tpulse_level_proc;

fore_vel_sum += fore_vmehndshk->tpulse_vel;
aft_vel_sum += aft_vmehndshk->tpulse_vel;

fore_tp_level = fore_testp_pwr;
aft_tp_level = aft_testp_pwr;
}





