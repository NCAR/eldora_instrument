/*
 *	$Id$
 *
 *	Module:	NU_LDDSP	 
 *	Original Author: Eric Loew 
 *      Copywrited by the National Center for Atmospheric Research
 *	Date:	 $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 *
 * description:
 *
 *	Loads ASCII files dumped from COFF executable into DSP32C via
 *	Parllel IO port set up as an 8 bit port.
 *      Also retries load up to 5 times if unsuccessful.
 *	The dump command format to generate the ASCII file is:
 *	
 *	d3dump -sp "COFF filename" > "dumpfilename"
 *
 *	(d3dump is a utility in the AT&T DSP32C Software Support Library)
 *
 *	(The COFF file is the normal output of d3make or d3ld)
 *
 *	The command line arguments are:
 *	1) # of triples
 *      2) frequency i.e. 1,2, or 3
 *	3) Board number 1,2,... as set on address switches
 *	4) DSP number   0,1,2,3 (for Quad DSP)
 *			0	(for Collator)
 *	5) filename	unix pathname for .dmp files 
 *                      e.g."/ELDORA/vx/mhen/tadpole/bin"
 * ...repeat above three arguments for other dsp's/boards.
 *        
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "vxWorks.h"
#include "stdioLib.h"
#include "nfsLib.h"
#include "varargs.h"
#include "iosLib.h"
#include "strLib.h"
#include "memLib.h"
 
#include "ctype.h"
#include "ELDRP7.h"

int dspgo();  
unsigned long getaddr();
unsigned int  load8();
static int freq, board, dsp, num_retry, j; 

nu_lddsp(va_alist)
va_dcl
{
    va_list ap;

  unsigned long  bd, frq;
  int res, dst, den, dspc, na, n;
  char *tfile;
  unsigned char *pio;
 
    va_start(ap);

    num_retry=0;
    n = va_arg(ap, int);
 
    for(na = 0; na < n; na++)
      {
	  frq = va_arg(ap, int);         /* frequency 1,2, or 3 */
	  freq = frq;
	  frq = (frq << 24) + EXTD_BASE;  /* frequency # * 10**6 + EXTD_BASE */
	  bd = va_arg(ap, int);          /* Board number */
	  board = bd;
	  bd = bd << 20;	         /* Board # * 10**5 = base addr */
	  bd += frq;                     /* Full base addr. of board */
	  dspc = va_arg(ap, int);
	  if(dspc < 0)
	    {
		dst = 0;
		den = 3;
	    }
	  else
	    {
		dst = dspc;
		den = dspc;
	    }
 
	  tfile = va_arg(ap, char *);            /* File to load */
	  for(j=dst;j<=den;j++)
	    {
		pio = (unsigned char *)(bd + (DSPSEL + ((unsigned long)j * DSPOFF)));
		dsp = j;
		/* Reset DSP in Software */
		*(pio + PCRL) = 0x0;
		*(pio + PCRL) = 0x1;
		/* Stop DSP */
		*(pio + PCRL) = 0x1A; /* DMA + AUTO + REGMAP; */
		*(pio + PCRH) = 0x0;
		printf("pcr register %x \n",*(pio + PCRL));
		printf("Load to pio addr %lx\n", pio);
		res = load8(pio,tfile);
		if(res >= 0)
		  {
		      printf("0x%x bytes loaded.\n",res);
		  }
		if(res == -1)
		  {
		      printf("Unable to open file  %s.\n",tfile);
		  }
		if(res == -2)
		  {
		      dspgo(1,freq,board,dsp);
		      j--;
		  }
	    }
      }
    va_end(ap);
}


unsigned int load8(pio, tfile)
unsigned char *pio;
char *tfile;
{
  FILE *nfs_fopen(),*fp1;
  unsigned int n, chksumo, chksumi, ii, flg;
  unsigned int lg = 0;
    int st = 0;
  char test[10],ptr[10], scr;
  unsigned long addr;
  int dl,dd;
  
  unsigned i;
  char *mnt_pnt,*fname;
  
  union
  {
    unsigned char pa[4];
    unsigned long ad24;
  }dspa;
 
  union
  {
    unsigned char pd[2];
    unsigned short data;
  }dspd;


  /* build the mounted file system name */
  i = 28;
  mnt_pnt = malloc(i);
  mnt_pnt = "odin:/vxbin/radar/dsp";
  i = strlen(mnt_pnt) + strlen(tfile) + 2;
  fname = malloc(i);
  (void) sprintf(fname, "%s/%s", mnt_pnt, tfile);
/*  
printf("here before fopen, tfile = %s\n",tfile);
printf("here before fopen, fname = %s\n",fname);
*/ 
  st = 0;
  if(!(fp1=fopen(fname,"r")))
  {
    st = -1; /* Return a "-1" if file unavailable */

    return(st);
  }
 
  n = 0;
  chksumo = 0;               /* Zero checksums */
  chksumi = 0;
  flg = 0;
  addr = 0xffffffff;
 
  do
  {
    fscanf(fp1,"%s",test);
    if((*test != '.') && (strlen(test) >=4 ))
    {
      sscanf(test,"%x",&lg);
      dspd.data = lg;
 
     /*  Output bytes to PDR here */
      *(pio + PDRL) = dspd.pd[0];
/*      for(dl=0;dl<1024;dl++)dd = dl;              A slight delay */
      *(pio + PDRH) = dspd.pd[1];
      chksumo += dspd.pd[0] + dspd.pd[1];
      n += 2;
      flg +=1;
    }
    else
    {
      if(flg >= 2)
      {
         chksumi = 0;
         printf("%s start addr = %lx\n",ptr,dspa.ad24);
         *(pio + PARE) = dspa.pa[2];
         *(pio + PARL) = dspa.pa[0];      
	/* If a xfer happened, compute the checksum */
        *(pio + PARH) = dspa.pa[1];
         for( ii = 0; ii < flg; ii++)
         {
            chksumi += *(pio + PDRL);
            chksumi += *(pio + PDRH);
         }
        if(chksumi != chksumo)
	{
	    if(num_retry < 6)
	      {
		  printf("Checksum error on load; retrying...\n");
		  num_retry++;
		  n=-2;
	      }
	    else
	      {
		  num_retry=0;
		  printf("     Load file checksum = %x\n",chksumo);
		  printf("     Readback  checksum = %x\n",chksumi);
		  n=0;
	      }
	}
	 else 
	   {
	       printf("Load successful: checksum = %x\n",chksumi);
	       num_retry=0;
	   }
     }
      if(n != -2)
	{
	    strcpy(ptr,test);
	    flg = 0;
	    chksumo = 0;
	    addr = getaddr(test);              /* Get the segment address */
	    dspa.ad24 = addr;
	}
/*  Set up PAR here   */
 
      *(pio + PARE) = dspa.pa[2];
      *(pio + PARL) = dspa.pa[0];

      *(pio + PARH) = dspa.pa[1];
    }
  }while(!feof(fp1) && n!=-2);
  scr = *(pio + PDRL);
  scr = *(pio + PDRH);
  fclose(fp1);
  return(n);
}
 
 
unsigned long getaddr(test)
char *test;
{
/* This is a map of the DSP32C address space for mode 6 */
/* Note: bytes are in 680X0 order */
  unsigned long adr;
  if(!strcmp(test,".bank0:"))       adr = 0x0;
  else if(!strcmp(test,".extA:"))   adr = 0x0;
  else if(!strcmp(test,".extB:"))   adr = 0x00006000;
  else if(!strcmp(test,".hi_ram:")) adr = 0x00f0ff00;
  else if(!strcmp(test,".rom:"))    adr = 0xffffffff;
  else if(!strcmp(test,".bank1:"))  adr = 0x00f0ff00;
  else if(!strcmp(test,".ram0:"))   adr = 0x00f0ff00;
  else if(!strcmp(test,".ram1:"))   adr = 0x00f8ff00;
  else if(!strcmp(test,".ram2:"))   adr = 0x00e0ff00;
  else adr = 0xffffffff;
  return(adr);
}






