/*
 *
 *	$Id$
 *
 *	Module:	cntrlGbl.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/08/14  21:36:36  reif
 * Initial revision
 *
 *
 * description: These are the global variables associated with
 *              the motor controller.             
 */

scope unsigned char *mot_timer;
scope unsigned char *vel_lsb;
scope unsigned char *vel_msb;
scope unsigned char *mot_prog_cntr;
scope unsigned char *mot_status;
scope unsigned char *mot_flag;
scope unsigned char *mot_gain;
scope unsigned char *mot_cmnd;
scope unsigned char *mot_reset;
scope unsigned char *pin_stat;
scope unsigned char *mot_ltch;
scope unsigned char *rd2016;
scope unsigned char *act_vel_lsb;
scope unsigned char *act_vel_msb;

/* Global variables for the integrator function */
scope float rpm;
scope float delta_degpersec;
scope float integrator_gain;











