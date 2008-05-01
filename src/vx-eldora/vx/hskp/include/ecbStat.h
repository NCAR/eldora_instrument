/*
 *	$Id$
 *
 *	Module:	ecbStat.h	 
 *	Original Author: Shawn B. Copeland
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.3  1993/09/20  17:05:17  shawn
 * *** empty log message ***
 *
 * Revision 1.2  1992/06/24  20:47:00  shawn
 * A version for safety's sake, current as of 6/24/92.
 *
 * Revision 1.1  1992/05/29  17:55:11  shawn
 * Initial revision
 *
 * description: Contains global status structures for ECB status.
 *        
 */

struct ecblastcmd    /* ID OF LAST COMMAND COMPLETED */
{
    unsigned char ecbadr;      /* ECB address of slave last command sent to */
    unsigned char comID;       /* Command ID of last command completed */
};

struct ecbool        /* DDS OUT-OF-LOCK DATA */
{
    unsigned char ecbadr;      /* ECB address of slave ool data is from */
    unsigned char newdata;     /* newdata flag: 1 = new data, 0 = old data */
    unsigned char oolbits;     /* byte of out-of-lock flags */
};

struct ecbtempi      /* TEMPERATURE DATA, SCALED INTEGER */
{
    unsigned char ecbadr;      /* ECB address of slave temp data is from */
    unsigned char newdata;     /* newdata flag: 1 = new data, 0 = old data */
    short onbtemp;             /* on-board temperature value, scaled int */
    short offbtemp;            /* off-board temperature value, scaled int */
};

struct ecbtempr      /* TEMPERATURE DATA, RAW COUNTS */
{
    unsigned char ecbadr;      /* ECB address of slave temp data is from */
    unsigned char newdata;     /* newdata flag: 1 = new data, 0 = old data */
    unsigned char onbtemp;     /* on-board temperature value, raw counts */
    unsigned char offbtemp;    /* off-board temperature value, raw counts */
};

struct ecbslvstat           /* ECB SLAVE BUS STATUS */
{
    unsigned char ecbadr;      /* ECB address of slave temp data is from */
    unsigned char newdata;     /* newdata flag: 1 = new data, 0 = old data */
    unsigned short syncerrs;   /* 16-bit "number of sync errors" */
    unsigned short typeerrs;   /* 16-bit "number of sequence-type errors" */
    unsigned short eoserrs;    /* 16-bit "number of wrong-EOS errors" */
    unsigned short feederrs;   /* 16-bit "number of wrong-feed-byte errors */
    unsigned short cmderrs;    /* 16-bit "number of command-execution errors */
    unsigned short emptyerrs;  /* 16-bit "number of stat-buffer-empty errors */
    unsigned short numberrs;   /* 16-bit "number of requested-status-
				  bytes-mismatch errors */
};

#ifdef MAINPROG
struct ecblastcmd ecbLastCmd = {0,0};
struct ecbool ecbOol = {0,0,0};
struct ecbtempi ecbTempi = {0,0,0,0};
struct ecbtempr ecbTempr = {0,0,0,0};
struct ecbslvstat ecbSlvStat = {0,0,0,0,0,0,0,0,0};
#else
extern struct ecblastcmd ecbLastCmd;
extern struct ecbool ecbOol;
extern struct ecbtempi ecbTempi;
extern struct ecbtempr ecbTempr;
extern struct ecbslvstat ecbSlvStat;
#endif
