/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 Header.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.8  1992/07/28  17:23:46  thor
 * *** empty log message ***
 *
 * Revision 1.8  1992/07/28  17:23:46  thor
 * *** empty log message ***
 *
 * Revision 1.7  1992/01/02  20:40:35  thor
 * Added method & C function to change TAPEHEADER of object.
 *
 * Revision 1.6  1992/01/02  20:28:40  thor
 * Added method for = operator.
 *
 * Revision 1.5  1991/10/22  17:03:16  thor
 * Removed now unneeded params pointer.
 *
 * Revision 1.4  1991/10/16  14:36:01  thor
 * Changed GetRealHeader to use user supplied memory.
 *
 * Revision 1.3  1991/10/14  19:14:37  thor
 * Fixed radar descriptor functions to handle 2 descriptors & added error
 * reporting for parameter functions.
 *
 * Revision 1.2  1991/09/11  16:27:09  thor
 * Added code to send RPC header.
 *
 * Revision 1.1  1991/09/05  16:41:28  thor
 * Initial revision
 *
 *
 *
 * description:
 *        The Header class provides a means of organizing the tape
 * header for passing via RPC. It also provides routines to
 * pack/unpack a real tape header tp/from the RPC form. Various
 * methods allow acces to the component parts of the tape header.
 * These will be of most use, since only the control system and the
 * recording system are interested in real tape headers.
 *
 */
#ifndef INCHeaderhh
#define INCHeaderhh

#define OK_RPC

extern "C" {
#include "Header.h"
#include "TapeHeader.h"
};

#ifdef UNIX
#include <stream.h>
#endif // UNIX

class Header {
  private:
    TAPEHEADER *th;		// Working copy of tape header.

    int numParams;		// How many parameters in use.

  public:
    Header(void);		// Virgin copy.

    Header(TAPEHEADER *t);	// Exisiting header.

    Header(void *t);		// Make one from a real tape header.

    int Parameter(PARAMETER &param, int paramNum);
    PARAMETER *Parameter(int paramNum);

    void CellSpacing(CELLSPACING &cs);
    CELLSPACING *CellSpacing(void);

    int Radar(RADARDESC &r, int descNum);
    RADARDESC *Radar(int descNum);

    void FieldRadar(FIELDRADAR &f);
    FIELDRADAR *FieldRadar(void);

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

    Header &operator=(Header &in);

    Header &operator=(TAPEHEADER *th);
#ifdef UNIX
    int Send(char *target);
    int Send(CLIENT *client);

    friend ostream& operator<<(ostream &os, Header &hdr);
    friend istream& operator>>(istream &is, Header &hdr);
#endif // UNIX

    ~Header(void);
};

#endif /* INCHeaderhh */

