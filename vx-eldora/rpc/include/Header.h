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
 * Revision 1.7  1992/07/28  17:31:02  thor
 * Added nav & insitu stuff.
 *
 * Revision 1.6  1992/01/02  20:41:07  thor
 * Added method & C function to change TAPEHEADER of object.
 *
 * Revision 1.6  1992/01/02  20:41:07  thor
 * Added method & C function to change TAPEHEADER of object.
 *
 * Revision 1.5  1992/01/02  20:28:51  thor
 * Added method for copying.
 *
 * Revision 1.4  1991/10/16  14:36:17  thor
 * Changed GetRealHeader to use user supplied memory.
 *
 * Revision 1.3  1991/10/14  19:13:34  thor
 * Fixed radar descriptor functions to handle 2 descriptors & added error repor
 * ing for parameter functions.
 *
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

#ifndef OK_RPC
#define OK_RPC
#endif

#include "TapeHeader.h"

typedef void *HeaderPtr;

static const int MAX_PARAM = 10;

extern HeaderPtr CreateHeader(TAPEHEADER *t, void *v);

extern HeaderPtr CreateHeaderFromFile(char *file);

extern int SetParameter(HeaderPtr ptr, PARAMETER *param, int paramNum);
extern PARAMETER *GetParameter(HeaderPtr ptr, int paramNum);

extern void SetCellSpacing(HeaderPtr ptr, CELLSPACING *cs, int descNum);
extern CELLSPACING *GetCellSpacing(HeaderPtr header, int descNum);

extern int SetRadar(HeaderPtr ptr, RADARDESC *cs, int descNum);
extern RADARDESC *GetRadar(HeaderPtr header, int descNum);

extern void SetFieldRadar(HeaderPtr ptr, FIELDRADAR *cs, int descNum);
extern FIELDRADAR *GetFieldRadar(HeaderPtr header, int descNum);

extern void SetVolume(HeaderPtr ptr, VOLUME *cs);
extern VOLUME *GetVolume(HeaderPtr header);

extern void SetWaveform(HeaderPtr ptr, WAVEFORM *cs);
extern WAVEFORM *GetWaveform(HeaderPtr header);

extern void SetNavDesc(HeaderPtr ptr, NAVDESC *nd);
extern NAVDESC *GetNavDesc(HeaderPtr ptr);

extern void SetInsitu(HeaderPtr ptr, INSITUDESC *id);
extern INSITUDESC *GetInsitu(HeaderPtr ptr);

extern void GetRpcHeader(HeaderPtr header, TAPEHEADER *th);

extern int GetRealHeader(HeaderPtr ptr, void *header);

extern void CopyHeader(HeaderPtr src, HeaderPtr dest);

extern void ResetHeader(HeaderPtr ptr, TAPEHEADER *th);

extern void DestroyHeader(HeaderPtr ptr);

extern int readHeaderFile(HeaderPtr ptr, char *file);

#endif /* INCHeaderh */

