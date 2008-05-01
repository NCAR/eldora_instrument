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
 * Revision 1.1  1991/12/09  18:25:15  eric
 * Initial revision
 *
 *
 * description:  Contains global definitions for the Multi Crate
 *               Pipeline (MCPL)
 *
 */

#ifndef INCmcpl_def
#define INCmcpl_def
#include "ELDRP7.h"
#include "raw_data.h"
#define HSKPR
/* The basic Mailbox, address and data structure */

struct MAD {
long sync_word;
long num_before_repeat;
short mailboxes[3000];
short mad_unused[68];
long  addresses[3000];
};

#define STARTOF_MAD_DATA 0x4700   /* Offset to first data block in MAD */

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

/* The Basic Raw DATA field */

struct RAW_D {
  RAW_DATA rd;
  float data[4096];
};

/* The Basic data ray */
struct DATA_RAY {
  struct ray_i rayinfo;
  struct platform_i pltfrminfo;
  struct field_parameter_data fielddata;
  unsigned long data[32000]; /* was [4096] */
};


/* Memory setup */

/* #define VMEMEM_BASE         0x6000000  for 68040 DPRAM */
#define VMEMEM_BASE         0xC00000
#define DATA_RAY_BASE       0x100
#define DATA_RAY_OFFSET     0x8000
#define DATA_NAV_BASE       0xD8100
#define DATA_NAV_OFFSET     0x8000
#define DATA_ADS_BASE       0xE8100
#define DATA_ADS_OFFSET     0x8000

#ifdef HSKPR
#define MAILBOX_OFFSET      0x100000
#define INIT_OFFSET         0x100004
#define TPB_TO_PROC_OFFSET  0x100024
#define RECEIVE_GMAD_OFFSET 0x100044
#define RECEIVE_DATA_OFFSET 0x100064
#define GMAD_TO_PROC_OFFSET 0x100084
#define BCAST_TPB_OFFSET    0x1000A4
#define GMAD_OFFSET         0x1000C4
#endif

#ifndef HSKPR
#define MAILBOX_OFFSET      0x0
#define INIT_OFFSET         0x4
#define TPB_TO_PROC_OFFSET  0x24
#define RECEIVE_GMAD_OFFSET 0x44
#define RECEIVE_DATA_OFFSET 0x64
#define GMAD_TO_PROC_OFFSET 0x84
#define BCAST_TPB_OFFSET    0xA4
#define GMAD_OFFSET         0xC4
#define RAY_OFFSET          0x47a4
#endif 
/* Base address abs register location */

#define MCPL_BASE   0xE000 + SHORT_BASE
#define CONTROL_REG 0x00
#define STATUS_REG  0x02
#define FIFO_REG    0x04
#define START_REG   0x06

/* Control words */

#define RESET 0x0010
#define HOLD 0x0012
#define READ_INIT_TPB 0x0011
#define TRIGGER 0x0013

/* Definitions for specific processors */
#define FORE_RADAR_NODE   4  /* Node number for the fore radar processor */
#define AFT_RADAR_NODE    2  /* Node number for the aft radar processor */
#define DISPLAY_NODE_1    5  /* Node number for the display processor #1 */
#define DISPLAY_NODE_2    6  /* Node number for the display processor #2 */
#define RECORD_NODE       7  /* Node number for the data recording processor */
#define GRID_NODE         3  /* Node number for the data gridding processor */

/* Number of MAD structures in each processor, zero means no processor */
#define NUM_STRUCT_DIS1 1  /* Number of mailbox,address data structures MAD */
#define NUM_STRUCT_DIS2 0  /* Number of MAD structures display processor #2 */
#define NUM_STRUCT_REC  1  /* Number of MAD structures recording processor */
#define NUM_STRUCT_GRID 0  /* Number of MAD structures data gridding proc */

/* Completion status to use for each processor */
#define C_STATUS_DIS1 0x08F7  /* Interrupt level 3, interrupt vector #243
                                 a32,D16 mailbox */
#define C_STATUS_DIS2 0x08F7  /* Interrupt level 3, interrupt vector #243
                                 a32,D16 mailbox */
#define C_STATUS_REC  0x0006  /* No interrupts, a32,d16 mailbox */
#define C_STATUS_GRID 0x0006  /* No interrupts, a32,d16 mailbox */

#define MADBASE_DIS1_0 0x10000000 /* Base address MAD #0 Display proc #1 */
#define MADBASE_DIS1_1 0x2200000  /* Base address MAD #1 Display proc #1 */
#define MADBASE_DIS2_0 0x10000000 /* Base address MAD #0 Display proc #2 */
#define MADBASE_DIS2_1 0x2200000  /* Base address MAD #1 Display proc #2 */
#define MADBASE_REC_0  0x2000000  /* Base address MAD #0 Data recording proc */
#define MADBASE_REC_1  0x0        /* Base address MAD #1 Data Recording proc */
#define MADBASE_GRID_0 0x0        /* Base address MAD #0 Data Gridding proc */
#define MADBASE_GRID_1 0x0        /* Base address MAD #1 Data Gridding proc */

#define DIS1_MEM_SIZE_0 0x200000 /* Memory size for MAD #0 Display proc #1 */
#define DIS1_MEM_SIZE_1 0x200000 /* Memory size for MAD #1 Display proc #1 */
#define DIS2_MEM_SIZE_0 0x200000 /* Memory size for MAD #0 Display proc #2 */
#define DIS2_MEM_SIZE_1 0x200000 /* Memory size for MAD #1 Display proc #2 */
#define REC_MEM_SIZE_0  0x1000000 /* Memory size for MAD #0 Data recording pro*/
#define REC_MEM_SIZE_1  0x0      /* Memory size for MAD #1 Data Recording pro*/
#define GRID_MEM_SIZE_0 0x0      /* Memory size for MAD #0 Data Gridding proc*/
#define GRID_MEM_SIZE_1 0x0      /* Memory size for MAD #1 Data Gridding proc*/

#endif /* INCmcpldef */







