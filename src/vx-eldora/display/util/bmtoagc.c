#include <stdio.h>
#include <string.h>

#define FAST register

void top(FILE *, char *);
void parse(FILE *, FILE *);

main(FAST int argc, FAST char **argv)
{
    FAST int opt;
    FAST FILE *pipe;
    FAST FILE *fp = stdout;
    FAST char *ptr;
    char cmd[256];
    extern char *optarg;
    extern int optind;

    while ((opt = getopt(argc,argv,"o:")) != -1)
      {
	  if (opt == 'o')
	    {
		fp = fopen(optarg,"w");

		if (fp == NULL)
		  {
		      perror(*argv);
		      exit(1);
		  }
		fclose(stdout);
	    }
      }

    if (argc == optind)
      {
	  fprintf(stderr,"%s: Sorry, you must specify a bitmap file.\n",*argv);
	  exit(2);
      }

    strcpy(cmd,"bmtoa -chars 01 ");
    strcat(cmd,argv[optind]);

    if ((pipe = popen(cmd,"r")) == NULL)
      {
	  perror(*argv);
	  exit(3);
      }

    if ((ptr = strrchr(argv[optind])) == NULL)
	ptr = argv[optind];

    top(fp,ptr);

    parse(pipe,fp);

    fprintf(fp,"0 }; \n");

    fclose(fp);

    fclose(pipe);

    exit(0);
}

void parse(FAST FILE *in, FAST FILE *out)
{
    FAST unsigned int value;
    char input[256];

    
    while (fgets(input,255,in) != NULL)
      {
	  FAST char *ptr = input;
	  FAST int i;
	  FAST int j = 32;

	  value = 0;

	  for (i = 0; i < j; i++)
	    {
		value <<= 1;

		if (*ptr++ == '1')
		  value++;
	    }
	  
	  fprintf(out,"0x%x, ",value);

	  value = 0;

	  for (i = 0; i < j; i++)
	    {
		value <<= 1;

		if (*ptr++ == '1')
		  value++;
	    }
	  
	  fprintf(out,"0x%x, \n",value);
      }
}

static char *toptext[] = {
"/* This is an AGC formatted bitmap header file. */\n",
"",
"long ",
0};


void top(FAST FILE *fp, FAST char *name)
{
    FAST char **ptr = toptext;

    for (; *ptr; ptr++)
      fprintf(fp,*ptr);

    fprintf(fp,name);

    fprintf(fp,"[] = {\n");
}
