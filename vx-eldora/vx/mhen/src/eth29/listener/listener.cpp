#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#define DO_STREAM 0

int main(int argc, char** argv) {

  if (DO_STREAM) {
    int status;
 
    struct sockaddr_in sa;

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    std::cout << "socket call returned " << fd << std::endl;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(50000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    status = bind(fd, (sockaddr*)&sa, sizeof(sa));
    std::cout << "bind call returned " << status << std::endl;

    status = listen(fd, 100);
    std::cout << "listen call returned " << status << std::endl;


    std::cout << "accepting connections" << std::endl;

    while(1) {
   
      int newFd;
      struct sockaddr  clientAddr;
      socklen_t sizeClient = sizeof(clientAddr);
   
      if ((newFd = accept(fd, &clientAddr,
			  &sizeClient)) < 0) {
	perror("accept");
	close (fd);
	return -1;
      }

      unsigned int maxBufSize =33554432;

      if (setsockopt(newFd, SOL_SOCKET, SO_RCVBUF, &maxBufSize, 
		     sizeof(maxBufSize)) < 0) {
	perror( "setsockopt - setting max. buf size" );
	exit (1);

      }
      unsigned int actualBufSize =0;
      socklen_t paramSize = sizeof(actualBufSize);

      if (getsockopt(newFd, SOL_SOCKET, SO_RCVBUF, &actualBufSize, 
		     &paramSize) < 0) {
	perror( "getsockopt - getting max. buf size" );
	exit (1);
      }

      std::cout << "got a connection" << std::endl;

      int total = 0;
      int last_total = 0;

      time_t last_time = time(NULL);

      while(1) {

	unsigned char buf[20000];

	status = read(newFd, buf, sizeof(buf));
	time_t this_time = time(NULL);

	total += status;

	int delta = this_time - last_time;
	if (delta >= 10) {
	  double throughput = (total - last_total)/ delta ;
	  std::cout  << delta << " secs, " << (total - last_total)/1000.0 <<
	    " kb, " << throughput/1000.0 << " kb/s" << std::endl;;
	  last_total = total;
	  last_time = this_time;
	}

	if (status < 0) {
	  perror("recv");
	  break;
	}
      }
    }
  } else {

    int status;
 
    int readCount = 0;

    struct sockaddr_in sa;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    std::cout << "socket call returned " << fd << std::endl;

    unsigned int maxBufSize = 33554432;

    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &maxBufSize, 
		   sizeof(maxBufSize)) < 0) {
      perror( "setsockopt - setting max. buf size" );
      exit (1);

    }
    unsigned int actualBufSize =0;
    socklen_t paramSize = sizeof(actualBufSize);

    if (getsockopt(fd, SOL_SOCKET, SO_RCVBUF, &actualBufSize, 
		   &paramSize) < 0) {
      perror( "getsockopt - getting max. buf size" );
      exit (1);
    }

    sa.sin_family = AF_INET;
    sa.sin_port = htons(50000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    status = bind(fd, (sockaddr*)&sa, sizeof(sa));
    std::cout << "bind call returned " << status << std::endl;

    int total = 0;
    int last_total = 0;

    time_t last_time = time(NULL);

    while(1) {

      unsigned char buf[20000];

      status = read(fd, buf, sizeof(buf));
      time_t this_time = time(NULL);
      readCount++;

      total += status;

      int delta = this_time - last_time;
      if (delta >= 10) {
	double throughput = (total - last_total)/ delta ;
	std::cout << "reads:" << readCount << ", " << delta << " secs, " << (total - last_total)/1000.0 <<
	  " kb, " << throughput/1000.0 << " kb/s" << std::endl;;
	last_total = total;
	last_time = this_time;
	readCount = 0;
      }

      if (status < 0) {
	perror("recv");
	break;
      }
    }
  }
}
