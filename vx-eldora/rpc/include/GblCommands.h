/*
 *	$Id$
 *
 *	Module:		 GblCommands.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1996/10/09 20:09:10  eric
 * changed command from ALIGN to ALIGN_MINI to avoid
 * confusion with housekeeper ALIGN.
 *
 * Revision 1.2  1996/10/09  19:48:44  eric
 * added ALIGN command to list.
 *
 * Revision 1.1  1996/10/09  19:43:42  thor
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCGblCommandsh
#define INCGblCommandsh

#define START   0x1
#define STOP    0x2
#define INIT    0x4
#define REBOOT  0x8
#define RELOAD  0x80000000
#define HEADER  0xf0000000
#define SEND_STATUS  0xffffffff

#define ALIGN_MINI   0x10

#endif /* INCGblCommandsh */
