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

scope unsigned int tapeHdr[2048];

/************* PARAMETER BLOCK STRUCTURE ***********/

scope struct param_block parmblk[23]; /* ARRAY OF PARAMETER BLOCKS */

scope struct mode_select mod_sel; 

scope struct mode_sense  mod_sen;

scope struct request_sense  rqst_sen;

scope struct rad_scatter_gather rad_sg[MAX_RAD_DSCR_BLK + 2];
scope struct nav_scatter_gather nav_sg[MAX_NAV_DSCR_BLK + 2];
scope struct ads_scatter_gather ads_sg[MAX_ADS_DSCR_BLK + 2];

/*********** GLOBAL STATUS FLAGS **********/

scope unsigned long write_attempts;
scope short vol_num;
scope short tape_num;
scope unsigned int hdrsz;
scope int hr;
scope int min;
scope int sec;
scope long jday;

#endif /*INC*/







