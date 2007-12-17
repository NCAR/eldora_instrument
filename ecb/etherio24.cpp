//Ether class
//Created by Charlie/Drew
//July 5/6, 2007
#include "etherio24.h"

EtherIO24::EtherIO24(std::string ipAddress, int port)
{
  _isGood = true;
  _ipAddress = ipAddress;
  _port = port;

  if (UDPSocketInit())
  {
    _isGood = false;
  }

}

////////////////////////////////////////////////////////////////////////////

EtherIO24::~EtherIO24()
{}

////////////////////////////////////////////////////////////////////////////
void
EtherIO24::Output(const unsigned char* data, int size)
{

  int d;

  d = sendto(_udpSocketFd,
             data,
             size,
             0,
             (struct sockaddr *)&_etherTo,
             sizeof(_etherTo));

  if (d == -1)
  {
    _isGood = false;
  }

  Delay();

}

////////////////////////////////////////////////////////////////////////////

void
EtherIO24::Delay()
{
  int temp=999999;
  while (temp !=0)
  {
    temp --;
  }
  return;
}

////////////////////////////////////////////////////////////////////////////
int
EtherIO24::UDPSocketInit()
{

  //Creates udp socket
  if ((_udpSocketFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    // -1 is error
    printf("Cannot create socket \n");
    return -1;
  }

  //Initializes socket
  _etherTo.sin_family = AF_INET;
  _etherTo.sin_port = htons(_port);
  _etherTo.sin_addr.s_addr = inet_addr(_ipAddress.c_str());

  return 0;

}
////////////////////////////////////////////////////////////////////////////
int
EtherIO24::UDPSocketClose()
{
  int f;

  if(!(f = close(_udpSocketFd)))
  {
    return -1;
  }
}
////////////////////////////////////////////////////////////////////////////
bool
EtherIO24::IsGood()
{
  return _isGood;
}

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////