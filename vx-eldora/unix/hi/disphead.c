  /* Reads in an existing header from a file and verifies it */ 

#define scope
#include "dispheadDef.h"
#include "dispheadGbl.h"
#include <netinet/in.h>

main(int argc, char **argv)
{  
  
  int bytesread, bytestoread, i;
  int version;
  FILE *filepntr;

  /*open inupt file*/

  if (argc > 1)
    strcpy(inname,argv[1]);
  else
    {
      printf("ENTER NAME OF INPUT FILE:  \n");
      gets(inname);
    }
  if((filepntr = fopen(inname,"rb")) == NULL){
    printf("UNABLE TO OPEN INPUT FILE: %s\n",inname);
    exit(1);
  }


  /*START READING IN THE DATA FROM THE FILE*/
     
  bytestoread = sizeof(volume);
  bytesread = fread(&volume,bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on volume descriptor\n");
    exit(2);
  }

  version = ntohs(volume.format_version);

  bytestoread = sizeof(waveform);
  bytesread = fread(&waveform,bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on Waveform descriptor\n");
    exit(3);
  }

  /*** read the fore radar information ***/

  bytestoread = sizeof(struct radar_d);
  bytesread = fread(&radar[FORE],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on fore radar descriptor\n");
    exit(4);
  }
  bytestoread = sizeof(struct field_radar_i);

  if (version < 2)
    bytestoread -= 80;

  bytesread = fread(&fradar[FORE],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on fore field radar info\n");
    exit(5);
  }
  bytestoread = sizeof(struct cell_spacing_d);
  bytesread = fread(&cell[FORE],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on fore cell spacing descriptor\n");
    exit(6);
  }

  actual_num_radars = 2;
  actual_num_params[FORE] = ntohs(radar[FORE].num_parameter_des);
  bytestoread = sizeof(struct parameter_d);

  for(i=0; i<actual_num_params[FORE]; i++)
    {
      bytesread = fread(&parameter[i][FORE],bytestoread,1,filepntr);
      if(bytesread <= 0){
	printf("EOF or bad read on fore parameter number: %d\n",i);
	exit(7);
      }
    }

  /*** read the aft radar information ***/

  bytestoread = sizeof(struct radar_d);
  bytesread = fread(&radar[AFT],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on AFT radar descriptor\n");
    exit(8);
  }
  bytestoread = sizeof(struct field_radar_i);

  if (version < 2)
    bytestoread -= 80;

  bytesread = fread(&fradar[AFT],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on AFT field radar info\n");
    exit(9);
  }
  bytestoread = sizeof(struct cell_spacing_d);
  bytesread = fread(&cell[AFT],bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on AFT cell spacing descriptor\n");
    exit(10);
  }

  actual_num_params[AFT] = ntohs(radar[AFT].num_parameter_des);
  bytestoread = sizeof(struct parameter_d);

  for(i=0; i<actual_num_params[AFT]; i++)
    {
      bytesread = fread(&parameter[i][AFT],bytestoread,1,filepntr);
      if(bytesread <= 0){
	printf("EOF or bad read on AFT parameter number: %d\n",i);
	exit(11);
      }
    }

  /*** read the navigational and insitu data descriptors ***/

  bytestoread = sizeof(struct nav_descript);
  bytesread = fread(&navigation,bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on navigational data descriptor\n");
    exit(12);
  }
  bytestoread = sizeof(struct insitu_descript);
  bytesread = fread(&insitudata,bytestoread,1,filepntr);
  if(bytesread <= 0){
    printf("EOF or bad read on insitu data descriptor\n");
    exit(13);
  }

  close(filepntr);

  /* Verify a header here */
 
  swapVolume(&volume);
  swapWaveform(&waveform);
  swapFieldRadar(&fradar[FORE]);
  swapFieldRadar(&fradar[AFT]);
  swapCellspacing(&cell[FORE]);
  swapCellspacing(&cell[AFT]);
  swapRadardesc(&radar[FORE]);
  swapRadardesc(&radar[AFT]);
  swapNavdesc(&navigation);
  swapInsitudesc(&insitudata);
  for (i = 0; i < actual_num_params[FORE]; i++)
    swapParameter(&parameter[i][FORE]);
  for (i = 0; i < actual_num_params[AFT]; i++)
    swapParameter(&parameter[i][AFT]);

  verify(stdout);
 
}






