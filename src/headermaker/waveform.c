#define scope extern
#include <stdlib.h>
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/****************************************************************************/
/*            SUBROUTINE INITIALIZE THE WAVEFORM DESCRIPTOR                */
/***************************************************************************/
void initialize_waveform(waveform_pntr)
      struct waveform_d *waveform_pntr;
   {

      int size,i;

          waveform_pntr->waveform_des[0] = 'W';
          waveform_pntr->waveform_des[1] = 'A';
          waveform_pntr->waveform_des[2] = 'V';
          waveform_pntr->waveform_des[3] = 'E';

          size = sizeof(struct waveform_d);
          waveform_pntr-> waveform_des_length = size;

          for(i = 0; i < 15; i++) waveform_pntr->ps_file_name[i] = ' ';

          for(i = 0; i < 6; i++) waveform_pntr->num_chips[i] = 0;

          for(i = 0; i < 256; i++) waveform_pntr->blank_chip[i] = 0;

          waveform_pntr->repeat_seq = -999.;
          waveform_pntr->repeat_seq_dwel = -999.;
          waveform_pntr->total_pcp = -999.;

          for(i = 0; i < 6; i++) waveform_pntr->chip_offset[i] = -999;

          for(i = 0; i < 6; i++) waveform_pntr->chip_width[i] = -999;

          waveform_pntr->ur_pcp = -999.;
          waveform_pntr->uv_pcp = -999.;

          for(i = 0; i < 6; i++) waveform_pntr->num_gates[i] = -999;

          for(i = 0; i < 2; i++){
              waveform_pntr->gate_dist1[i] = -999;
              waveform_pntr->gate_dist2[i] = -999;
              waveform_pntr->gate_dist3[i] = -999;
              waveform_pntr->gate_dist4[i] = -999;
              waveform_pntr->gate_dist5[i] = -999;
	  }
    }/*end initialize_waveform_descriptor*/
/****************************************************************************/
/*            SUBROUTINE INITIALIZE THE WAVEFORM STATE                      */
/***************************************************************************/
void initialize_waveform_state()
   {

       waveform_st.ps_file_name[0] = 0;
       waveform_st.num_chips[0] = 0;
       waveform_st.blank_chip[0] = 0;
       waveform_st.repeat_seq = 0.0;
       waveform_st.repeat_seq_dwel = 0.0;
       waveform_st.total_pcp = 0.0;
       waveform_st.chip_offset[0] = 0;
       waveform_st.chip_width[0] = 0;
       waveform_st.ur_pcp = 0.0;
       waveform_st.uv_pcp = 0.0;
       waveform_st.num_gates[0] = 0;
       waveform_st.gate_dist1[0] = 0;

    }/*end initialize_waveform_state */

/**************************************************************************/
/*               SUBROUTINE EDIT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

void edit_waveform(waveform_pntr)
    struct waveform_d *waveform_pntr;
  {
    char string[80],input[80];
    int choice;
    short numchip,chipoffset,chipwidth,i;
    float nummilliseconds;
    short dwell,totalpcp,numgates,gatedist;
    float urpcp,uvpcp;
    char blank;
    
    for(;;){
      do{
        printf("\nEDIT WAVEFORM MENU\n");
        printf("0. RETURN TO EDIT INITIALIZE MENU\n");
        printf("1. ENTER THE PULSING SCHEME FILE NAME (16 ASCII CHARS)\n");
        printf("2. ENTER THE NUMBER OF CHIPS IN A REPEAT\n");
        printf("3. ENTER BLANKING RAM SEQUENCE\n");
        printf("4. ENTER THE NUMBER OF MILLISECONDS IN A REPEAT SEQUENCE(ms)\n");
        printf("5. ENTER THE NUMBER OF REPEAT SEQUENCES IN A DWELL TIME\n");
        printf("6. ENTER TOTAL NUMBER OF PCP IN A REPEAT SEQUENCE\n");
        printf("7. ENTER THE NUMBER OF 60Mhz CLOCK CYCLES TO WAIT BEFORE STARTING A PARTICULAR CHIP IN 60 Mhz COUNTS\n");
        printf("8. ENTER THE NUMBER OF 60Mhz CLOCK CYCLES IN EACH CHIP IN 60Mhz UNTS\n");
        printf("9. ENTER THE NUMBER OF PCP THAT SET THE UNAMBIGUOUS RANGE,(after real time unfolding)\n");
        printf("10.ENTER THE NUMBER OF PCP THAT SET THE UNAMBIGUOUS VELOCITY,(after  real time unfolding\n");
        printf("11. ENTER TOTAL NUMBER OF GATES SAMPLED\n");
        printf("12. GATE DISTANCE 1\n");
        printf("13. GATE DISTANCE 2\n");
        printf("14. GATE DISTANCE 3\n");
        printf("15. GATE DISTANCE 4\n");
        printf("16. GATE DISTANCE 5\n");
        printf("ENTER YOUR CHOICE:\n");
        gets(input);
        choice = atoi(input);
        if(choice < 0 || choice >16) printf("INVALID ENTRY\n");
      }while(choice <0 || choice > 16);


      switch(choice){
          case 0:
                 return;
                 break;
	  case 1:
                 do{
                    printf("ENTER THE FILE NAME\n");
                    gets(string);
                    if(strlen(string) > 16) printf("STRING TOO LONG\n"); 
		 } while(strlen(string) >16);
                 strcpy(waveform_pntr->ps_file_name,string);                
                 break;
	   case 2:
                   i = 0;
                   do{
                      printf("CHIP %d ?.  ENTER -99 TO QUIT\n",i + 1);
                      gets(input);
                      numchip = atoi(input);
                      if(numchip == -99){
                         i = 6;
		      }
                      else{
                          waveform_pntr->num_chips[i] = numchip;
                          i++;
		      }
		     } while( i < 6);
                     break;
	    case 3:
                     for(i=0; i<waveform_pntr->total_pcp; i++)
		       {
                        printf("\nBlanking Ram number %d : ",i);
                        gets(input);
			blank = atoi(input);
			waveform_pntr->blank_chip[i] = blank;
		    }
                        break;
	     case 4:  
                      printf("ENTER MILLISECONDS:\n");
                      gets(input);
                      nummilliseconds = atof(input);
                      waveform_pntr->repeat_seq = nummilliseconds;
                      break;
             case 5:
                      printf("ENTER DWELL:\n");
                      gets(input);
                      dwell = atoi(input);
                      waveform_pntr->repeat_seq_dwel = dwell;
                      break;
             case 6:
                      printf("ENTER TOTAL PCP IN REPEAT SEQUENCE:\n");
                      gets(input);
                      totalpcp = atoi(input);
                      waveform_pntr->total_pcp = totalpcp;
                      break;
	     case 7:
                   i = 0;
                   do{
                      printf("CHIP OFFSET  %d ?.  ENTER -99 TO QUIT\n",i + 1);
                      gets(input);
                      chipoffset = atoi(input);
                      if(chipoffset == -99){
                         i = 6;
		      }
                      else{
                          waveform_pntr->chip_offset[i] = chipoffset;
                          i++;
		      }
		     } while( i < 6);
                     break;

              case 8:
                   i = 0;
                   do{
                      printf("CHIP WIDTH %d ?.  ENTER -99 TO QUIT\n",i + 1);
                      gets(input);
                      chipwidth = atoi(input);
                      if(chipwidth == -99){
                         i = 6;
		      }
                      else{
                          waveform_pntr->chip_width[i] = chipwidth;
                          i++;
		      }
		     } while( i < 6);
                     break;
               case 9:
                      printf("ENTER NUMBER OF PCP:\n");
                      gets(input);
                       urpcp = atof(input);
                      waveform_pntr->ur_pcp = urpcp;
                      break;
               case 10:
                      printf("ENTER NUMBER OF PCP:\n");
                      gets(input);
                      uvpcp = atof(input);
                      waveform_pntr->uv_pcp = uvpcp;
                      break;
               case 11:
                   i = 0;
                   do{
                      printf("NUM GATES  %d ?.  ENTER -99 TO QUIT\n",i + 1);
                      gets(input);
                      numgates = atoi(input);
                      if(numgates == -99){
                         i = 5;
		      }
                      else{
                          waveform_pntr->num_gates[i] = numgates;
                          i++;
		      }
		     } while( i < 5);
                     break;
               case 12:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         waveform_pntr->gate_dist1[i] = gatedist;
		     }
                     break;
               case 13:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         waveform_pntr->gate_dist2[i] = gatedist;
		     }
                     break;
               case 14:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         waveform_pntr->gate_dist3[i] = gatedist;
                     }
                     break;
               case 15:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         waveform_pntr->gate_dist4[i] = gatedist;
                     }
                     break;
                case 16:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         waveform_pntr->gate_dist5[i] = gatedist;
                     }
                     break;
                default:
                     break;
 
        } /*switch*/
    }/*for*/
  }/*end edit waveform descriptor*/


/**************************************************************************/
/*               SUBROUTINE FPRINT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

void fprint_waveform(waveform_pntr, print_pntr)
    struct waveform_d *waveform_pntr;
    FILE *print_pntr;
  {
    int i,k;

   fprintf(print_pntr,"***************WAVEFORM DESCRIPTOR*****************\n");
     fprintf(print_pntr,"WAVEFORM IDENTIFIER        :");
     for(i=0; i<4; i++)
       fprintf(print_pntr,"%c",waveform_pntr->waveform_des[i]);
     fprintf(print_pntr,"\nWAVEFORM DESCRIPTOR LENGTH : %ld\n",
             waveform_pntr->waveform_des_length);
     fprintf(print_pntr,"PULSING SCHEME FILE NAME   : ");
     for(i = 0; i < 16; i++)
        fprintf(print_pntr,"%c",waveform_pntr->ps_file_name[i]);
     fprintf(print_pntr,"\nNUMBER OF CHIPS IN A REPEAT: ");
     for(i = 0; i< 6; i++) 
        fprintf(print_pntr,"%d ",waveform_pntr->num_chips[i]);
      fprintf(print_pntr,"\nBLANKING RAM SEQUENCE      : ");
         k = 0;
          do{
             fprintf(print_pntr,"%3d> ",k);      
             for(i=0; i<10; i++)
	       {
		   fprintf(print_pntr,"%2x ",waveform_pntr->blank_chip[k]);
		   k++;
	       }
	     fprintf(print_pntr,"\n");
	 }while(k < waveform_pntr->total_pcp);

      fprintf(print_pntr,"\nNUMBER OF MILLISECONDS PER REPEAT : %f\n",
              waveform_pntr->repeat_seq);
      fprintf(print_pntr,"NUMBER OF REPEAT SEQUENCES IN A DWELL TIME: %d\n",
		 waveform_pntr->repeat_seq_dwel);
      fprintf(print_pntr,"TOTAL NUMBER OF PCP IN A REPEAT SEQUENCE  : %d\n",
                 waveform_pntr->total_pcp);
      fprintf(print_pntr,"CHIP OFFSET (60 MHz counts)       : ");
       for(i = 0; i < 6; i++)  
         fprintf(print_pntr,"% d",waveform_pntr->chip_offset[i]);
      fprintf(print_pntr,"\nCHIP WIDTH (60 MHz counts): ");
      for(i = 0; i < 6; i++)
         fprintf(print_pntr,"%d ",waveform_pntr->chip_width[i]);
      fprintf(print_pntr,"\nPCP THAT SET THE UNAMBIGIOUS RANGE : %f\n",
                                     waveform_pntr->ur_pcp);
      fprintf(print_pntr,"PCP THAT SET THE UNAMBIGIOUS VELOCITY : %f\n",
                                     waveform_pntr->uv_pcp);
      fprintf(print_pntr,"TOTAL NUMBER OF SAMPLED GATES:\n");
      for(i = 0; i < 5; i++) 
          fprintf(print_pntr," %d",waveform_pntr->num_gates[i]);
   fprintf(print_pntr,"\n");
   fprintf(print_pntr,"GATE DISTANCE 1 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist1[0],waveform_pntr->gate_dist1[1]);
   fprintf(print_pntr,"GATE DISTANCE 2 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist2[0],waveform_pntr->gate_dist2[1]);
   fprintf(print_pntr,"GATE DISTANCE 3 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist3[0],waveform_pntr->gate_dist3[1]);
   fprintf(print_pntr,"GATE DISTANCE 4 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist4[0],waveform_pntr->gate_dist4[1]);
   fprintf(print_pntr,"GATE DISTANCE 5 (60 MHz Counts): %d %d\n",
	   waveform_pntr->gate_dist5[0],waveform_pntr->gate_dist5[1]);
  }/*end fprint_waveform */

/****************************************************************************/
/*            SUBROUTINE CALCULATE THE WAVEFORM DESCRIPTOR                 */
/***************************************************************************/
int calculate_waveform()
{
  #define PERIOD_60MHZ      1.6666666e-8   /* 60 MHz clock period in seconds */
  #define METERS_PER_60MHZ  2.5            /* Meters per 60 MHz clock period */
  #define NS_TO_60MHZ       0.06           /* Multiplier for nano-sec to 60 MHz
					      clock periods */

  int i;
  int hpa_on_time;
  int hpa_off_time;
  int fst_gate_60mhz;
  float afloat;
  int width;
  int number_to_calculate;

  number_to_calculate = 6;
 
/*  If the pulsing scheme file name has already been selected
    by the user then we can do a bunch of stuff */

  if(waveform_st.ps_file_name[0] != 0)  /* Has this already been done */    
    number_to_calculate--;

  else if(engin_st.schemenum > 0)       /* Can it be done this time ? */
    {
	number_to_calculate--;
	waveform_st.ps_file_name[0] = 2;

/* Set the pulsing scheme file name to that selected */
 
	for(i = 0; i < 16; i++)
	  waveform.ps_file_name[i] = pscheme[engin_win.schemenum].name[i];

/*  The number of chips of each frequency is dependent on pulsing scheme */

	for(i = 0; i < 6; i++)
	  waveform.num_chips[i] = pscheme[engin_win.schemenum].num_chips[i];
	waveform_st.num_chips[0] = 2;

/* The blanking RAM sequence is dependent on the pulsing scheme */

	for(i = 0; i < 256; i++)
	  waveform.blank_chip[i] = pscheme[engin_win.schemenum].blank_chip[i];
	waveform_st.blank_chip[0] = 2;

/* Total number of PCP in a repeat sequence is dependent on pulsing scheme */

	waveform.total_pcp = pscheme[engin_win.schemenum].total_pcp;
	waveform_st.total_pcp = 2;

/* The number of PCP that set the unambiguous range and velocity are
    pulsing scheme dependent */

	waveform.ur_pcp = pscheme[engin_win.schemenum].ur_pcp;
	waveform.uv_pcp = pscheme[engin_win.schemenum].uv_pcp;
	waveform_st.ur_pcp = 2;
	waveform_st.uv_pcp = 2;

    } /* Things that are completely determined by the pulsing scheme */

/* User has selected number of samples in a dwell (but dwell time needs to be
   an integer number of repeat sequences) */

  if(waveform_st.repeat_seq_dwel != 0)
    number_to_calculate--;
  else if(engin_st.schemenum > 0 && engin_st.numsamples > 0)
    {
	afloat = (float)engin_win.numsamples + 
	  0.5 * pscheme[engin_win.schemenum].samples_in_repeat;
	waveform.repeat_seq_dwel = 
	  afloat / pscheme[engin_win.schemenum].samples_in_repeat;
	number_to_calculate--;
	waveform_st.repeat_seq_dwel = 3;
    }

/* number of milliseconds in repeat sequence is dependent on number of PCP
    that set the PRF and the total number of PCP in a repeat sequence (both
    pulsing scheme dependent), and the user selected PRF */
 
  if(waveform_st.repeat_seq != 0)       /* Has this already been calculated? */
    number_to_calculate--;
  else if(engin_st.schemenum > 0 && engin_st.prf > 0)
    {
    waveform.repeat_seq = 1000.0 / (engin_win.prf * 
      (float)pscheme[engin_win.schemenum].pcp_sets_prf)*
	(float)waveform.total_pcp;
    number_to_calculate--;
    waveform_st.repeat_seq = 3;
    }

    /* Use HPA turn on and off times, plus the chip width
       specified by the user to set the chip offsets and widths. */

  if(waveform_st.chip_offset[0] != 0) /* Have they already been calculated? */
    number_to_calculate--;
  else if(engin_st.forehpa_snum > 0 && engin_st.afthpa_snum > 0 
	  && engin_st.chipdur > 0 && waveform_st.num_chips[0] > 0
	  && engin_st.schemenum > 0)
    {
    hpa_on_time = hpa[engin_win.forehpa_snum].turn_on_time;
    if( hpa_on_time < hpa[engin_win.afthpa_snum].turn_on_time)
          hpa_on_time = hpa[engin_win.afthpa_snum].turn_on_time; 

    /* If takes 250 ns for the exciter to turn on, if we haven't allowed
       enough time for that, we need to extend the on time */
    if(hpa_on_time < 250) hpa_on_time = 250;

    hpa_off_time = hpa[engin_win.forehpa_snum].turn_off_time;
    if( hpa_off_time > hpa[engin_win.afthpa_snum].turn_off_time)
          hpa_off_time = hpa[engin_win.afthpa_snum].turn_off_time; 

/* First, we need to calculate the number of nano-seconds in a pulse */

    afloat = 1000.0 * engin_win.chipdur *
      pscheme[engin_win.schemenum].chips_in_pulse;
 
    for(i = 0; i < 5; i++)
      {
	  if(waveform.num_chips[i] > 0)
		waveform.chip_offset[i] = hpa_on_time * NS_TO_60MHZ +
		  pscheme[engin_win.schemenum].chip_percent[i] *
		    afloat * 0.01 * NS_TO_60MHZ;
	  else
	    waveform.chip_offset[i] = 0;
      }

    waveform.chip_offset[5] = 0;  /* Preknock is always first */
    waveform_st.chip_offset[0] = 3;

    for(i = 0; i < 5; i++)
      {
	  if(waveform.num_chips[i] > 0)
		waveform.chip_width[i] =
		  (150. * engin_win.chipdur) / METERS_PER_60MHZ;
	  else
		waveform.chip_width[i] = 0;
      }

/* Width of preknock is entire pulse, plus the turn on time, we used to
   subtract the turn off time, but the exciter turns off with preknock
   causing the last chip to get raggy */

    waveform.chip_width[5] = (afloat + hpa_on_time) * NS_TO_60MHZ;

    waveform_st.chip_width[0] = 3;
    number_to_calculate --;
}

    /* The number of gates to sample was user selected */

  if(waveform_st.num_gates[0] != 0) /* Have they already been calculated? */
    number_to_calculate--;
  else if(engin_st.numgates > 0 && waveform_st.num_chips[0] > 0 )
    {

    for(i = 0; i < 5; i++)
      {
	  if(waveform.num_chips[i] > 0)
	    waveform.num_gates[i] = engin_win.numgates;
	  else
	    waveform.num_gates[i] = 0;
      }
    waveform_st.num_gates[0] = 3;
    number_to_calculate--;
    }

/* The time to the first gate sample is dependent on user selected range
       to the first gate - and delays inherent in the system - these
       delays are dependent on the IF frequency number */
if(waveform_st.gate_dist1[0] != 0)
  number_to_calculate--;
else if(engin_st.chipdur > 0 && engin_st.first_gate_dis > 0 &&

	fradar_st.dly_tube_antenna > 0)
  {

/* width of the chips in 60 MHertz counts */

    width = (150. * engin_win.chipdur) / METERS_PER_60MHZ;

    fst_gate_60mhz = width/2 +
      rcvr[FORE].delay_chip_rcvrs * NS_TO_60MHZ +
      2 * fradar[FORE].dly_tube_antenna * NS_TO_60MHZ + 
	engin_win.first_gate_dis / METERS_PER_60MHZ;

    if(fst_gate_60mhz < (1.5 * width))
      {
	  fst_gate_60mhz = 1.5 * width;
	  engin_win.first_gate_dis = METERS_PER_60MHZ *
	    (fst_gate_60mhz - width/2 - 
	     rcvr[FORE].delay_chip_rcvrs *  NS_TO_60MHZ -
	     2 * fradar[FORE].dly_tube_antenna * NS_TO_60MHZ); 
      }
 
    if(waveform.num_chips[0] > 0)
      {
          waveform.gate_dist1[0] = fst_gate_60mhz;
          waveform.gate_dist1[1] = width;
      }
     else
      {
          waveform.gate_dist1[0] = -999;
          waveform.gate_dist1[1] = -999;
       }
     if(waveform.num_chips[1] > 0)
       {
          waveform.gate_dist2[0] = fst_gate_60mhz;
          waveform.gate_dist2[1] = width;
       }
     else
       {
	   waveform.gate_dist2[0] = -999;
	   waveform.gate_dist2[1] = -999;
       }
      if(waveform.num_chips[2] > 0)
	{
	    waveform.gate_dist3[0] = fst_gate_60mhz;
	    waveform.gate_dist3[1] = width;
	}
      else
	{
	    waveform.gate_dist3[0] = -999;
	    waveform.gate_dist3[1] = -999;
	}
      if(waveform.num_chips[3] > 0)
	{
	    waveform.gate_dist4[0] = fst_gate_60mhz;
	    waveform.gate_dist4[1] = width;
	}
      else
	{
	    waveform.gate_dist4[0] = -999;
	    waveform.gate_dist4[1] = -999;
	}
      if(waveform.num_chips[4] > 0)
	{
	    waveform.gate_dist5[0] = fst_gate_60mhz;
	    waveform.gate_dist5[1] = width;
	}
      else
	{
	    waveform.gate_dist5[0] = -999;
	    waveform.gate_dist5[1] = -999;
	}
      waveform_st.gate_dist1[0] = 3;
      number_to_calculate--;
      }

    return(number_to_calculate);

    }/* end calculate_waveform */

