
/*
 *	$id: recordFuncs.h,v 1.4 1996/02/09 17:45:29 craig Exp craig $
 *
 *	Module:	recordFuncs.h	 
 *	Original Author: Craig Walther
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:  Function prototypes for all of the ELDORA specific
 *               tape recording functions 
 *        
 */
#ifndef INCrecordFuncsh
#define INCrecordFuncsh



/************** FUNCTION PROTOTYPES *******************/
void flag_check(char,char,char,char);
unsigned int nav_length(unsigned int *);
unsigned int ads_length(unsigned int *);
unsigned int tape_header(void);
void drv_sel(unsigned int *,unsigned int,unsigned char);
void cal_sum(void);
int reduce_data(short,unsigned int *);
void reduce_init(void);
long data_ray_length(void);
void mail_box(void);
void startup(void);

#endif /* INC */
