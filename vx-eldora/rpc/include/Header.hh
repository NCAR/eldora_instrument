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

    PARAMETER *params;		// Working copy of parameters.
};

#ifdef UNIX
#include <stream.h>
#endif // UNIX

class Header {
  private:
    TAPEHEADER *th;		// Working copy of tape header.

    void Parameter(PARAMETER &param, int paramNum);

  public:
    Header(void);		// Virgin copy.

    Header(TAPEHEADER *t);	// Exisiting header.
    void Radar(RADARDESC &r);
    RADARDESC *Radar(void);

    int Parameter(PARAMETER &param, int paramNum);
    PARAMETER *Parameter(int paramNum);

    void CellSpacing(CELLSPACING &cs);
    CELLSPACING *CellSpacing(void);

    int Radar(RADARDESC &r, int descNum);
    RADARDESC *Radar(int descNum);

    void Waveform(WAVEFORM &w);
    WAVEFORM *Waveform(void);
    int GetRealHeader(void **header);

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

