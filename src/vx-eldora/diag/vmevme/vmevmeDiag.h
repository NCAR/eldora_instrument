/*
 *	$Id$
 *
 *	Module:	vmevmeDiag.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *        
 */

/* Pointers and global variables for Vme to Vme diagnostics */

scope int int_count;

scope char *local_status_reg;
scope char *remote_status_reg;
scope char *remote_command_reg;
scope char *local_command_reg;
scope char *remote_address_page_reg;
scope char *remote_address_mod_reg;
scope char *vme_iack_read;
scope long fore_int_cnt;
scope long aft_int_cnt;


scope void vme_isr(void);
