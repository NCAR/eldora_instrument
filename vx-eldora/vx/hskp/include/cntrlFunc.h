/*

 *	$Id$
 *
 *	Module:	cntrlFunc.h	 
 *	Original Author: Reif Heck 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *             
 *             
 *              
 *              
 *             
 *             
 *             
 */

#ifndef INCcntrlFunch
#define INCcntrlFunch

/*************** FUNCTION PROTOTYPES *****************/

void mtr_menu(void);
float get_pos(void);
void set_vel(float);
void write_reg(void);
void write_data(void);
void read_data(void);
void read_rpm(void);
void init_motor(void);
void go_motor(void);
void stop_motor(void);
void goto_pos(float);

#endif /* INC */
