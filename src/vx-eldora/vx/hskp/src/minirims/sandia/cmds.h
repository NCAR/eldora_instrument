/*+ DEC/CMS REPLACEMENT HISTORY, Element CMDS.H+*/
/*+ *4    28-JUL-1988 09:39:02 JRFELLE ""+*/
/*+ *3    23-MAY-1988 12:18:28 ECREEL "rwed prot set by eec"+*/
/*+ *2    20-MAY-1988 15:14:23 ECREEL "no mods -- used only for compiling purposes by eec"+*/
/*+ *1     7-MAR-1988 15:45:10 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element CMDS.H+*/
/****************************************************************
* cmds.h							*
*****************************************************************

Description:  this file defines the rims commands needed for each
	      mode of operation and defines the mode number for
	      each navigator operational mode.

inputs:	      none
outputs:      none.

notes:	      none

called routines:  none

*****************************************************************/

/* Last Modified:  8/18/87    BY:  E.J. Nava, 9131		*/

/* define navigator operational modes	*/

#define NAV 0x00
#define HOLD 0x01
#define INIT 0x02
#define TEST 0x03
#define ALIGN0 0x08
#define ALIGN90 0x09

