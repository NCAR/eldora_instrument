//EtherIO24 Class
//Created by Charlie/Drew
//July 5/6, 2007
#ifndef ETHERIO24_H
#define ETHERIO24_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

/**
	@author Drew
*/
/// Representation of an EtherIO24 device. Arbitrary bytes
/// can be sent to the device via UDP packets. Bytes can
/// also be read from an EhterIO24.
class EtherIO24
{
public:
  /// Constructor
  /// @param ipAddress The ipAddress of the board
  /// @param port The port number.
  EtherIO24(std::string ipAddress, int port);

  /// Destructor
  ~EtherIO24();

  /// Send bytes to the Ether IO 24
  /// @param data The bytes to be sent
  /// @param size The number of bytes
  void Output(const unsigned char* data, int size);

  /// Return an indication of whether the class/device is working properly.
  /// @return True if okay, false if there has been a error.
  bool IsGood();


protected:
  /// Delay with a iterative loop for some undefined time period.
  void Delay();

  /// Create the UDP socket
  /// @return 0 if create succeeded, -1 if failed
  int UDPSocketInit();

  /// Close the UDP Socket
  /// @return -1 if failed
  int UDPSocketClose();

  /// The current good/bad indicator.
  bool _isGood;

  /// The udp socket file descriptor.
  int _udpSocketFd;

  /// The IP name
  std::string _ipAddress;

  /// The IP port
  int _port;

  /// Network structure for sending datagrams
  struct sockaddr_in _etherTo;

  /// Network structure for receiveing datagrams
  struct sockaddr_in _etherFrom;


};

#endif
