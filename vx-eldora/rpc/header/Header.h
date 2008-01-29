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
 * Revision 1.9  2008/01/29 17:37:11  burghart
 * Merged rpc/include/Header.hh into rpc/include/Header.h to create a single
 * header file to be used for both C and C++ source.
 *
 * Revision 1.8  2005/08/03 20:20:56  granger
 * updates copied from the versions in the rpc/include directory, except for
 * Aircraft.h which looks it got accidentally replaced with the contents of
 * Parameter.h; some of these are commits of changes from
 * /net/eldora/eldora/rpc/header which were never committed to rcs; and some
 * files are being added to revision control for the first time
 *
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
 * 
 *   This file defines the interface (C and C++) to the Header class. 
 *   For C, HeaderPtr is used as an opaque pointer to an instance of Header.
 *
 */
#ifndef INCHeaderh
#define INCHeaderh

#ifndef OK_RPC
#define OK_RPC
#endif

#ifdef __cplusplus
extern "C" {
class Header; /* defined later */
typedef Header* HeaderPtr;
#else
typedef void* HeaderPtr; /* opaque pointer for C */
#endif

#include "TapeHeader.h"

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

/*
 * Below here is C++-only stuff, with the definition of class Header
 */
#ifdef __cplusplus
} /* end extern "C" */

#include <fstream.h>

class Header {
  protected:
    TAPEHEADER *th;   // Working copy of tape header.

    int numParams;    // How many parameters in use.

  public:
    Header(void);   // Virgin copy.

    Header(TAPEHEADER *t);  // Exisiting header.

    Header(void *t);    // Make one from a real tape header.

    Header(const char *file); // Read in from a file.

    int Parameter(PARAMETER &param, int paramNum);
    PARAMETER *Parameter(int paramNum);

    void CellSpacing(CELLSPACING &cs, int descNum = 1);
    CELLSPACING *CellSpacing(int descNum = 1);

    int Radar(RADARDESC &r, int descNum);
    RADARDESC *Radar(int descNum);

    void FieldRadar(FIELDRADAR &f, int descNum = 1);
    FIELDRADAR *FieldRadar(int descNum = 1);

    void Volume(VOLUME &v);
    VOLUME *Volume(void);

    void Waveform(WAVEFORM &w);
    WAVEFORM *Waveform(void);

    void NavDesc(NAVDESC &n);
    NAVDESC *NavDesc(void);

    void Insitu(INSITUDESC &id);
    INSITUDESC *Insitu(void);

    TAPEHEADER *GetRpcHeader(void) { return(th); }

    int GetRealHeader(void *header);

    int readFile(const char *file);

    Header &operator=(Header &in);

    Header &operator=(TAPEHEADER *th);

    friend ostream& operator<<(ostream &os, Header &hdr);
    friend istream& operator>>(istream &is, Header &hdr);

    ~Header(void);
};
#endif /* __cplusplus */

#endif /* INCHeaderh */

