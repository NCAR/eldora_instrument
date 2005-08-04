#include <stdio.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

struct dblk {
    float angle;
    unsigned char data[1024];
};

struct dblk Data;

#define FAST register

main(FAST int argc, FAST char **argv)
{
    FAST int i;
    FAST int j;
    FAST int loops = 360;
    FAST int sfd;
    float angle = 0.0;
    float inc = 1.0;
    float rollover = 360.0;
    struct tms t1, t2;
    FAST struct dblk *ptr;
    FAST struct dblk *data;

    struct sockaddr_in addr;

    if (argc > 1)
      loops = atoi(argv[1]);

    j = loops * sizeof(struct dblk);

    if ((ptr = (struct dblk *)malloc(j)) == NULL)
      {
	  fprintf(stderr,"Not enough memory!\n");
	  exit(1);
      }

    data = ptr;

    sfd = open("crunched.data",O_RDONLY);

    for (i = 0; i < loops; i++, ptr++)
      {
	  FAST int k;
	  FAST int l = 1024;

	  k = read(sfd,ptr,sizeof(struct dblk));

	  if (k != sizeof(struct dblk))
	    fprintf(stderr,"Only read %d bytes!\n",k);

	  for (k = 0; k < 1024; k++)
	    if (ptr->data[k] >= 62)
	      fprintf(stderr,"Found out of bound data = 0x%x.\n",ptr->data[k]);
      }

    close(sfd);

    sfd = socket(AF_INET,SOCK_DGRAM,0);

    if (sfd < 0)
      {
	  perror(argv[0]);
	  exit(1);
      }

    bzero((char *)&addr,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("128.117.85.1");
    addr.sin_port = 6000;

    times(&t1);

    ptr = data;

    for (i = 0; i < loops; i++, ptr++)
      {
	  sendto(sfd,(char *)ptr,sizeof(struct dblk),0,&addr,sizeof(addr));
	  
	  usleep(1000);
      }

    Data.angle = -1.0;

    sendto(sfd,(char *)&Data,sizeof(Data),0,&addr,sizeof(addr));

    times(&t2);

    rollover = (float)(t2.tms_utime - t1.tms_utime + t2.tms_stime - 
		       t1.tms_stime) / 60.0;

    printf("Elapsed time - %f.\n",rollover);

    sleep(2);

    close(sfd);

    exit(0);
}
