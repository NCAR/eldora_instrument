/*
 *	$Id$
 *
 *	Module: tapeGbl.h		 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1996/10/02  17:40:42  craig
 * Initial revision
 *
 *
 * description:  Contains global variables used with the Exabyte tape functions
 *        
 */

#ifndef tapeGbl_h
#define tapeGbl_h

#ifndef scope
#define scope extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/************* PARAMETER BLOCK STRUCTURE ***********/

scope int tape_vme_offset; /* offset between where the CPU sees the tape
                              structures and where the Ciprico does */ 

scope struct param_block *parmblk[LAST_CIP_CMD + 1];

scope struct mode_struct *mod_sel; 

scope struct mode_struct  *mod_sen;

scope struct request_sense  *rqst_sen;

scope struct log_pg *log_page;

scope struct supported_pgs *suprt_pgs;

scope struct scatter_gather *sg[NUM_SG_STRUCTS][MAX_SG_BLK + 2];
scope int sg_blk_cnt[NUM_SG_STRUCTS];  /* scatter gather block counter */
scope int sg_add_cnt[NUM_SG_STRUCTS];  /* scatter gather address counter */

/********** define all of the tape functions here ********/

scope void dlt_cmds(unsigned int, unsigned char, int);
scope void chan_attn(int);
scope void cip_cmds(unsigned int,unsigned char, unsigned char, int);
scope void cip_init();
scope void drive_init(unsigned char);
scope int fill_sg(unsigned int *, int,int,int);
scope void initialize_tape(int,int);
scope void md_sel_init();
scope void md_sns_init();
scope void parm_blk_init();
scope void print_md_sns();
scope void print_results();
scope void print_rqst_sns();
scope void print_stat(int,unsigned int);
scope int read_tape(unsigned int *,unsigned int,unsigned char,
		    unsigned char, int, int);
scope void rqst_sns_init();
scope void send_diags(char,short,char,char);
scope void sg_init(int);
scope void space_tape(long,char,char,char,int);
scope int tape_cntrl_len(int);
scope void tape_menu();
scope void tp_cpy(unsigned int *, unsigned int);
scope char tst_unt_rdy(unsigned char, int);
scope char write_tape(unsigned int *, unsigned int, unsigned char,
		      unsigned char, int, int);
scope int tape_remain(unsigned char);

/*********** declare some general purpose parameters here ********/

scope unsigned char scsi_id[4];
scope int starting_amt[4];

#ifdef __cplusplus
};
#endif

#endif  /* tapeGbl_h */



