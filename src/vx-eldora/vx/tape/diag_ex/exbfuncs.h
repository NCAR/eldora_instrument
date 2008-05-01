
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
 * Revision 1.4  1992/10/17  21:38:07  reif
 * changed navgthr,adsgthr, and radargthr to use global tranfer arrays
 *
 * Revision 1.3  1992/10/15  17:53:28  reif
 * *** empty log message ***
 *
 * Revision 1.2  1992/08/21  22:17:41  reif
 * *** empty log message ***
 *
 * Revision 1.1  1992/08/14  18:09:44  reif
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCexbfuncsh
#define INCexbfuncsh



/************** FUNCTION PROTOTYPES *******************/

void cip_init(void);
int write_tape(unsigned int *,unsigned int,unsigned char,unsigned char);
int read_tape(unsigned int *,unsigned int,unsigned char,unsigned char);
char tst_unt_rdy(unsigned char);
void space_tape(long,char,char,char);
void erase_tape(void);
void rd_blk_lmts(void);
void cip_cmds(unsigned int, unsigned char, unsigned char);
unsigned char exb_cmds(unsigned int, unsigned char, unsigned char);
void vfy_tape(void);
void inquire(void);
void locate(void);
void rsrv_unit(void);
void rels_unit(void);
void read_posit(void);
void prvnt_allow_tape_rmv(void);
void select_opt(void);
void parm_blk_init(void);
void chan_attn(int);
void print_stat(int,unsigned int);
void md_sel_init(void);
void md_sns_init(void);
void print_md_sns(void);
void rqst_sns_init(void);
void print_rqst_sns(void);
void sctr_gthr(int,int);
int fill_sg(unsigned int *,unsigned int);
int orig_amnt(void);
void tp_cpy(unsigned int *,unsigned int);
void send_diags(char,short,char,char);
void print_results(void);

#endif /* INC */







