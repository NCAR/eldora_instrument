#include <stream.h>
#include <fcntl.h>

#define FAST register

#define OK_RPC
#define UNIX
#define CLIENT_SIDE
extern "C" {
#include "TapeHeader.h"
};

#include "ParamNames.h"

#include "Header.hh"

struct v {
    VOLUME vol;
    WAVEFORM wave;
};

struct rd {
    RADARDESC rdr;
    FIELDRADAR frdr;
    CELLSPACING cs;
};

static const int dsize = sizeof(struct v) + (2 * sizeof(struct rd)) + 
		    (20 * sizeof(PARAMETER));
static char datablk[dsize];

main(FAST int argc, FAST char **argv)
{
#ifndef DEBUG
    FAST int opt;
    FAST int type = 1;
    extern int optind;
    extern char *optarg;

    while ((opt = getopt(argc,argv,"s:")) != -1)
      {
	  if (opt == 's')
	    type = atoi(optarg);
      }

    if (optind == argc)
      {
	  cerr << "Sorry, you must specify a file name.\n";
	  exit(1);
      }

    FAST char *file = argv[optind];

    FAST int fd;
    if ((fd = open(file,O_RDONLY)) < 0)
      {
	  perror(argv[0]);
	  exit(1);
      }

    read(fd,datablk,dsize);

    FAST struct rd *ptr = (struct rd *)(datablk + sizeof(struct v));

    FAST int p = ptr->rdr.num_parameter_des;

    FAST int psize = p * sizeof(PARAMETER);

    FAST struct rd *ptr2 = (struct rd *)(datablk + sizeof(struct v)  + psize +
					 sizeof(struct rd));

    if (type == 1)
      bcopy(ptr,ptr2,sizeof(struct rd) + psize);
    else
      read(fd,(char *)ptr2,sizeof(struct rd) + psize);

    Header hdr(datablk);

#else
    int max = 3;

    if (argc == 2)
      max = atoi(argv[1]);

    Header hdr;

    PARAMETER p;

    PARAMETER *pm;

    p.parameter_bias = 0;
    p.parameter_scale = 1;

    for (FAST int i = 0; i < max; i++)
      {
	  hdr.Parameter(p,i);
	  pm = hdr.Parameter(i);

	  strcpy(&pm->parameter_name[0],ParamTapeNames[i]);
      }

    RADARDESC *rdr = hdr.Radar(1);

    rdr->num_parameter_des = 10;
    rdr->total_num_des = 11;

    CELLSPACING cs;

    cs.num_segments = 1;
    cs.spacing[0] = 300;
    cs.num_cells[0] = 400;
    cs.distToFirst = 0;

    hdr.CellSpacing(cs);
#endif // DEBUG

    CELLSPACING *pcs = hdr.CellSpacing();

    PARAMETER *pm = hdr.Parameter(0);

    cout << pm->parameter_name << "\n";
    char target[20];

    *target = 0;

    cout << "Enter target system name: ";
    cin >> target;

    FAST CLIENT *clnt = clnt_create(target,HeaderRPC,HeaderVers,"tcp");

    if (clnt == NULL)
      {
	  clnt_pcreateerror(argv[0]);
	  exit(1);
      }

    TAPEHEADER *th = hdr.GetRpcHeader();

    int k;

    cout << "Enter number of tries: ";
    cin >> k;

    FAST int j = k;


    for (FAST int i = 1; i < j; i++)
      {
	  hdr.Send(target);

	  cout << "Sent " << i << " times.\n";
      }

    exit(0);
}

static void donothing(void)
{
    exit(0);
}
