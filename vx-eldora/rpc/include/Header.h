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
 *
 *
 * description:
 *        
 */
#ifndef INCHeaderh
#define INCHeaderh

#define OK_RPC

#include "TapeHeader.h"

typedef void *HeaderPtr;

static const int MAX_PARAM = 10;

extern HeaderPtr CreateHeader(TAPEHEADER *t, void *v);

extern void SetParameter(HeaderPtr ptr, PARAMETER *param, int paramNum);
extern PARAMETER *GetParameter(HeaderPtr ptr, int paramNum);

extern void SetCellSpacing(HeaderPtr ptr, CELLSPACING *cs);
extern CELLSPACING *GetCellSpacing(HeaderPtr header);

extern void SetRadar(HeaderPtr ptr, RADARDESC *cs);
extern RADARDESC *GetRadar(HeaderPtr header);

extern void SetFieldRadar(HeaderPtr ptr, FIELDRADAR *cs);
extern FIELDRADAR *GetFieldRadar(HeaderPtr header);

extern void SetVolume(HeaderPtr ptr, VOLUME *cs);
extern VOLUME *GetVolume(HeaderPtr header);

extern void SetWaveform(HeaderPtr ptr, WAVEFORM *cs);
extern WAVEFORM *GetWaveform(HeaderPtr header);

extern TAPEHEADER *GetRpcHeader(HeaderPtr header);

extern void *GetRealHeader(HeaderPtr ptr);

#endif /* INCHeaderh */

