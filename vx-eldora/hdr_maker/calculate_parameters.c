#include <stdio.h>
#include <floatingpoint.h>
#include "field.h"


#define NUM 1
#define CONSTANT 16272.3247

calculate_parameters(vheader)
  struct volume_header *vheader;
{

 char string[80];
 int samples,chip,num_freqs;
 extern int cells,cellspacing;
 extern int prf;
 int    i;
 float  duty,arg1,arg2,divide,fdivisor;
 int    idivisor;
 short  sdivisor;

  do{            /*added april 29*/
    printf("ENTER THE NUMBER OF FREQUENCIES\n");
    gets(string);
    num_freqs = atoi(string);
    if((num_freqs < 0) || (num_freqs > 3)) printf("INVALID NUMBER OF FREQUENCIES:\n");
  }while(num_freqs < 0 || num_freqs > 3);

  printf("ENTER NUMBER OF SAMPLES\n");
  gets(string);
  samples = atoi(string);
  printf("ENTER PULSE REPITITION FREQUENCY\n");
  gets(string);
  prf = atoi(string);
  printf("ENTER NUMBER OF GATES\n");
  gets(string);
  cells = atoi(string);
  printf("ENTER GATE SPACING\n");
  gets(string);
  cellspacing = atoi(string);
  printf("ENTER DUTY CYCLE IN PERCENT\n");
  gets(string);
  duty = atof(string);


  for(i = 0; i < 6; i++){
      vheader->waveform.num_chips[i] = 0;
  }

  for(i = 0; i < num_freqs; i++){
      vheader->waveform.num_chips[i] = 1; 
  }
  
  chip = vheader->waveform.num_chips[0];
  vheader->waveform.num_gates[0] = cells;
  vheader->waveform.gate_dist1[1] = (cellspacing/2.5) + 0.5;
  vheader->waveform.repeat_seq_dwel = (samples/chip) + 0.5;
  vheader->waveform.repeat_seq = (chip * 1000.)/prf;
  /*initialize chip_widths to all 0*/
  for(i = 0; i < 6; i++){
      vheader->waveform.chip_width[i] = 0;
  }
  arg1     = duty * 60000000;
  arg2     = prf  * 100;
  divide   = arg1/arg2;
  fdivisor = (divide + 0.5)/num_freqs;
  idivisor = (int)(fdivisor);
  sdivisor = (short)idivisor;
  for(i = 0; i < num_freqs; i++){
      vheader->waveform.chip_width[i] = sdivisor;
      printf("chip_width{%d] = %d\n",i,vheader->waveform.chip_width[i]);
    }
  vheader->waveform.gate_dist1[0] = vheader->waveform.gate_dist1[1];
  return;
}


fix_velocity_scale_factor(sensordescriptor,numparameters)
  struct sensorDes *sensordescriptor;
  int *numparameters;

{
  int i;
  

  printf("PRF %d\n",prf);
  for(i = 0; i < *numparameters; i++){
     if(sensordescriptor->parameter[i].parameter_name[0] == 'V'&&
        sensordescriptor->parameter[i].parameter_name[1] == 'R'){
        /*sensordescriptor->parameter[i].parameter_scale = CONSTANT/prf;*/
        /*sensordescriptor->parameter[i].parameter_scale = 1.;*/
        printf("VELOCITY SCALE FACTOR IS %f FOR PARAMETER # %d\n",
              sensordescriptor->parameter[i].parameter_scale,i+1);
    }
 }
}

fix_cellspacing(sensordescriptor)
   struct sensorDes *sensordescriptor;
{

   sensordescriptor->cell.distToFirst = cellspacing; 
   sensordescriptor->cell.spacing[0] =  cellspacing;
   sensordescriptor->cell.num_cells[0] = cells;
}



