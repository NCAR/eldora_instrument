/*
 *	$Id$
 *
 *	Module:	tapeDef.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1996/11/15  22:08:25  craig
 * *** empty log message ***
 *
 * Revision 1.1  1996/10/02  17:40:39  craig
 * Initial revision
 *
 *
 * description: Defines all of the parameters and structures associated 
 *              with the Exabyte tape writing software
 *        
 */
#ifndef INCtapeDefh
#define INCtapeDefh

/* DLT command identifiers: */

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
#define LOAD            10 
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
#define LOG_SELECT      21
#define LOG_SENSE       22
#define LOG_SENSE_SPGS  23
#define LAST_CMD_ID     23

/* Rimfire command identifiers: */
#define UNIT_OPS    24 
#define GEN_OPS     25
#define LAST_CIP_CMD 25 


/* SCSI COMMAND OPCODES */

#define TPUR 0x00        /* Test Unit Ready */
#define RWND 0x01        /* Rewind */
#define RQST_SNS 0x03    /* Request sense */
#define RD_BLK_LMTS 0x05 /* Read Bloc k Limits */
#define TP_RD 0x08       /* Tape read */
#define TP_WRT 0x0A      /* Tape write */
#define WRT_FLMK 0x10    /* Write a Filemark */
#define SP 0x11          /* Space */
#define INQ 0x12         /* Inquiry */
#define VFY 0x13         /* Verify */
#define MD_SEL 0x15      /* Mode Select */
#define RSV_UT 0x16      /* Reserve Unit */
#define REL_UT 0x17      /* release Unit */
#define ERS 0x19         /* Erase */
#define MD_SNS 0x1A      /* Mode Sense */
#define ULD 0x1B         /* Load - Unload */
#define LD_ULD 0x1B      /* Load - Unload */
#define MED_RMV 0x1E     /* Prevent/allow medium removal */
#define LOC 0x2B         /* Locate */
#define RD_POS 0x34      /* Read position */
#define RCV_DIAGS 0x1C   /* Receive Diagnostics */
#define SND_DIAGS 0x1D   /* Send diagnostics */
#define LOG_SEL   0x4C   /* Log Select */
#define LOG_SEN   0x4D   /* Log Sence */

/* Ciprico command opcodes */

#define UOPS 0x08        /* Unit ops */
#define GOPS 0x07        /* General ops */

/******************* MISC DEFINES ***************************/

#define CIP_BASE 0xEF00
#ifndef SHORT_BASE
#define SHORT_BASE 0xFFFF0000
#endif
#define CIP_ADDR_BUFF 0x00
#define CIP_CHAN_ATTN 0x08
#define CIP_BD_STAT 0x10
#define CIP_BD_RST 0x18
#define CIPRICO_ERR 0x20
#define RSVD 0x00
#define LEOT 0x40
#define LBOT 0x01 
#define BLOCKS 0x00
#define EOFS 0x01
#define EOD  0x03
#define INT_LVL 0x00
#define USER_ID 0x00

#define MAX_TRIES 70
#define MAX_COUNT 1000
#define MAX_BLK_SZ 32768
#define MAX_SG_BLK 20
#define NUM_SG_STRUCTS 6
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

#define SCATTER_GATHER 0x69
#define BLOCKED 0x68
#define CONTROL_BYTE 0x8400;

/* Address modifiers - must be used to match your system */
#define STD_AM 0x003D  /* Standard space address modifier */
#define EXT_AM 0x000D  /* Extended space address modifier */

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

struct mode_struct{ /* mode_select parameter block */
    unsigned char res1;
    unsigned char res2;
    unsigned char buf_mode_spd;  /* buffer mode and speed */
    unsigned char blk_desc_len; /* block descriptor length */
    unsigned char density;
    unsigned char num_blks_msb;
    unsigned char num_blks_mid;
    unsigned char num_blks_lsb;
    unsigned char res3;
    unsigned char blk_len_msb;
    unsigned char blk_len_mid;
    unsigned char blk_len_lsb;
    unsigned char page_code;   /* Only page is device configuration */
    unsigned char page_len;
    unsigned char CAP_CAF_fmt;
    unsigned char active_par;
    unsigned char write_full_ratio;
    unsigned char read_full_ratio;
    unsigned char write_dly_msb;
    unsigned char write_dly_lsb;
    unsigned char DBR_other_stuff;
    unsigned char gap_size;
    unsigned char EOD_EGG_SEW;
    unsigned char buff_size_msb;
    unsigned char buff_size_mid;
    unsigned char buff_size_lsb;
    unsigned char data_compress;
    unsigned char res4;
    }; 

struct request_sense{ /* request sense parameter structure */

    unsigned char vld_err_code; 
    unsigned char seg_num;
    unsigned char sense_key;
    unsigned char info_msb;
    unsigned char info_mid_high;
    unsigned char info_mid_low;
    unsigned char info_lsb;
    unsigned char addit_sense_len;
    unsigned char cmnd_spcfc_msb;
    unsigned char cmnd_spcfc_mid_high;
    unsigned char cmnd_spcfc_mid_low;
    unsigned char cmnd_spcfc_lsb;
    unsigned char addit_sense_code;
    unsigned char addit_sense_qual;
    unsigned char sub_assembly_code;
    unsigned char SKSV_CD_BPV_bitpntr;
    unsigned char fld_pntr_msb;
    unsigned char fld_pntr_lsb;
    unsigned char internal_stat;
};

struct length_and_address {
    unsigned int am_and_length;
    unsigned int address;
};


struct scatter_gather {
    unsigned int next_sg_blk_add;
    struct length_and_address len_add[8];
};

struct receive_diags{
    unsigned char page_code;
    char resvd;
    char page_length_msb;
    char page_length_lsb;
    char diag_params1;
    char diag_params2;
    char diag_params3;
    char diag_params4;
    char diag_params5;
    char diag_params6;
    char diag_params7;
    char diag_params8;
};

struct log_page_head{
    unsigned char page_code;
    char resvd1;
    char page_length_msb;
    char page_length_lsb;
};

struct log_param_short{
    char code_msb;
    char code_lsb;
    char misc;
    char length;
    char pvalue_msb;
    char pvalue_midh;
    char pvalue_midl;
    char pvalue_lsb;
};

struct log_param_long{
    char code_msb;
    char code_lsb;
    char misc;
    char length;
    char pvalue0;
    char pvalue1;
    char pvalue2;
    char pvalue3;
    char pvalue4;
    char pvalue5;
    char pvalue6;
    char pvalue7;
};

struct log_pg{
    struct log_page_head head;
    struct log_param_short sub_delay;
    struct log_param_short pos_delay;
    struct log_param_short total_rewrites;
    struct log_param_short total_errors_cor;
    struct log_param_short total_proc;
    struct log_param_long  total_bytes;
    struct log_param_short total_errors_uncor;
    struct log_param_short vendor_unique;
};

struct supported_pgs{
    char page_code;
    char reserved;
    char length_msb;
    char length_lsb;
    char page1;
    char page2;
    char page3;
    char page4;
    char page5;
};

#endif /* INC */





