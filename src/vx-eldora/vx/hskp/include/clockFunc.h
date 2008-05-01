/*
 *      $Id$
 *
 *      Module:  clockFunc.h
 *      Original Author: Aditi Kapoor
 *      Copyrighted by the National Center for Atmospheric Research
 *     $Date$
 *
 *
 * Description: This include module declares the functions used for the
 *              Bancomm clock card in ELDORA. (replaced the TOD card)
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  2003/09/25  20:58:50  kapoor
 * added keyword "scope"
 *
 * Revision 1.1  2003/09/25  20:56:57  kapoor
 * Initial revision
 *
 * Revision 1.1  2003/09/23  19:39:28  kapoor
 * Initial revision
 *
 *
 */

#ifndef INCclkFunch
#define INCclkFunch


/* Bancomm clock card */

scope void clock_menu(void);         /* Clock card debug menu */

/* scope void get_time(void);           * Read time from the Clock card */
scope void get_time(char *,char *,char *,short *,short *,char *,char *,
                    char *);

scope char sync_irig(void);          /* Empty dummy function to replace TOD */
scope void get_year(void);           /* Read year from the Clock card */ 
scope void init_clock(short);        /* Initialize the Clock card */
scope void send_packet(char *);      /* Send command packet to the Clock card*/

scope void set_time(short,short,short,short,short,short); 
                                     /* Set Time, in TOD board format */ 
/*scope void set_time(int,int,int,int,int,int,char *);
                               * Encode packet to set time in the Clock card*/

scope void start_clock(void);        /* Load time into the Clock card */

#endif /* INC */


