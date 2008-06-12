//Drew Leung
//with help from Tom and Charlie
//ether_setIP.c
//June 19,2007

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{

  int udp_socket;
  int z;
  int b;
  int len_inet;
  socklen_t etherlen;
  int j;
  int ip1;
  int ip2;
  int ip3;
  int ip4;
  //receive buffer
  char dgram[512];
  struct sockaddr_in etherTo;
  struct sockaddr_in etherFrom;
  unsigned char epromCmd[5];

  printf("--------Ether IO 24 IP Address Setup Menu----------\n\n");
  printf("Enter the first set of numbers of the IP Address (XXX.xxx.xxx.xxx):\n");
  scanf("%d",&ip1);
  printf("Enter the second set of numbers of the IP Address (xxx.XXX.xxx.xxx):\n");
  scanf("%d",&ip2);
  printf("Enter the third set of numbers of the IP Address (xxx.xxx.XXX.xxx):\n");
  scanf("%d",&ip3);
  printf("Enter the last set of numbers of the IP Address (xxx.xxx.xxx.XXX):\n");
  scanf("%d",&ip4);



  //Create udp socket;
  if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    // -1 is error
    printf("Cannot create socket \n");
  }
  else
  {
    // 0 or positive is succesful
    printf("Opened socket: %d\n",udp_socket);
  }


  //Initializes the UDP Socket
  etherTo.sin_family = AF_INET;
  //Port Number 2424
  etherTo.sin_port = htons(2424);
  etherTo.sin_addr.s_addr = inet_addr("10.10.10.10");
  len_inet = sizeof etherTo;
  printf("length of etherTo: %d \n",len_inet);

  //Send 'IO24' to the board to get MAC address
  b= sendto(udp_socket,
            "IO24",
            4,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  printf("%x\n", etherTo.sin_addr.s_addr);

  if (b < 1)
  {
    //Need strerror to translate errno to words
    printf("%s \n",strerror(errno));
  }

  //Accepting message
  z = recvfrom(udp_socket,
               dgram,
               sizeof dgram,
               0,
               (struct sockaddr *)&etherFrom,
               &etherlen);

  if(z != -1)
  {
    //If z does not equal 1-> received something,
    //Print whatever is in buffer
    printf("recvfrom returned: %d\n", z);
    printf("%s\n",dgram);
    printf("%x\n", etherFrom.sin_addr.s_addr);
    printf("dgram contains: ");
    for (j = 0; j < z; j++)
      printf("%02x ", (unsigned char)dgram[j]);
    printf("\n");
  }


  //Write enable EEPROM- 5 bytes
  epromCmd[0] = '\'';
  epromCmd[1] = '1';
  epromCmd[2] = 0;
  epromCmd[3] = 0xAA;
  epromCmd[4] = 0x55;

  //Sendto spits out number of bytes sent
  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //Enable fixed IP function; write to address 5
  epromCmd[0] = '\'';
  epromCmd[1] = 'W';
  epromCmd[2] = 5;
  epromCmd[3] = 0;
  epromCmd[4] = 254;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //Write First Half of IP address to address 6
  epromCmd[0] = '\'';
  epromCmd[1] = 'W';
  epromCmd[2] = 6;
  epromCmd[3] = ip2;
  epromCmd[4] = ip1;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //Write last half of IP address to address 7
  epromCmd[0] = '\'';
  epromCmd[1] = 'W';
  epromCmd[2] = 7;
  epromCmd[3] = ip4;
  epromCmd[4] = ip3;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //read address 6
  epromCmd[0] = '\'';
  epromCmd[1] = 'R';
  epromCmd[2] = 6;
  epromCmd[3] = 0;
  epromCmd[4] = 0;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //read address 7
  epromCmd[0] = '\'';
  epromCmd[1] = 'R';
  epromCmd[2] = 7;
  epromCmd[3] = 0;
  epromCmd[4] = 0;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //Read what is in address 5
  epromCmd[0] = '\'';
  epromCmd[1] = 'R';
  epromCmd[2] = 5;
  epromCmd[3] = 0;
  epromCmd[4] = 0;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);

  //reset module to activate new settings on board
  epromCmd[0] = '\'';
  epromCmd[1] = '@';
  epromCmd[2] = 0;
  epromCmd[3] = 0xAA;
  epromCmd[4] = 0x55;

  b= sendto(udp_socket,
            epromCmd,
            5,
            0,
            (struct sockaddr *)&etherTo,
            sizeof(etherTo));
  printf("sendto returned: %d \n",b);
  sleep(1);


  close(udp_socket);

  printf("The Ether IO 24 Board is now set to %d.%d.%d.%d\n",ip1,ip2,ip3,ip4);

  return(0);
}





