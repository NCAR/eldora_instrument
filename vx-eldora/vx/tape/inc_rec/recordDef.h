/*
 *	$Id$
 *
 *	Module:	recordDef.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description: Contains definitions specific to the ELDORA tape recording
 *              software
 *        
 */
#ifndef INCrecordDefh
#define INCrecordDefh

/******************* MISC DEFINES ***************************/
#define MAD_BASE 0x2000000
#define NUM_RECS 0x2000004
#define MAIL_BOX 0x2000008
#define DATA_RECS 0x2001800

#define TOTAL_TAPE_SIZE 10000.0   /* In mega bytes */
#define MAX_MEM 4096
#define DEG_TO_RAD 0.01745
#define MAX_FILE_SIZE 95000000
#define ENDING_PERCENT 5.0
#define MAX_RAD_DSCR_BLK 29
#define MAX_NAV_DSCR_BLK 4
#define MAX_ADS_DSCR_BLK 4

/********* GLOBAL STRUCTURE OF STRUCTURES ********/

struct data_beam{
    struct ray_i ri;
    struct platform_i pi;
    struct field_parameter_data fpd;
};


#endif /* INC */




