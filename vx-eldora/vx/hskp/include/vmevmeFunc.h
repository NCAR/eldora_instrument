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
 * Revision 1.1  1992/08/19  19:35:34  craig
 * Initial revision
 *
 *        
 */

/* Functions used with the vme to vme interfaces */

scope void init_vmevme(void);
scope void stop_vmevme(void);
scope void start_vmevme(void);
scope void vmevme_isr(void);
scope void fill_nav_info(int);
scope void fill_platform(long);
scope void dc_removal(void);
scope void vmevme_menu(void);



