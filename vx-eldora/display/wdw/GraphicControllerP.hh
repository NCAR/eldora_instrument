/*
 *	$Id$
 *
 *	Module:		 GraphicControllerP.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *        This header contains private infomation for the
 * GraphicController class. The structures are private in the sense
 * that users need not know the details of how they are done. The
 * offsets are truely private - they are only needed for class
 * initialization. 
 *
 */
#ifndef INCGraphicControllerPhh
#define INCGraphicControllerPhh

struct WindowRegisters {	/* Per window precomputed addresses. */
    long *base;
    long *scrSize;
    long *offsets;
    unsigned short *priority;
    unsigned short *zoom;
};

#ifdef GRAPHIC_CTLR_CLASS
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";
#endif GRAPHIC_CTLR_CLASS

#ifdef GRAPHIC_CTLR_PRIVATE
static const unsigned short ENABLE = 0x10;

/* Offsets to various board control registers. */
static const int GCR_OFFSET = 0x1000800;
static const int MASK_OFFSET = 0x1000802;
static const int VECTOR_OFFSET = 0x1000803;

/* Broadcast register set offset and size. */
static const int WDW0_BRS = 0x80;
static const int BRS_SIZE = 0x10;

/* Offset to TS 8-bit broadcast registers. */
static const int TS_REG_OFFSET = 0x102f000;

/* Offset to TS priority register. */
static const int TS_PRI_OFFSET = 0x102f00c;

/* Offset to TS 1-bit registers in window 3. */
static int ONE_BIT_BASES[] = {
    0x1021030, 0x1021070, 0x1022030, 0x1022070, 0x1024030, 0x1024070,
    0x1028030, 0x1028070
};

/* Offset to board control register set 2. */
static const int TS_VSYN_OFFSET = 0x102f0c0;

/* RAMDAC offsets */
static const int RAMDAC_ADDR = 0x1010000;
static const int RAMDAC_CLUT = 0x1010010;
static const int RAMDAC_CONTROL = 0x1010020;
static const int RAMDAC_OLUT = 0x1010030;

/* QPDM offsets */
static const int QPDM_FIFO = 0x1033000;
static const int QPDM_BLOCK_FIFO = 0x1033010;
static const int QPDM_WRITE_ADDR = 0x1033020;
static const int QPDM_WRITE_REG = 0x1033030;

/* Video parameters - very monitor dependent. */
static const unsigned short VSYN = 3;
static const unsigned short VBSP = 29;
static const unsigned short VFP = 3;
static const unsigned short VINIT = 0; /* No frame interrupts. */
static const unsigned short HSYN = 112;
static const unsigned short HBSP = 432;
static const unsigned short HFP = 24;
static const unsigned short HBLK = 0;

#endif GRAPHIC_CTLR_PRIVATE

#endif INCGraphicControllerPhh
