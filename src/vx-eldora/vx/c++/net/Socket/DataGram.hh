/*  This looks like C, but it's really C++!!
 *	$Id$
 *
 *	Module:		DataGram.hh 
 *	Original Author: Richard E. Neitzel
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
#ifndef INCDataGramhh
#define INCDataGramhh

#include "Socket.hh"

class DataGram : public Socket {
  public:
    DataGram(int port, char *host = NULL);
};

#endif // INCDataGramhh

