//Ether class
//Created by Charlie/Drew
//July 5/6, 2007
#include <netdb.h>
#include "etherio24.h"

EtherIO24::EtherIO24(std::string hostname, int port) :
	_hostName(hostname), _port(port)
{
  _isGood = (UDPSocketInit() == 0);
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
  struct hostent* hent = gethostbyname(_hostName.c_str());
  if (! hent)
  {
	  printf("Lookup of host '%s' failed.\n", _hostName.c_str());
	  return -1;
  }
  
  _etherTo.sin_family = AF_INET;
  _etherTo.sin_port = htons(_port);
  _etherTo.sin_addr.s_addr = *(long*)hent->h_addr_list[0];

  return 0;

}
////////////////////////////////////////////////////////////////////////////
int
EtherIO24::UDPSocketClose()
{
  int f;

  if(!(f = close(_udpSocketFd)))
    return -1;
  
  return 0;
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
