/*  
  Load a motorola S-record file into ECB MASTER DPRAM, for execution
  by the HC11 on it's side of the DPRAM.
  
  NOTE: Currently hard-coded for S1-type records.
  
  return values:
       0  = sucessful load;
       -1 = file open error;
       -2 = not an S-record file;
       -3 = checksum error;
       -4 = file termination record not found;
       */

#include <vxWorks.h>
#include <stdioLib.h>
#include "ecbMaster.h"
short ldsrec(tfile)
     
     char tfile[];
{
  char *mnt_pnt,*fname,temp[80];
  char firsttwo[3],S0[3],S1[3],S9[3];
  FILE *fp1;
  unsigned char *addr;
  int i,c,charcnt,chksumi,chksumo,reladdrh,reladdrl,reladdr;

  strcpy(S0,"S0");
  strcpy(S1,"S1");
  strcpy(S9,"S9");
  
  /* build the mounted file system name */
  i = 28;
  mnt_pnt = (char *) malloc(i);
  mnt_pnt = "thor:/pc_gbl/shawn";
  i = strlen(mnt_pnt) + strlen(tfile) + 2;
  fname = (char *) malloc(i);
  sprintf(fname, "%s/%s", mnt_pnt, tfile);
  
  if ((fp1=fopen(fname,"r")) == NULL)
    {
      printf("File open error for %s... aborting\n",fname);
      return(-1);
    }
  printf("file: %s is now open for reading\n",fname);
  
  /* Get first record */
  fscanf(fp1,"%2s",firsttwo);
  if (strcmp(firsttwo,S0) == 0)
    {
      chksumi = chksumo = 0;
      fscanf(fp1,"%2x",&charcnt);
      chksumi += charcnt;
      printf("Header from S-record file follows:\n");
      for (i=1; i<charcnt; i++)
	{
	  fscanf(fp1,"%2x",&c);
	  chksumi += c;
	  printf("%c",c);
	}
      printf("\n");
      fscanf(fp1,"%2x",&chksumo); /* read checksum */
      chksumi = 0xff & ~chksumi;
      if (chksumi != chksumo)
	{
	  printf("Checksums don't match for record #1... aborting\n");
	  return(-3);
	}
      c = getc(fp1); /* get newline character */
      fscanf(fp1,"%2s",firsttwo);  /* get first two characters of next line */
    }
  else
    {
      printf("Initial header record (S0) not found in file.\n");
    }
  
  printf("Loading code into absolute (VME) address:\n");
  /* get rest of records, load memory as we go */
  while (strcmp(firsttwo,S1) == 0)
    {
      charcnt = chksumi = chksumo = 0;
      fscanf(fp1,"%2x",&charcnt); /* get number of bytes */
      chksumi += charcnt;
      charcnt -= 3;               /* adjust by 3 bytes of address & checksum */
      fscanf(fp1,"%2x%2x",&reladdrh,&reladdrl); /* get relative address bytes*/
      reladdr = (reladdrh<<8) + reladdrl; /* calculate 16-bit address */
      chksumi += (reladdrh + reladdrl);
      addr = (unsigned char *)(MASTERBASE + reladdr); /*calculate VME address*/
      printf("starting address is %8x, number of bytes to write is %d\n",addr,charcnt); 
      for (i=0; i<charcnt; i++)
	{
	  fscanf(fp1,"%2x",&c);
	  chksumi += c;
	  *addr = (unsigned char) c;
	  printf("address %8x written with %2x\n",addr,c);
	  addr++;
	}
      chksumi = 0xff & ~chksumi;
      fscanf(fp1,"%2x",&chksumo);  /* get checksum */
      if (chksumi != chksumo)
	{
	  printf("Checksum error... aborting\n");
	  return(-3);
	}
      c = getc(fp1);               /* get newline character */
      fscanf(fp1,"%2s",firsttwo);  /* get first two chars of next line */
    }

  if (strcmp(firsttwo,S9) != 0)
    {
      printf("File termination record (S9) not found... aborting\n");
      return(-4);
    }
  return(0);
}











