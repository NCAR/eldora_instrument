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
 * Revision 1.1  1992/05/29  17:55:11  shawn
 * Initial revision
 *
 * description: Contains global status structures for ECB status.
 *        
 */

struct ecbflags      /* GENERAL BUS STATUS BIT FLAGS */
{
    unsigned char ecbgen;      /* 8 bits of general purpose ECB status flags */
    unsigned char slvalive;    /* 8 bits of alive/dead flags for ECB slaves */
    unsigned char onBoverT;    /* 8 bits of "on-board overtemp" flags */
    unsigned char offBoverT;   /* 8 bits of "off-board overtemp" flags */
    unsigned char forDDSool;   /* 8 bits of "forward DDS out-of-lock" flags */
    unsigned char aftDDSool;   /* 8 bits of "aft DDS out-of-lock" flags */
};

struct ecblastcmd    /* ID OF LAST COMMAND COMPLETED */
{
    unsigned char ecbadr;      /* ECB address of slave last command sent to */
    unsigned char comID;       /* Command ID of last command completed */
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
struct ecbflags ecbFlags = {0x00,0x00,0x00,0x00,0x00,0x00};
struct ecblastcmd ecbLastCmd = {0,0};
struct ecbtempi ecbTempi = {0,0,0,0};
struct ecbtempr ecbTempr = {0,0,0,0};
struct ecbslvstat ecbSlvStat = {0,0,0,0,0,0,0,0,0};
#else
extern struct ecbflags ecbFlags;
extern struct ecblastcmd ecbLastCmd;
extern struct ecbtempi ecbTempi;
extern struct ecbtempr ecbTempr;
extern struct ecbslvstat ecbSlvStat;
#endif
