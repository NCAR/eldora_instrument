/*+ DEC/CMS REPLACEMENT HISTORY, Element BLOCK_CRC.C+*/
/*+ *1     7-MAR-1988 15:41:15 JRFELLE ""+*/
/*+ DEC/CMS REPLACEMENT HISTORY, Element BLOCK_CRC.C+*/
/* -------------------------------------------------------------------------- */

unsigned int block_crc(first_addr,last_addr)
unsigned char *first_addr, *last_addr;

/*  This routine returns the 16-bit CRC value computed over the memory space
    from first_addr thru last_addr.  The routine initializes the CRC generator
    to 0xFFFF prior to doing the calculation.
*/
    {
    extern void crcgen();
    unsigned int crc;

    crc = 0xFFFF;
    while (first_addr <= last_addr) crcgen(*first_addr++,&crc);
    return(crc);
    }

/* -------------------------------------------------------------------------- */

static void crcgen(ibyte,crcp)
unsigned char ibyte;
unsigned int *crcp;

/*  This CRC generator routine utilizes a modified CRC-16 algorithm, based on
    the technique described for the BSC protocol in the article "Software
    Error Checking Procedures for Data Communication Protocols" by Wong,
    Kolofa and Krause (COMPUTER DESIGN, Feb. 1979, p. 122).  This routine
    calculates the 16-bit CRC in inverted order as compared to the algorithm
    described in the above article.  With this modification, if an input byte
    sequence is followed by the 2-byte CRC for that sequence (low byte first),
    the resultant CRC will be zero.
*/
    {
    unsigned int i, crc;

    static unsigned int crc16[16] =
	{
	0x0000, 0xCC01, 0xD801, 0x1400, 0xF001, 0x3C00, 0x2800, 0xE401,
	0xA001, 0x6C00, 0x7800, 0xB401, 0x5000, 0x9C01, 0x8801, 0x4400
	};

    crc = *crcp;
    i = (crc ^ ibyte) & 0x0F;
    crc = (crc >> 4) ^ crc16[i];
    ibyte >>= 4;
    i = (crc ^ ibyte) & 0x0F;
    *crcp = (crc >> 4) ^ crc16[i];
    }

/* -------------------------------------------------------------------------- */

