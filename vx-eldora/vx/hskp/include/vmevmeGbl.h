/*
 *	$Id$
 *
 *	Module:	vmevmeGbl.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.7  1993/09/20  16:39:19  craig
 * *** empty log message ***
 *
 * Revision 1.6  1992/11/03  22:53:59  craig
 * *** empty log message ***
 *
 * Revision 1.1  1992/09/01  16:51:08  craig
 * Initial revision
 *
 *        
 */

/* Global variables for using the Vme to Vme interfaces */

scope struct vmevmehndshk *fore_vmehndshk;
scope struct vmevmehndshk *aft_vmehndshk;
scope char *fore_local_status;
scope char *fore_local_command;
scope char *fore_remote_status;
scope char *fore_remote_command;
scope char *aft_local_status;
scope char *aft_local_command;
scope char *aft_remote_status;
scope char *aft_remote_command;
scope SEM_ID vmeSem;

/* Global variables to keep track of position in the radar handshake areas */

scope long current_offset;
scope long current_index;

scope long current_platform_index;
scope long current_platform_offset;
scope short platform_status[27];

scope long iru_lag_offset;
scope long iru_lag_index;

/* Global variables to keep track of position in the nav handshake areas */

scope long nav_current_size;
scope long nav_current_index;

scope struct nav_data *current_nav_pntr;
scope char nav_flag;
scope char nav_mask;

/* Global variables to keep track of position in all of the handshake areas */

scope long ads_current_offset;
scope long ads_current_index;

/* Global "I have answered an interrupt" variable */
scope short in_vmevme_isr;

/* Global "use fake angles flag" */
scope int fake_angles;
