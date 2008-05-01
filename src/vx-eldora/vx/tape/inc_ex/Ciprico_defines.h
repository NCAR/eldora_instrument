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
 * Revision 1.3  1995/01/26  16:22:24  craig
 * *** empty log message ***
 *
 *
 * Revision 1.2  1992/10/17  21:38:07  reif
 * changed for global structures
 *
 * Revision 1.1  1992/08/14  18:08:58  reif
 * Initial revision
 *
 *
 *
 * description:
 *        
 */
#ifndef INCCiprico_definesh
#define INCCiprico_definesh

/* Exabyte command identifiers: */

#define TEST_UNIT_READY 0 
#define REWND           1 
#define REQUEST_SENSE   2   
#define READ_BLK_LIMS   3
#define TAPE_READ       4  
#define TAPE_WRITE      5 
#define WRITE_FILEMARK  6 
#define SPACE           7 
#define INQUIRY         8 
#define MODE_SELECT     9 
#define RESERVE_UNIT    10  
#define RELEASE_UNIT    11  
#define ERASE           12 
#define MODE_SENSE      13 
#define UNLOAD          14 
#define MEDIA_ACCESS    15
#define LOCATE          16 
#define VERIFY          17
#define READ_POSIT      18
#define SEND_DIAGNOSTICS 19
#define RCV_DIAGNOSTICS 20

/* Rimfire command identifiers: */

#define UNIT_OPS    21 
#define GEN_OPS     22 

/* EXABYTE COMMAND OPCODES */
#define TPUR 0x00
#define RWND 0x01
#define RQST_SNS 0x03
#define RD_BLK_LMTS 0x05
#define TP_RD 0x08
#define TP_WRT 0x0A
#define WRT_FLMK 0x10
#define SP 0x11
#define INQ 0x12
#define VFY 0x13
#define MD_SEL 0x15
#define RSV_UT 0x16
#define REL_UT 0x17
#define ERS 0x19
#define MD_SNS 0x1A
#define ULD 0x1B
#define MED_RMV 0x1E
#define LOC 0x2B
#define RD_POS 0x34
#define UOPS 0x08
#define GOPS 0x07
#define RCV_DIAGS 0x1C
#define SND_DIAGS 0x1D

/******************* MISC DEFINES ***************************/
#define CIP_BASE 0xFFFFEF00
#define CIP_ADDR_BUFF 0x00
#define CIP_CHAN_ATTN 0x08
#define CIP_BD_STAT 0x10
#define CIP_BD_RST 0x18
#define RSVD 0x00
#define LEOT 0x40
#define LBOT 0x01 
#define BLOCKS 0x00
#define EOFS 0x01
#define EOD  0x03
#define CB_AM 0x843D
#define AM 0x3D
#define INT_LVL 0x00
#define USER_ID 0x00

#define MAD_BASE 0x2000000
#define NUM_RECS 0x2000004
#define MAIL_BOX 0x2000008
#define DATA_RECS 0x2001800
#define MAX_TRIES 70
#define MAX_COUNT 1000
#define MAX_BLK_SZ 32768
#define GOP_THROTTLE 0xA0
#define GOP_OP_FLAGS 0x01
#define GOP_OWN_ID 0x07
#define GOP_TID 0xFF
#define GOP_RESV 0x00000000
#define UOP_DTO1 0xFD
#define UOP_DTO2 0xE8
#define UOP_UNIT_ID0 0x00 
#define UOP_UNIT_ID1 0x01 
#define UOP_TID 0xFF
#define UOP_VME 0x07F80000
#define UOP_CNT 0x00000002
#define MAX_MEM 4096
#define SCATTER_GATHER 0x69
#define HEADER 0x68
#define DEG_TO_RAD 0.01745
#define MAX_FILE_SIZE 95000000
#define ENDING_PERCENT 5.0
#define MAX_RAD_DSCR_BLK 29
#define MAX_NAV_DSCR_BLK 4
#define MAX_ADS_DSCR_BLK 4

/************* PARAMETER BLOCK STRUCTURE ***********/

struct param_block {
    /* Rimfire extended parameter block */
    unsigned int cmd_id;
    unsigned char resvd;
    unsigned char flags;
    unsigned char addr_mod;
    unsigned char targ_id;
    unsigned int vme_addr;
    unsigned int xfer_count;
    unsigned char scsi_blk[12];  /* SCSI command block  */
    unsigned short res1;       /* reserved        */
    unsigned char int_level;    /* interrupt information*/
    unsigned char user_id;
    unsigned int res2;      /* reserved area 2 */
    unsigned int  status_cmd_id; /* make space for status return */
    unsigned char res3;
    unsigned char scsi_status;
    unsigned char error;
    unsigned char stat_flags;    
    unsigned char class_code;
    unsigned char segment;
    unsigned char scsi_flags;
    unsigned char info3;
    unsigned char info4;
    unsigned char info5;
    unsigned char info6;
    unsigned char ex_length;
    };
typedef struct param_block PARMBLK;

struct mode_select{ /* mode_select parameter block */
    unsigned char res1;
    unsigned char res2;
    unsigned char buf_mode;  /* buffer mode,bits 4-6,0=no buf,1=buf*/
    unsigned char blk_desc_len; /* block descriptor length */
    unsigned char density;
    unsigned char num_blks_msb;
    unsigned char num_blks_mid;
    unsigned char num_blks_lsb;
    unsigned char res3;
    unsigned char blk_len_msb;
    unsigned char blk_len_mid;
    unsigned char blk_len_lsb;
    unsigned char vend_parms;
    unsigned char res4;
    unsigned char motion_thr;
    unsigned char recon_thr;
    unsigned char gap_thr;
    }; 

struct mode_sense{ /* mode sense parameter block */
    unsigned char sense_data_len;
    unsigned char medium;
    unsigned char WP_buf_mode;  /* buffer mode,bits 4-6,0=no buf,1=buf*/
    unsigned char blk_desc_len; /* block descriptor length */
    unsigned char density;
    unsigned char num_blks_msb;
    unsigned char num_blks_mid;
    unsigned char num_blks_lsb;
    unsigned char res3;
    unsigned char blk_len_msb;
    unsigned char blk_len_mid;
    unsigned char blk_len_lsb;
    unsigned char vend_parms;
    unsigned char res4;
    unsigned char motion_thr;
    unsigned char recon_thr;
    unsigned char gap_thr;
    };

struct request_sense{ /* request sense parameter structure */
     /* see the Exabyte manual on page 149 */
    unsigned char vld_err_code; 
    unsigned char seg_num;
    unsigned char sense_key;
    unsigned char info_msb;
    unsigned char info_mid_high;
    unsigned char info_mid_low;
    unsigned char info_lsb;
    unsigned char addit_sense_len;
    unsigned char res1;
    unsigned char res2;
    unsigned char res3;
    unsigned char undr_ovr_cntr;
    unsigned char addit_sense_code;
    unsigned char addit_sense_qual;
    unsigned char res5;
    unsigned char res6;
    unsigned char err_ctr_msb;
    unsigned char err_ctr_mid;
    unsigned char err_ctr_lsb;
    unsigned char stat1;
    unsigned char stat2;
    unsigned char stat3;
    unsigned char res7;
    unsigned char rem_tape_msb;
    unsigned char rem_tape_mid;
    unsigned char rem_tape_lsb;
    unsigned char trkng_retry_cntr;
    unsigned char rd_wrt_retry_cntr;
    unsigned char fault_code;
};
struct rad_scatter_gather {
    unsigned int next_rad_sg_addr;
    unsigned int am_and_dat_len1;
    unsigned int dat_addr1;
    unsigned int am_and_dat_len2;
    unsigned int dat_addr2;
    unsigned int am_and_dat_len3;
    unsigned int dat_addr3;
    unsigned int am_and_dat_len4;
    unsigned int dat_addr4;
    unsigned int am_and_dat_len5;
    unsigned int dat_addr5;
    unsigned int am_and_dat_len6;
    unsigned int dat_addr6;
    unsigned int am_and_dat_len7;
    unsigned int dat_addr7;
    unsigned int am_and_dat_len8;
    unsigned int dat_addr8;
};
struct nav_scatter_gather {
    unsigned int next_nav_sg_addr;
    unsigned int am_and_dat_len1;
    unsigned int dat_addr1;
    unsigned int am_and_dat_len2;
    unsigned int dat_addr2;
    unsigned int am_and_dat_len3;
    unsigned int dat_addr3;
    unsigned int am_and_dat_len4;
    unsigned int dat_addr4;
    unsigned int am_and_dat_len5;
    unsigned int dat_addr5;
    unsigned int am_and_dat_len6;
    unsigned int dat_addr6;
    unsigned int am_and_dat_len7;
    unsigned int dat_addr7;
    unsigned int am_and_dat_len8;
    unsigned int dat_addr8;
};
struct ads_scatter_gather {
    unsigned int next_ads_sg_addr;
    unsigned int am_and_dat_len1;
    unsigned int dat_addr1;
    unsigned int am_and_dat_len2;
    unsigned int dat_addr2;
    unsigned int am_and_dat_len3;
    unsigned int dat_addr3;
    unsigned int am_and_dat_len4;
    unsigned int dat_addr4;
    unsigned int am_and_dat_len5;
    unsigned int dat_addr5;
    unsigned int am_and_dat_len6;
    unsigned int dat_addr6;
    unsigned int am_and_dat_len7;
    unsigned int dat_addr7;
    unsigned int am_and_dat_len8;
    unsigned int dat_addr8;
};

#endif /* INC */




