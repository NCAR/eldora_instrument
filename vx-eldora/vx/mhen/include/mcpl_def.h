/*
 *	$Id$
 *
 *	Module: mcpl_def.h 		 
 *	Original Author: Craig Walther 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date  $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:  Contains global definitions for the Multi Crate
 *               Pipeline (MCPL)
 *
 */
/* The basic Mailbox, address and data structure */

#ifndef INCmcpl_def
#define INCmcpl_def

struct MAD {
long sync_word;
long num_before_repeat;
short mailboxes[1000];
short mad_unused[20];
long  addresses[1000];
};

/* The basic Transfer Parameter Block Structure */

struct TPB {
unsigned long  source_start_address;
unsigned long  sink_start_address;
unsigned short source_attribute;
unsigned short sink_attribute;
unsigned long  transfer_length;
unsigned short completion_status;
unsigned short nack_count;
unsigned long  link_address;
unsigned short link_attribute;
unsigned short mcpl_control_word;
unsigned long  mailbox_address;
};

/* Base address abs register location */

#define MCPL_BASE   0xFFFFE000
#define CONTROL_REG 0x00
#define STATUS_REG  0x02
#define FIFO_REG    0x04
#define START_REG   0x06

/* Control words */

#define RESET 0x0000
#define HOLD 0x0002
#define READ_INIT_TPB 0x0001
#define TRIGGER 0x0003

/* Definitions for specific processors */
#define RADAR_NODE   6       /* Node number for the radar processor */
#define DISPLAY_NODE 3       /* Node number for the display processor */
#define RECORD_NODE  4       /* Node number for the data recording processor */
#define GRID_NODE    5       /* Node number for the data gridding processor */

/* Number of MAD structures in each processor, zero means no processor */
#define NUM_STRUCT_DIS 1    /* Number of mailbox,address data structures MAD */
#define NUM_STRUCT_REC 1    /* Number of MAD structures recording processor */
#define NUM_STRUCT_GRID 0   /* Number of MAD structures data gridding proc */

/* Completion status to use for each processor */
#define C_STATUS_DIS  0x08F7  /* Interrupt level 3, interrupt vector #243
                                 a32,D16 mailbox */
#define C_STATUS_REC  0x0006  /* No interrupts, a32,d16 mailbox */
#define C_STATUS_GRID 0x0006  /* No interrupts, a32,d16 mailbox */

#define MADBASE_DIS_0 0x2000000  /* Base address MAD #0 Display proc */
#define MADBASE_DIS_1 0x2200000  /* Base address MAD #1 Display proc */
#define MADBASE_REC_0 0x2000000  /* Base address MAD #0 Data recording proc */
#define MADBASE_REC_1 0x0        /* Base address MAD #1 Data Recording proc */
#define MADBASE_GRID_0 0x0       /* Base address MAD #0 Data Gridding proc */
#define MADBASE_GRID_1 0x0       /* Base address MAD #1 Data Gridding proc */

#define DIS_MEM_SIZE_0  0x200000  /* Memory size for MAD #0 Display proc */
#define DIS_MEM_SIZE_1  0x200000  /* Memory size for MAD #1 Display proc */
#define REC_MEM_SIZE_0 0x200000   /* Memory size for MAD #0 Data recording proc */
#define REC_MEM_SIZE_1 0x0     /* Memory size for MAD #1 Data Recording proc */
#define GRID_MEM_SIZE_0 0x0     /* Memory size for MAD #0 Data Gridding proc */
#define GRID_MEM_SIZE_1 0x0     /* Memory size for MAD #1 Data Gridding proc */

#endif /* INCmcpldef */










