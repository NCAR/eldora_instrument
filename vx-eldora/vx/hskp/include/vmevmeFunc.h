/*
 *	$Id$
 *
 *	Module:	vmevmeFunc.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1992/09/01  16:51:08  reif
 * Initial revision
 *        
 */

/* Functions used with the vme to vme interfaces */

scope void init_vmevme();
scope void stop_vmevme();
scope void start_vmevme();
scope void vmevme_isr();
scope void vmevme_menu();
scope void fill_nav_info(int);
