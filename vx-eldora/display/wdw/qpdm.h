/*
 *	$Id$
 *
 *	Module:		 qpdm.h
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
 *        These are definitions of QPDM specific items.
 *
 */
#ifndef INCqpdmh
#define INCqpdmh

/* Fifo flags - HF == half full, AE == almost empty */
static const int FIFO_HF = 0x80;
static const int FIFO_AE = 0x40;

/* Turn on external fifo */
static const unsigned char FIFO_ENABLE = 0x10;

// These are QPDM codes for setting the drawing environment.
static const short SET_COLOR = 0x20;
static const short SET_BLK_SIZE = 0x22;
static const short LINE_STYLE = 0x25;
static const short LINE_PHASE = 0x26;
static const short SET_SCALE = 0x29;

static const short SET_ACT_BITS = 0x30;
static const short SET_LISTEN_BITS = 0x31;
static const short CONTROL_CLIP = 0x34;
static const short CONTROL_PICK = 0x35;
static const short DEF_PEL = 0x36;
static const short SET_QPDM_POS = 0x38;
static const short SET_STACK = 0x39;

static const short MOVE_PEN = 0x40;

static const short COPY_BLK = 0x5c;
static const short TRANSFORM_BLK = 0x5e;

static const short FONT_BASE = 0x63;

// Graphic codes.
// Note: Graphic drawing commands must have an op added to work correctly!
static const short ARC_CURRENT = 0x8;
static const short CIRCLE_CURRENT = 0xa;
static const short LINE_CURRENT = 0xc;

static const short FILLED_RECT_CURRENT = 0x10;
static const short FILLED_TRI_CURRENT = 0x12;

static const short STRING = 0x44;
static const short ARC = 0x48;
static const short CIRCLE = 0x4a;
static const short LINE = 0x4c;
static const short POINT = 0x4e;

static const short FILLED_RECT = 0x50;
static const short FILLED_TRI = 0x52;

// Logical and Graphic operations.
static const short L_SET = 0;
static const short L_OR = 0x100;
static const short L_AND = 0x200;
static const short L_XOR = 0x300;
static const short L_ZERO = 0x400;
static const short G_SET = 0x500;
static const short G_OR = 0x600;
static const short G_XOR = 0x700;

#endif INCqpdmh

