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
 * Revision 1.1  2003/09/23  19:39:28  kapoor
 * Initial revision
 *
 *
 */

#ifndef INCclkFunch
#define INCclkFunch


/* Bancomm clock card */
void clock_menu(void);         /* Clock card debug menu */
void get_time(void);           /* Read time from the Clock card */
void get_year(void);           /* Read year from the Clock card */
void init_clock(void);         /* Initialize the Clock card */
void send_packet(char *);      /* Send command packet to the Clock card */
void set_time(int,int,int,int,int,int,char *);
                               /* Encode packet to set time in the Clock card*/
void start_clock(char *);        /* Load time into the Clock card */

#endif /* INC */


