/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 RecordComm.hh
 *	Original Author: Richard e. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
// Revision 1.1  1994/03/24  16:20:11  thor
// Initial revision
//
 *
 *
 * description:
 *        
 */
#ifndef INCRecordComm_hh
#define INCRecordComm_hh

#ifndef UNIX
#define UNIX
#define OK_RPC
#define CLIENT_SIDE

extern "C" {
#include "tapeControl.h"
};
#endif // UNIX

class RecordComm {
  public:
    RecordComm();
    
    RecordComm(char *target);

    int start(char *target);

    TapeStatus *recordOn();

    TapeStatus *recordOff();

    TapeStatus *eject();

    TapeStatus *rewind();

    TapeStatus *getStatus();

    TapeStatus *log();

  private:
    CLIENT *client;

    TapeCommand cmd;
};
#endif // INCRecordComm_hh
