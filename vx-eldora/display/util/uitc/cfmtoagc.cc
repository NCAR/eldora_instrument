#include <stream.h>
#include <fcntl.h>

main(int argc, char **argv)
{
    if (argc == 1)
      {
	  cerr << "Must specify name of cfm file.\n";
	  exit(1);
      }

    int cfm;

    if ((cfm = open(argv[1],O_RDONLY)) < 0)
      {
	  perror(*argv);
	  exit(1);
      }

    int agc = 1;

    if (argc == 3)
      {
	  if ((agc = open(argv[2],O_WRONLY | O_CREAT, 0644)) < 0)
	    {
		perror(*argv);
		exit(1);
	    }
      }

    unsigned short rs[514], gs[514], bs[514];
    
    read(cfm,gs,514);
    read(cfm,rs,514);
    read(cfm,bs,514);

    int j = 514;;
    int k = j - 1;

    long old = 0;

    for (int i = 0; i < j; i++)
      {
	  unsigned short tmp = rs[i]/ 255.0;

	  rs[i] = tmp;

	  tmp = gs[i]/ 255.0;

	  gs[i] = tmp;

	  tmp = bs[i]/ 255.0;

	  bs[i] = tmp;

	  long color = rs[i] << 24;
	  color |= gs[i] << 16;
	  color |= bs[i] << 8;

	  color &= 0xffffff00;

	  if (color != old && color != 0)
	    {
		write(agc,&color,sizeof(long));
		old = color;
		cout << rs[i] << ", " << gs[i] << ", " << bs[i] << "\n";
	    }
      }

    close(cfm);

    exit(0);
}
