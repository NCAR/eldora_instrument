
static char FN[] = __FILE__;

#include <stdLoc.h>
#include <stdlib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <vme.h>

#define NO_RP7_SCOPE

#include "vxWorks.h"
#include "math.h"
#include "stdioLib.h"
#include "intLib.h"
#include "memLib.h"
#include "semLib.h"
#include "taskLib.h"
#include "tyLib.h"
#include "ioLib.h"
#include "in.h"
#include "systime.h"
#include "sysLib.h"

#include "RadarGbls.h"
#include "HeaderRpc.h"
#include "Parameter.h"
#include "RadarDesc.h"
#include "Header.h"
#include "CellSpacing.h"
#include "Ray.h"
#include "Platform.h"
#include "FieldParam.h"
#include "Waveform.h"
#include "IndFreq.h"
#include "TimeSeries.h"
#include "RDPGlobals.h"
#include "raw_data.h"
#include "mcpl_def.h"
#include "mcpl_gbl.h"
#include "LclCommands.h"
#include "ELDRP7.h"

#include "coll_dpramdefs.h"
#include "colquadsubs.h"
#include "dspdefs.h"
#include "dspaddr.h"
#include "rp7.h"

int ascope()
{
    int choice, choice_1, choice_2, status;
    
    while(1)
        {
            printf("\n");
            printf("\n");
            printf("ASCOPE DIAGNOSTIC MENU \n");
            printf("\n");

            printf("1) Raw Time Series \n");
            printf("2) Power and Velocity \n");
            printf("3) Power Spectrum \n");
	    printf("4) Select Time Series Gate and Freq \n");
            printf("5) Exit ASCOPE Diagnostic Menu \n");

            scanf(" %d",&choice);
            switch(choice)
                {
                  case 1:
                      do
                      {
                          printf("\n");
                          printf("\n");
                          printf("Raw Time Series Menu \n");
                          printf("\n");
                          printf("1) Display Inphase Data \n");
                          printf("2) Display Quadrature Data \n");
			  printf("3) Change Scale Factor \n");
			  printf("4) Return to Main Menu \n");
                          scanf(" %d",&choice_1);
                          switch(choice_1)
                              {
                                case 1:
				  ASCOPE_mode = TIMESERIES;
				  TS_off = 0;
                                    break;
                                case 2:
				  ASCOPE_mode = TIMESERIES;
				  TS_off = 1;
                                    break;
                                case 3:
				  printf("\n");
				  printf("\n");
				  printf("ENTER SCALE FACTOR (FLOAT)\n");
				  scanf(" %f",&SCALE_fac);
				  break;
                                case 4:
                                    break;
                              }
                      }while(choice_1 != 4);
                      break;
                  case 2:
                      do
                      {
                          printf("\n");
                          printf("\n");
                          printf("Doppler Moments Menu \n");
                          printf("\n");
                          printf("1) Display Power \n");
                          printf("2) Display Velocity \n");
			  printf("3) Return to Main Menu \n");
                          scanf(" %d",&choice_1);
                          switch(choice_1)
                              {
                                case 1:
				  ASCOPE_mode = POWER;
                                    break;
                                case 2:
				  ASCOPE_mode = VELOCITY;
                                    break;
                                case 3:
                                    break;
                              }

                           }while(choice_1 != 3);
                      break;
                               
		case 3:
		  printf("\n");
		  printf("\n");
		  do {
		    printf("Enter Number of points in FFT \n");
		    scanf(" %d",&choice_1);
		    if(choice_1 == 64 || choice_1 == 128 || choice_1 == 256 || choice_1 == 512 || choice_1 == 1024)
		      {
			FFT_pts = choice_1;
			status = 1;
		      }
		    else
		      {
			printf("Invalid FFT size \n");
			status = 0;
		      }
		  }while(!status);
		  ASCOPE_mode = PWR_SPECTRUM;
		  break;
		case 4:
		     do
                      {
                          printf("\n");
                          printf("\n");
                          printf("Raw Time Series Menu \n");
                          printf("\n");
                          printf("1) Select Time Series Gate \n");
                          printf("2) Select Time Series Frequency \n");
			  printf("3) Return to Main Menu \n");
                          scanf(" %d",&choice_1);
                          switch(choice_1)
                              {
                                case 1:
                                        printf("\n");
                                        printf("\n");
                                        printf("ENTER TIME SERIES GATE \n");
                                        scanf(" %d",&TS_gate);
                                    break;
                                case 2:
				  do
				    {
				      printf("\n");
				      printf("\n");
				      printf("ENTER TIME SERIES FREQUENCY \n");
                                        scanf(" %d",&choice_2);
					if(choice_2 < 1 || choice_2 >5)
					  {
					    printf("FREQUENCY MUST BE BETWEEN 1 AND 5 \n");
					    status = 0;
					  }
					else
					  {
					    TS_freq = choice_2;
					    status = 1;
					  }
				    }while(!status);
                                    break;
                                case 3:
                                    break;
                              }
                      }while(choice_1 != 3);
                      break;

		    break;
		case 5:
		  return(-1);
		  break;
                }
        }
}
