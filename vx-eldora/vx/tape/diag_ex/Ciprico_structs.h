/*
 *	$Id$
 *
 *	Module:		 
 *	Original Author: 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.5  1992/11/04  04:55:13  reif
 * *** empty log message ***
 *
 * Revision 1.4  1992/10/30  23:59:26  reif
 * *** empty log message ***
 *
 * Revision 1.3  1992/10/17  21:38:07  reif
 * changed for global structures
 *
 * Revision 1.2  1992/10/15  17:53:28  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  18:09:14  reif
 * Initial revision
 *
 *
 *
 * description:
 *        
 */

/********* GLOBAL TRANSFER ARRAYS ****************/

unsigned int sg_xfer_len[8];
unsigned int sg_xfer_addr[8];
int scsi_id[4];

/************* PARAMETER BLOCK STRUCTURE ***********/

struct param_block parmblk[23]; /* ARRAY OF PARAMETER BLOCKS */

struct mode_select mod_sel; 

struct mode_sense  mod_sen;

struct request_sense  rqst_sen;

struct scatter_gather sg[10];

struct receive_diags rcv_results;








