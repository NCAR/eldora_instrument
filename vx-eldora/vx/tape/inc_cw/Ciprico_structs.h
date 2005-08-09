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
 * Revision 1.3  1996/02/09  17:45:16  craig
 * *** empty log message ***
 *
 * Revision 1.2  1994/11/14  17:58:18  craig
 * *** empty log message ***
 *
 * Revision 1.1  1994/01/06  21:39:12  craig
 * Initial revision
 *
 *
 * description:
 *        
 */
#ifndef INCCiprico_structsh
#define INCCiprico_structsh
#ifndef scope
#define scope extern
#endif

/********* GLOBAL STRUCTURE OF STRUCTURES ********/

struct data_beam{
    struct ray_i ri;
    struct platform_i pi;
    struct field_parameter_data fpd;
};

/********* GLOBAL TRANSFER ARRAYS ****************/

scope unsigned int rad_xfer_len[8];
scope unsigned int rad_xfer_addr[8];
scope unsigned int nav_xfer_len[8];
scope unsigned int nav_xfer_addr[8];
scope unsigned int ads_xfer_len[8];
scope unsigned int ads_xfer_addr[8];

/*********** GLOBAL TAPE HEADER ARRAY **************/

scope unsigned int tapeHdr[4000];

/************* PARAMETER BLOCK STRUCTURE ***********/

scope struct param_block parmblk[LAST_CIP_CMD + 1]; 
/* ARRAY OF PARAMETER BLOCKS */

scope struct mode_struct mod_sel; 

scope struct mode_struct  mod_sen;

scope struct request_sense  rqst_sen;

scope struct scatter_gather rad_sg[MAX_RAD_DSCR_BLK + 2];
scope struct scatter_gather nav_sg[MAX_NAV_DSCR_BLK + 2];
scope struct scatter_gather ads_sg[MAX_ADS_DSCR_BLK + 2];

scope struct log_pg log_page;
scope struct supported_pgs suprt_pgs;

/*********** GLOBAL STATUS FLAGS **********/

scope unsigned long write_attempts;
scope short vol_num;
scope short tape_num;
scope unsigned int hdrsz;

#endif /*INC*/







