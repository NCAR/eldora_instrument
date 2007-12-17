//Stepped Attenuator Class
//Written July 20, 2007


#ifndef SA_H
#define SA_H

#include <string>
#include <math.h>

#include "etherio24.h"

/**
	@author Drew
*/
class SA
{

public:
  /**
   * Constructor
   * @param ipAddres Address of the etherio24 card
   * @param port port number
   * @param radar fore or aft radar
   */
  SA(std::string ipAddress, int port, int radar);

  ///Destructor
  ~SA();

  ///Programs Attenuator Value into Attenuator
  ///@param attenval attenuator value
  void SetAtten(int attenval);

  ///Changes Attenuator Value Inputted by User into Attenuator recognizable Value
  /// @param attenval user input attenuator value
  int ChgAttenVal(int attenval);

  /// fore(0) or aft(8) radar
  int _radar;


protected:
  ///Object of Ether IO 24 Class
  EtherIO24 _ether2;

  ///Send data to specified ports
  ///@param portletter Port Name
  ///@param data Value to Send to Specified Port
  void SendToBoard(unsigned char portletter, unsigned char data);

};

#endif