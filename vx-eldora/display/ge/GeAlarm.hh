/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		 GeAlarm.hh
 *	Original Author: Richard E. K. Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:		 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 * Revision 1.1  1991/10/01  16:15:16  thor
 * Initial revision
 *
 *
 *
 * description:
 *        The class GeAlarm encapsulates everything needed to generate
 * alarms from the realtime crate to the control crate. To set the
 * alarm to a particular value use Set. Use Post to send the alarm on
 * its way.
 *
 */
#ifndef INCGeAlarm
#define INCGeAlarm

class GeAlarm;

#include "GeGlobal.hh"

class GeAlarm {
  private:
    DispStatus status;

    CLIENT *client;

    int socket;

    int system;

  public:
    GeAlarm(char *server, int system);

    void Set(int error);

    void Post(void) { sendalarm_1(&status,client); }

    ~GeAlarm(void);
};



#endif // INCGeAlarm

