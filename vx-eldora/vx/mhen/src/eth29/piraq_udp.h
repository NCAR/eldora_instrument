/*
 * $Date$
 $ $Id$
 */
#ifndef PIRAQ_UDP_H
#define PIRAQ_UDP_H

#include "piraq/dd_types.h"

/* define UDP packet types */
#define UDPTYPE_CMD             1
#define UDPTYPE_IQDATA  2
#define UDPTYPE_ABPDATA 3

#define  MAGIC           0x12345678
#define MAXPACKET       1200

typedef struct {
    uint4 magic;             /* must be 'MAGIC' value above */
    uint4 type;             /* e.g. DATA_SIMPLEPP, defined in piraq.h */
    uint4 sequence_num;     /* increments every beam */
    uint4 totalsize;      /* total amount of data only (don't count the size of this header) */
    uint4 pagesize;       /* amount of data in each page INCLUDING the size of the header */
    uint4 pagenum;		/* packet number : 0 - pages-1 */
    uint4 pages;     /* how many 'pages' (packets) are in this group */
} UDPHEADER;
#endif /* PIRAQ_UDP_H */

/*
 * LOG:
 * $Log$
 * Revision 1.2  2003/03/14 15:20:50  vanandel
 * *** empty log message ***
 *
 * Revision 1.1  2003/01/31 21:47:33  vanandel
 * add sequence_num to UDPHEADER, and place in separate file.
 *
 */
