#include <stdio.h>
#include "field.h"
FILE *infdf;


  union loadvheader {
         struct volume_header vheader;
         char vheadbuf[700];
  };     

  union loadforesensdesc{
        struct sensorDes  foresensordescriptor;
        char   foresensdesbuf[2000];
    };

  union loadaftsensdesc{
        struct sensorDes  aftsensordescriptor;
        char   aftsensdesbuf[2000];
    };

  union loadnavagation{
        struct navDesc     navagationdesc;
        char   navbuf[2000];
    };

read_header(vheader,foresensordescriptor,aftsensordescriptor,
            navagationdesc,numparameters)
  struct volume_header *vheader;
  struct sensorDes     *foresensordescriptor;
  struct sensorDes     *aftsensordescriptor;
  struct navaDesc      *navagationdesc;
  int *numparameters;
 {
  
  int bytesread,number;
  int voldsize,i,wavesize,size,num;
  int radarsize,fradarsize,cellsize,paramsize,navsize;
  char inname[80];  
  char string[80];
  union loadvheader volhead;
  union loadforesensdesc foresenshead;
  union loadaftsensdesc aftsenshead;
  union loadnavagation  navahead;


     /*open inupt file*/
     printf("NAME OF INPUT FILE?\n");
     gets(inname);
     if((infdf = fopen(inname,"rb")) == NULL){
        printf("UNABLE TO OPEN INPUT FILE: %s\n",inname);
        return;
     }
     printf("INPUT FILE %s IS OPEN\n",inname);

     /*START READING IN THE DATA FROM THE FILE*/
     
     voldsize = sizeof(vheader->volume);
     wavesize = sizeof(vheader->waveform);
     size     = voldsize + wavesize;
     bytesread = fread(volhead.vheadbuf,size,1,infdf);
     if(bytesread < 0){
        printf("EOF OR BAD READ\n");
       return;
     }
     /* copy the buffer into the structure*/
     bcopy(volhead.vheadbuf,vheader,size);     

/*forward sensor descriptor*/
     /*read in the foresensordescriptor fields*/
     radarsize  = sizeof(foresensordescriptor->radar);
     fradarsize = sizeof(foresensordescriptor->fradar);
     cellsize   = sizeof(foresensordescriptor->cell);
     printf("HOW MANY PARAMETERS ARE IN THE FORWARD RADAR DESCRIPTOR\n");
     gets(string);
     num = atoi(string);
     *numparameters = num;
     paramsize  = (sizeof(foresensordescriptor-> parameter)/10)*num;
     size = radarsize + fradarsize + cellsize + paramsize;
     bytesread  = fread(foresenshead.foresensdesbuf,size,1,infdf);
     if(bytesread < 0){
        printf("EOF OR BAD READ\n");
        return;
     }
     bcopy(foresenshead.foresensdesbuf,foresensordescriptor,size);

/*aft sensor descriptor*/
     /*read in the aftsensordescriptor fields*/
     radarsize  = sizeof(aftsensordescriptor->radar);
     fradarsize = sizeof(aftsensordescriptor->fradar);
     cellsize   = sizeof(aftsensordescriptor->cell);
     printf("HOW MANY PARAMETERS ARE IN THE AFT RADAR DESCRIPTOR\n");
     gets(string);
     num = atoi(string);
     *numparameters = num;
     paramsize  = (sizeof(aftsensordescriptor-> parameter)/10)*num;
     size = radarsize + fradarsize + cellsize + paramsize;
     bytesread  = fread(aftsenshead.aftsensdesbuf,size,1,infdf);
     if(bytesread < 0){
        printf("EOF OR BAD READ\n");
       return;
     }
     bcopy(aftsenshead.aftsensdesbuf,aftsensordescriptor,size);

     /*navagation descriptor*/
     navsize = sizeof(navagation);
     bytesread  = fread(navahead.navbuf,size,1,infdf);
     if(bytesread < 0){
        printf("EOF OR BAD READ\n");
       return;
     }
     bcopy(navahead.navbuf,navagationdesc,navsize);

     close(infdf);
 }




