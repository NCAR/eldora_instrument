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
 *
 *        
 */

/* Global variables for using the Vme to Vme interfaces diagnostics only! */

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

/* Global "I have answered an interrupt" variable */
scope short in_vmevme_isr;
