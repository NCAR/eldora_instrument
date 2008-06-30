
#ifndef TRIGMUX_H
#define TRIGMUX_H

#include <string>
#include <math.h>

#include "etherio24.h"

/**
	@author Drew
*/
class TrigMux
{
public:
  ///Constructor
  /// @param ipAddres Address of the etherio24 card
  /// @param port port number
  /// @param muxchoice Choose test pulse mux(0) or transmit mux (8). 
  TrigMux(std::string ipAddress, int port, int muxchoice);

  ///Destructor
  ~TrigMux();

  ///Sets the Tigger Mux with the Counter Value
  /// @param channelnum channel number
  /// @param countevalue value for the counter in the Trigger Mux
  void SetMux(int channelnum, int countervalue);

  /// test pulse or transmit mux (0 or 8)
  int _muxchoice;

protected:
  ///Object of Ether IO 24 Class
  EtherIO24 _ether3;

  ///Send data to specified ports
  ///@param portletter Port Name
  ///@param data Value to Send to Specified Port
  void SendToBoard(unsigned char portletter, unsigned char data);


};

#endif
