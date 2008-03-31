//DDS class
//Created by Charlie/Drew
//July 5/6, 2007


#ifndef DDS_H
#define DDS_H

#include <string>
#include <math.h>

#include "etherio24.h"

/**
	@author Drew
*/
class DDS
{
public:
  /**
   * Constructor
   * @param ipAddres Address of the etherio24 card
   * @param port port number
   * @param unit DDS unit number; 0 based (0-4)
   */
  DDS(std::string ipAddress, int port, int unit);

  ///Destructor
  ~DDS();

  ///Send frequency to DDS Module
  ///@param freqin Desired frequency
  ///@param uin unit number; 0 based
  ///@param nrin Noise Reduction
  void Rfputval(double freqin, int nrin);

  ///Initialize the DDS
  void Rfinit();


protected:
  ///Creates Ether IO 24 Object
  EtherIO24 _ether;

  ///DDS unit number; 0 based
  int _ddsunit;

  ///Send bytes to EtherIO 24 to Toggle DS bit
  void Toggle();

  ///Send data to specified ports
  ///@param portletter Port Name
  ///@param data Value to Send to Specified Port
  void SendToBoard(unsigned char portletter, unsigned char data);

  ///Set Upper Four Bits to DDS Unit Address
  void SetDDSAddress();

  ///Variable used to store results of bit manipulations
  unsigned char _q;

  ///Variable represeting address <5:7>
  unsigned char _DDS67;

  ///Variable representing address <8:9>
  unsigned char _DDS89;

  ///Address Array
  unsigned char _Address[2];

  ///Data Array
  unsigned char _Data[2];

  ///Select Array
  unsigned char _Select[2];

  ///Hex to Dec Conversion
  ///@param hex Hex Value
  ///@return Decimal Value
  int hextodec(char hex);

  ///Dec to Hex Conversion
  ///@param dec Decimal Value
  ///@return Hex Value
  std::string dectohex(int dec);


};

#endif

