#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>

#define DO_STREAM 1

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
 
    struct sockaddr_in sa;

    int fd = socket(AF_INET, SOCK_DGRAM, 0);
    std::cout << "socket call returned " << fd << std::endl;

    sa.sin_family = AF_INET;
    sa.sin_port = htons(50000);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    status = bind(fd, (sockaddr*)&sa, sizeof(sa));
    std::cout << "bind call returned " << status << std::endl;

    while(1) {

      unsigned char buf[2000];

      status = read(fd, buf, sizeof(buf));

      std::cout << "recv returned " << status << std::endl;

      if (status < 0) {
	perror("recv");
	break;
      }
    }
  }
}
