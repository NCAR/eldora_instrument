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
 * Revision 1.1  1992/08/14  21:36:51  reif
 * Initial revision
 *
 *
 * description: Function templates to go with the motor controller
 *              software
 *             
 */

#ifndef INCcntrlFunch
#define INCcntrlFunch

/*************** FUNCTION PROTOTYPES *****************/

void motor_menu(void);
float get_pos(void);
void set_vel(float);
void write_reg(void);
void read_reg(void);
float read_rpm(void);
void init_motor(void);
void go_motor(void);
void stop_motor(void);
void goto_pos(float);

#endif /* INC */
