#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "bitReverse.h"

#define FAST register

static void convert(void);
static void useage(void);

main(int argc, char **argv)
{
    FAST int opt;
    FAST int fd;

    extern char *optarg;
    extern int opterr;

    opterr = 0;

    while ((opt = getopt(argc,argv,"i:o:")) != -1)
      {
	  switch(opt)
	    {
	      case 'i':
		if ((fd = open(optarg,O_RDONLY)) < 0)
		  {
		      perror(argv[0]);
		      exit(1);
		  }
		dup2(fd,0);
		break;

	      case 'o':
		if ((fd = open(optarg,O_WRONLY | O_CREAT,0666)) < 0)
		  {
		      perror(argv[0]);
		      exit(1);
		  }
		dup2(fd,1);
		break;

	      default:
		useage();
	    }
      }

    convert();

    exit(0);
}

static void convert(void)
{
    FAST unsigned char value;
    FAST char *stat = "static";
    char input[100];

    while (gets(input) != NULL)
      {
	  if (*input == '#')
	    continue;

	  if (!strncmp(input,stat,strlen(stat)))
	    puts(input);
	  else
	    {
		FAST char *left = input;
		FAST char *right;

		while ((right = strchr(left,',')) != NULL)
		  {
		      *right++ = 0;
		      
		      value = reverseChar((unsigned char)strtol(left,(char **)
								NULL,16));

		      fprintf(stdout," 0x%.2x,",value);

		      left = right;
		  }
		fprintf(stdout,"\n");
	    }
      }
    fprintf(stdout,"};\n");
}

static char *htext[] = {
"Usage:",
"",
"bmtotad [-i file] [-o file]",
"   where -i indicates that the named file is the input file to convert and",
"-o indicates the output file to write. The defaults are stdin and stdout",
"respectively.",
0};

static void useage(void)
{
    FAST char **ptr = htext;

    for (; *ptr; ptr++)
      fprintf(stderr,"%s\n",*ptr);

    exit(1);
}
