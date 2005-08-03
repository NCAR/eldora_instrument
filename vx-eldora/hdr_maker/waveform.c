#include <stdio.h>
#include <floatingpoint.h>
#include "field.h"

/****************************************************************************/
/*            SUBROUTINE INITIALIZE THE WAVEFORM DESCRIPTOR                */
/***************************************************************************/
   initialize_waveform_descriptor(vheader)
      struct volume_header *vheader;
   {

      int size,i;

          vheader->waveform.waveform_des[0] = 'W';
          vheader->waveform.waveform_des[1] = 'A';
          vheader->waveform.waveform_des[2] = 'V';
          vheader->waveform.waveform_des[3] = 'E';
          size = sizeof(vheader->waveform);
          vheader-> waveform.waveform_des_length = size;
          for(i = 0; i < 15; i++) vheader->waveform.ps_file_name[i] = ' ';
          for(i = 0; i < 6; i++) vheader->waveform.num_chips[i] = -999;
          for(i = 0; i < 256; i++) vheader-> waveform.blank_chip[i] = ' ';
          vheader->waveform.repeat_seq = -999.;
          vheader->waveform.repeat_seq_dwel = -999.;
          vheader->waveform.total_pcp = -999.;
          for(i = 0; i < 6; i++) vheader->waveform.chip_offset[i] = -999;
          for(i = 0; i < 6; i++) vheader->waveform.chip_width[i] = -999;
          vheader->waveform.ur_pcp = -999.;
          vheader->waveform.uv_pcp = -999.;
          for(i = 0; i < 6; i++) vheader->waveform.num_gates[i] = -999;
          for(i = 0; i < 2; i++){
              vheader->waveform.gate_dist1[i] = -999;
              vheader->waveform.gate_dist2[i] = -999;
              vheader->waveform.gate_dist3[i] = -999;
              vheader->waveform.gate_dist4[i] = -999;
              vheader->waveform.gate_dist5[i] = -999;
	  }
    }/*end initialize_waveform_descriptor*/



/**************************************************************************/
/*               SUBROUTINE EDIT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

  edit_waveform_descriptor(vheader)
    struct volume_header *vheader;
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
        printf("7. ENTER THE NUMBER OF 60Mhz CLOCK CYCLES TO WAIT BEFORE                           STARTING A PARTICULAR CHIP IN 60 Mhz COUNTS\n");
        printf("8. ENTER THE NUMBER OF 60Mhz CLOCK CYCLES IN EACH CHIP IN 60
   Mhz UNTS\n");
        printf("9. ENTER THE NUMBER OF PCP THAT SET THE UNAMBIGUOUS RANGE,
   (after real time unfolding)\n");
        printf("10.ENTER THE NUMBER OF PCP THAT SET THE UNAMBIGUOUS VELOCITY,
   (after  real time unfolding\n");
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
                 strcpy(vheader->waveform.ps_file_name,string);                
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
                          vheader->waveform.num_chips[i] = numchip;
                          i++;
		      }
		     } while( i < 6);
                     break;
	    case 3:
                     for(i=0; i<vheader->waveform.total_pcp; i++)
		       {
                        printf("\nBlanking Ram number %d : ",i);
                        gets(input);
			blank = atof(input);
			vheader->waveform.blank_chip[i] = blank;
		    }
                        break;
	     case 4:  
                      printf("ENTER MILLISECONDS:\n");
                      gets(input);
                      nummilliseconds = atof(input);
                      vheader->waveform.repeat_seq = nummilliseconds;
                      break;
             case 5:
                      printf("ENTER DWELL:\n");
                      gets(input);
                      dwell = atoi(input);
                      vheader->waveform.repeat_seq_dwel = dwell;
                      break;
             case 6:
                      printf("ENTER TOTAL PCP IN REPEAT SEQUENCE:\n");
                      gets(input);
                      totalpcp = atoi(input);
                      vheader->waveform.total_pcp = totalpcp;
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
                          vheader->waveform.chip_offset[i] = chipoffset;
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
                          vheader->waveform.chip_width[i] = chipwidth;
                          i++;
		      }
		     } while( i < 6);
                     break;
               case 9:
                      printf("ENTER NUMBER OF PCP:\n");
                      gets(input);
                       urpcp = atof(input);
                      vheader->waveform.ur_pcp = urpcp;
                      break;
               case 10:
                      printf("ENTER NUMBER OF PCP:\n");
                      gets(input);
                      uvpcp = atof(input);
                      vheader->waveform.uv_pcp = uvpcp;
                      break;
               case 11:
                   i = 0;
                   do{
                      printf("NUM GATES  %d ?.  ENTER -99 TO QUIT\n",i + 1);
                      gets(input);
                      numgates = atoi(input);
                      if(numgates == -99){
                         i = 6;
		      }
                      else{
                          vheader->waveform.num_gates[i] = numgates;
                          i++;
		      }
		     } while( i < 6);
                     break;
               case 12:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         vheader->waveform.gate_dist1[i] = gatedist;
		     }
                     break;
               case 13:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         vheader->waveform.gate_dist2[i] = gatedist;
		     }
                     break;
               case 14:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         vheader->waveform.gate_dist3[i] = gatedist;
                     }
                     break;
               case 15:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         vheader->waveform.gate_dist4[i] = gatedist;
                     }
                     break;
                case 16:
                     for(i=0; i < 2; i++){
                         printf("ENTER GATE DIST %d\n",i);
                         gets(input);
                         gatedist = atoi(input);
                         vheader->waveform.gate_dist5[i] = gatedist;
                     }
                     break;
                default:
                     break;
 
        } /*switch*/
    }/*for*/
  }/*end edit waveform descriptor*/



/**************************************************************************/
/*               SUBROUTINE PRINT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

  print_waveform_descriptor(vheader)
    struct volume_header *vheader;

  {
    int i,k;
          printf("*****************WAVEFORM DESCRIPTOR*******************\n");
          printf("WAVEFORM IDENTIFIER        : %s\n",vheader->waveform.
                                                     waveform_des);
          printf("WAVEFORM DESCRIPTOR LENGTH : %d\n",vheader->waveform.
                                                          waveform_des_length);
          printf("PULSING SCHEME FILE NAME   : %s\n",vheader->waveform.
                                                     ps_file_name);
          printf("NUMBER OF CHIPS IN A REPEAT:\n");
          for(i = 0; i< 6; i++) 
             printf(" %d",vheader->waveform.num_chips[i]);
          printf("\nBLANKING RAM SEQUENCE      :\n");
          k = 0;
          do{
             printf("%3d: ",k);      
             for(i=0; i<10; i++)
	       {
		   printf("%2x ",vheader->waveform.blank_chip[k]);
		   k++;
	       }
	     printf("\n");
	 }while(k < vheader->waveform.total_pcp);

          printf("\nNUMBER OF MILLISECONDS PER REPEAT : %f\n",vheader->
                                                       waveform.repeat_seq);
          printf("NUMBER OF REPEAT SEQUENCES IN A DWELL TIME: %d\n",
		 vheader->waveform.repeat_seq_dwel);
          printf("TOTAL NUMBER OF PCP IN A REPEAT SEQUENCE  : %d\n",
                 vheader->waveform.total_pcp);
          printf("CHIP OFFSET                 :\n");
          for(i = 0; i < 6; i++)  
             printf(" %d",vheader->waveform.chip_offset[i]);

          printf("\nCHIP WIDTH\n");
          for(i = 0; i < 6; i++)
             printf(" %d",vheader->waveform.chip_width[i]);

          printf("\nPCP THAT SET THE UNAMBIGIOUS RANGE : %f\n",
                                     vheader->waveform.ur_pcp);
          printf("PCP THAT SET THE UNAMBIGIOUS VELOCITY : %f\n",
                                     vheader->waveform.uv_pcp);
          printf("TOTAL NUMBER OF SAMPLED GATES:\n");
          for(i = 0; i < 6; i++) 
              printf(" %d",vheader->waveform.num_gates[i]);
          printf("GATE DISTANCE 1 : %d %d\n",vheader->waveform.gate_dist1[0],
                                     vheader->waveform.gate_dist1[1]);

          printf("GATE DISTANCE 2 : %d %d\n",vheader->waveform.gate_dist2[0],
                                     vheader->waveform.gate_dist2[1]);

          printf("GATE DISTANCE 3 : %d %d\n",vheader->waveform.gate_dist3[0],
                                     vheader->waveform.gate_dist3[1]);

          printf("GATE DISTANCE 4 : %d %d\n",vheader->waveform.gate_dist4[0],
                                     vheader->waveform.gate_dist4[1]);

          printf("GATE DISTANCE 5 : %d %d\n",vheader->waveform.gate_dist5[0],
                                     vheader->waveform.gate_dist5[1]);
  }/*end print_waveform_descriptor*/

/**************************************************************************/
/*               SUBROUTINE FPRINT WAVEFORM DESCRIPTOR                     */
/**************************************************************************/

  fprint_waveform_descriptor(vheader)
    struct volume_header *vheader;

  {
    int i,k;
    FILE *printer,*popen();

    if((printer = popen("lpr -Pnec","w")) == NULL){
         printf("UNABLE TO ACCESS LASER PRINTER\n");
         return;
     }
        
     fprintf(printer,"***************WAVEFORM DESCRIPTOR*****************\n");
     fprintf(printer,"WAVEFORM IDENTIFIER        : %s\n",
             vheader->waveform.waveform_des);
     fprintf(printer,"WAVEFORM DESCRIPTOR LENGTH : %d\n",
             vheader->waveform.waveform_des_length);
     fprintf(printer,"PULSING SCHEME FILE NAME   : ");
     for(i = 0; i < 16; i++)
        fprintf(printer,"%c",vheader->waveform.ps_file_name[i]);
     fprintf(printer,"\nNUMBER OF CHIPS IN A REPEAT:\n");
     for(i = 0; i< 6; i++) 
        fprintf(printer," %d",vheader->waveform.num_chips[i]);
      fprintf(printer,"\nBLANKING RAM SEQUENCE      :\n");
         k = 0;
          do{
             fprintf(printer,"%3d: ",k);      
             for(i=0; i<10; i++)
	       {
		   fprintf(printer,"%2x ",vheader->waveform.blank_chip[k]);
		   k++;
	       }
	     fprintf(printer,"\n");
	 }while(k < vheader->waveform.total_pcp);

      fprintf(printer,"\nNUMBER OF MILLISECONDS PER REPEAT : %f\n",
              vheader->waveform.repeat_seq);
      fprintf(printer,"NUMBER OF REPEAT SEQUENCES IN A DWELL TIME: %d\n",
		 vheader->waveform.repeat_seq_dwel);
      fprintf(printer,"TOTAL NUMBER OF PCP IN A REPEAT SEQUENCE  : %d\n",
                 vheader->waveform.total_pcp);
      fprintf(printer,"CHIP OFFSET                 :\n");
       for(i = 0; i < 6; i++)  
         fprintf(printer," %d",vheader->waveform.chip_offset[i]);
      fprintf(printer,"\nCHIP WIDTH\n");
      for(i = 0; i < 6; i++)
         fprintf(printer," %d",vheader->waveform.chip_width[i]);
      fprintf(printer,"\nPCP THAT SET THE UNAMBIGIOUS RANGE : %f\n",
                                     vheader->waveform.ur_pcp);
      fprintf(printer,"PCP THAT SET THE UNAMBIGIOUS VELOCITY : %f\n",
                                     vheader->waveform.uv_pcp);
      fprintf(printer,"TOTAL NUMBER OF SAMPLED GATES:\n");
      for(i = 0; i < 6; i++) 
          fprintf(printer," %d",vheader->waveform.num_gates[i]);
      fprintf(printer,"GATE DISTANCE 1 : %d %d\n",vheader->waveform.gate_dist1[0],
                                     vheader->waveform.gate_dist1[1]);

      fprintf(printer,"GATE DISTANCE 2 : %d %d\n",vheader->waveform.gate_dist2[0],
                                     vheader->waveform.gate_dist2[1]);
      fprintf(printer,"GATE DISTANCE 3 : %d %d\n",vheader->waveform.gate_dist3[0],
                                     vheader->waveform.gate_dist3[1]);
      fprintf(printer,"GATE DISTANCE 4 : %d %d\n",vheader->waveform.gate_dist4[0],
                                     vheader->waveform.gate_dist4[1]);
      fprintf(printer,"GATE DISTANCE 5 : %d %d\n",vheader->waveform.gate_dist5[0],
                                     vheader->waveform.gate_dist5[1]);
      close(printer);
  }/*end print_waveform_descriptor*/

