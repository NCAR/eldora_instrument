/*
 *	$Id$
 *
 *	Module:		 Header.h
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.2  1991/09/11  16:28:11  thor
 * Fixed routines to match class.
 *
 * Revision 1.1  1991/09/05  16:41:36  thor
 * Initial revision
 *
 *
 *
 * description:
 *        This file defines the C routines that are used to interface
 * to the Header class. HeaderPtr is used as an opaque pointer to an
 * instance of Header.
 *
 */
#ifndef INCHeaderh
#define INCHeaderh

#define OK_RPC

#include "TapeHeader.h"

typedef void *HeaderPtr;

static const int MAX_PARAM = 10;

extern HeaderPtr CreateHeader(TAPEHEADER *t, void *v);

extern int SetParameter(HeaderPtr ptr, PARAMETER *param, int paramNum);
extern PARAMETER *GetParameter(HeaderPtr ptr, int paramNum);

extern void SetCellSpacing(HeaderPtr ptr, CELLSPACING *cs);
extern CELLSPACING *GetCellSpacing(HeaderPtr header);

extern int SetRadar(HeaderPtr ptr, RADARDESC *cs, int descNum);
extern RADARDESC *GetRadar(HeaderPtr header, int descNum);

extern void SetFieldRadar(HeaderPtr ptr, FIELDRADAR *cs);
extern FIELDRADAR *GetFieldRadar(HeaderPtr header);

extern void SetVolume(HeaderPtr ptr, VOLUME *cs);
extern VOLUME *GetVolume(HeaderPtr header);

extern void SetWaveform(HeaderPtr ptr, WAVEFORM *cs);
extern WAVEFORM *GetWaveform(HeaderPtr header);

extern void GetRpcHeader(HeaderPtr header, TAPEHEADER *th);

extern int GetRealHeader(HeaderPtr ptr, void **header);

extern void DestroyHeader(HeaderPtr ptr);

#endif /* INCHeaderh */

