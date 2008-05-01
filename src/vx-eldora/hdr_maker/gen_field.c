#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/uio.h>
#include "field.h"

main()
{  
  
  int bytesread,ios,sweep,radarsize,cellsize,paramsize,fldradsize;
  int navsize,insitusize;
  int byteswritten,recordnum,another;
  int isclose,eotptr,index,i,j,k,local;
  float lat[3],lon[3];
  extern int errno;
  extern char *sys_errlist[];
  char inname[80],outname[80];
  int size,choice,ansr;
  static int numparameters;
  FILE *outfdf;


   for(;;){
     menu(&choice);
     switch(choice){
        case 0: 
             close(outfdf);
             exit();
             break;
	case 1:
            initialize_menu(&vheader,&foresensordescriptor,
                            &aftsensordescriptor,&navagationdesc,
                            &insitudescriptor,&numparameters);
            break;
	case 2:
            edit_menu(&vheader,&foresensordescriptor,
                            &aftsensordescriptor,
                            &navagationdesc,&parameter);
            break;
	case 3:
            printf("OUTPUT FILE NAME ?\n");
            gets(outname);
            printf("OUTPUT FILE NAME: %s\n",outname);
            if((outfdf = fopen(outname,"wb")) == NULL){
                printf("UNABLE TO OPEN THE OUTPUT  FILE\n");
             }
            size = sizeof(vheader);
            byteswritten = fwrite(&vheader,size,1,outfdf);
            /*WRITE OUT SENSORDESCRIPTORS FOR EACH RADAR*/
            radarsize =  sizeof(radar);
            fldradsize = sizeof(fradar);
            cellsize   = sizeof(cell);
            paramsize  = (sizeof(parameter)/MAXPARAMETERS)*numparameters;
            size = radarsize + fldradsize + cellsize + paramsize;
            /*FORWARD*/
            byteswritten = fwrite(&foresensordescriptor,size,1,outfdf);
            /*AFT*/
            byteswritten = fwrite(&aftsensordescriptor,size,1,outfdf);
            navsize      = sizeof(navagation);
            byteswritten = fwrite(&navagationdesc,navsize,1,outfdf);
            insitusize   = sizeof(insitudata);
            byteswritten = fwrite(&insitudescriptor,insitusize,1,outfdf);
            break;
	 case 4:
           read_header(&vheader,&foresensordescriptor,&aftsensordescriptor,
                                &navagationdesc,&numparameters);
           print_volume_descriptor(&vheader);
           printf("\n");
           sleep(2);
           print_waveform_descriptor(&vheader);
           printf("\n");
           sleep(2);
           print_foreradar_descriptor(&foresensordescriptor);
           printf("\n");
           sleep(2);
           print_aftradar_descriptor(&aftsensordescriptor);
           printf("\n");
           sleep(2);
           print_forecellspacing_descriptor(&foresensordescriptor);
           printf("\n");
           sleep(2);
           print_aftcellspacing_descriptor(&aftsensordescriptor);
           printf("\n");
           sleep(2);
           for(i = 0; i < numparameters; i++){ 
             print_foreparameter_descriptor(&foresensordescriptor,&i);
             printf("\n");
             print_aftparameter_descriptor(&aftsensordescriptor,&i);
             sleep(2);
	   }
           printf("\n");
           print_navagational_descriptor(&navagationdesc);
           break;
         case 5:
           print_menu_screen(&vheader,&foresensordescriptor,
                                      &aftsensordescriptor,
                                      &navagationdesc);
           break;  
	 case 6:
           print_menu_printer(&vheader,&foresensordescriptor,
                                       &aftsensordescriptor,
                                       &navagationdesc); 
           break;
	 case 7:
           calculate_parameters(&vheader);
           print_waveform_descriptor(&vheader);
           fix_velocity_scale_factor(&foresensordescriptor,&numparameters);
           fix_cellspacing(&foresensordescriptor);
           edit_aftcellspacing_descriptor(foresensordescriptor,
                                               aftsensordescriptor);
           print_forecellspacing_descriptor(&foresensordescriptor);
           printf("\n");
           print_aftcellspacing_descriptor(&aftsensordescriptor);
           break;
        default:
           break;   
	 }/*switch*/
    }/*for*/

}/*end main*/



/**************************************************************************/
/*                            MAIN MENU                                   */
/**************************************************************************/
  menu(choice)
  int *choice;
{
  int ansr;
  char string[80];
       do {
          printf("0. EXIT THIS PROGRAM\n");
          printf("1. INITIALIZE MENU\n");
          printf("2. EDIT MENU\n");
          printf("3. WRITE THE VOLUME HEADER TO A FILE\n");
          printf("4. READ IN A HEADER FROM A FILE\n");
          printf("5. PRINT DESCRIPTORS TO SCREEN MENU\n");
          printf("6. SEND DESCRIPTORS TO LASER PRINTER\n");
          printf("7. INPUT SPECIFIC PARAMETERS\n");
          printf("ENTER YOUR CHOICE\n");
          gets(string);
          ansr  = atoi(string);
          if(ansr < 0 || ansr > 7) printf("INVALID ENTRY\n");
       } while(ansr < 0 || ansr > 7);
       *choice = ansr;
}

/*************************************************************************/
/*                     ITITIALIZE MENU                                   */ 
/*************************************************************************/

  initialize_menu(vheader,foresensordescriptor,aftsensordescriptor,
                   navagationdesc,insitudescriptor,numparameters)
    struct volume_header *vheader;
    struct sensorDes *foresensordescriptor,*aftsensordescriptor;
    struct navaDesc  *navagationdesc;
    struct inSituDataDesc  *insitudescriptor;
    int *numparameters;

  {
    int choice,i;
    int loopindex;
    int number_of_parameters,num;
    char input[80];
     for(;;){
        do{
          printf("\n***************ITITIALIZE MENU*******************\n");
          printf("0. RETURN TO MAIN MENU\n");
          printf("1. INITIALIZE THE VOLUME DESCRIPTOR\n");
          printf("2. INITIALIZE THE WAVEFORM DESCRIPTOR\n");
          printf("3. INITIALIZE THE FORE AND AFT RADAR DESCRIPTORS\n");
          printf("4. INITIALIZE THE CELL SPACING  DESCRIPTOR\n");
          printf("5. INITTIALIZE THE FIELD RADAR DESCRIPTOR\n");
          printf("6. INITIALIZE THE PARAMETER DESCRIPTORS\n");
          printf("7. INITIALIZE THE NAVAGATIONAL DATA DESCRIPTOR\n");
          printf("8. INITIALIZE THE INSITU DATA DESCRIPTOR\n");
          printf("9. INITIALIZE ALL OF THE ABOVE AT ONCE\n");
          printf("ENTER YOUR CHOICE\n");
          gets(input);
          choice = atoi(input);
          if(choice < 0 || choice > 9) printf("INVALID ENTRY\n");
        } while(choice < 0 || choice > 9);
        switch (choice){
	   case 0: return;
                break;
           case 1: 
                initialize_volume_descriptor(vheader);
                print_volume_descriptor(vheader);
                break;
           case 2:
                initialize_waveform_descriptor(vheader);
                print_waveform_descriptor(vheader);
                break;
           case 3:
                  printf("ENTER F FOR THE FORWARD RADAR DESCRIPTOR\n");
                  printf("ENTER A FOR THE AFT RADAR DESCRIPTOR\n");
                  gets(input);
                  if(input[0] == 'F' || input[0] == 'f'){
                         initialize_foreradar_descriptor(foresensordescriptor);
                         print_foreradar_descriptor(foresensordescriptor);
		  }
                  else if(input[0] == 'A' || input[0] == 'a'){
                         initialize_aftradar_descriptor(aftsensordescriptor);
                         print_aftradar_descriptor(aftsensordescriptor);
		  }
                  else{
                         printf("invalid entry\n");
         	  }
                break;
           case 4:
                initialize_forecellspacing_descriptor(foresensordescriptor);
                printf("FORWARD RADAR\n");
                print_forecellspacing_descriptor(foresensordescriptor);
                printf("\n");
                printf("AFT RADAR\n");
                initialize_aftcellspacing_descriptor(aftsensordescriptor);
                print_aftcellspacing_descriptor(aftsensordescriptor);
                break;
           case 5: 
                initialize_forefieldradar_info(foresensordescriptor);
                print_forefield_radar_descriptor(foresensordescriptor);
                printf("\n");
                initialize_aftfieldradar_info(aftsensordescriptor);
                print_aftfield_radar_descriptor(aftsensordescriptor);
                break;
           case 6:
                do{
                  printf("THE MAXIMUM NUMBER OF PARAMETES ALLOWED IS 10\n");
                  printf("HOW MANY PARAMETERS DO YOU HAVE ?\n");
                  gets(input);
                  number_of_parameters = atoi(input);
                  if(number_of_parameters >10) printf("TOO MANY PARAMETERS\n");
	       }while(number_of_parameters >10);
                for(i = 0; i < number_of_parameters; i++){
                   num = i;
                   initialize_foreparameter_descriptor(foresensordescriptor,
                                                       &num);
                   print_foreparameter_descriptor(foresensordescriptor,&num);

                   initialize_aftparameter_descriptor(aftsensordescriptor,
                                                       &num);
                   print_aftparameter_descriptor(foresensordescriptor,&num);
	        }
                break;
           case 7: 
                initialize_navagational_descriptor(navagationdesc);
                print_navagational_descriptor(navagationdesc);
                break;
           case 8:
                initialize_insitudata_descriptor(insitudescriptor);
                print_insitudata_descriptor(insitudescriptor);
                break;                
           case 9:
                initialize_volume_descriptor(vheader);
                initialize_waveform_descriptor(vheader);
                initialize_foreradar_descriptor(foresensordescriptor);
                initialize_aftradar_descriptor(aftsensordescriptor);
                initialize_forefieldradar_info(foresensordescriptor);
                initialize_aftfieldradar_info(aftsensordescriptor);
                initialize_forecellspacing_descriptor(foresensordescriptor);
                initialize_aftcellspacing_descriptor(aftsensordescriptor);
                do{
                  printf("THE MAXIMUM NUMBER OF PARAMETES ALLOWED IS 10\n");
                  printf("HOW MANY PARAMETERS DO YOU HAVE ?\n");
                  gets(input);
                  number_of_parameters = atoi(input);
                  *numparameters = number_of_parameters;
                  if(number_of_parameters >10) printf("TOO MANY PARAMETERS\n");
	       }while(number_of_parameters >10);
                for(i = 0; i < number_of_parameters; i++){
                   num = i;
                   initialize_foreparameter_descriptor(foresensordescriptor,
                                                       &num);
                   initialize_aftparameter_descriptor(aftsensordescriptor,
                                                       &num);
	        }
                initialize_navagational_descriptor(navagationdesc);
                initialize_insitudata_descriptor(insitudescriptor);
                break;
           default:
                break;
      }/*switch*/

    }/*for*/

 }/*end initialize menu*/


/************************************************************************/
/*              SUBROUTINE EDIT MENU                                   */
/***********************************************************************/
    edit_menu(vheader,foresensordescriptor,aftsensordescriptor,
              navagationdesc)
    struct volume_header *vheader;
    struct sensorDes *foresensordescriptor,*aftsensordescriptor;
    struct navDesc   *navagationdesc;
{
    int choice;
    int paramnum;
    char input[80];
     for(;;){
        do{
          printf("\n***************EDIT MENU*******************\n");
          printf("0. RETURN TO MAIN MENU\n");
          printf("1. EDIT THE VOLUME DESCRIPTOR\n");
          printf("2. EDIT THE WAVEFORM DESCRIPTOR\n");
          printf("3. EDIT THE FORE AND  AFT RADAR DESCRIPTORS\n");
          printf("4. EDIT THE FORE AND AFT FIELD RADAR INFORMATION DESC\n");
          printf("5. EDIT THE FORE AND AFT CELL SPACING  DESCRIPTORS\n");
          printf("6. EDIT THE PARAMETER DESCRIPTORS\n");
          printf("7. SET THE NAVAGATIONAL DATA DESCRIPTOR FLAGS\n");
          printf("ENTER YOUR CHOICE\n");
          gets(input);
          choice = atoi(input);
          if(choice < 0 || choice > 7) printf("INVALID ENTRY\n");
        } while(choice < 0 || choice > 7);
        switch (choice){
	   case 0: return;
                break;
           case 1: 
                edit_volume_descriptor(vheader);
                print_volume_descriptor(vheader);
                break;
           case 2:
                edit_waveform_descriptor(vheader);
                print_waveform_descriptor(vheader);
                break;
           case 3:
                printf("ENTER F FOR FORWARD RADAR\n");
                printf("ENTER A FOR AFT RADAR\n");
                gets(input);
                if(input[0] == 'A' || input[0] == 'a'){
                     printf("AFT RADAR\n");
                     edit_aftradar_descriptor(aftsensordescriptor);
                     print_aftradar_descriptor(aftsensordescriptor);
                }
                else if(input[0] == 'F' || input[0] == 'f'){
                     printf("FORWARD RADAR\n");
                     edit_foreradar_descriptor(foresensordescriptor);
                     print_foreradar_descriptor(foresensordescriptor);
                }
                else{
                     printf("INVALID ENTRY\n");
	        }
                break;
           case 4: 
                printf("ENTER F FOR FORWARD RADAR\n");
                printf("ENTER A FOR AFT RADAR\n");
                gets(input);
                if(input[0] == 'A' || input[0] == 'a'){
                     printf("AFT RADAR\n");
                     edit_aftfield_radar_info(aftsensordescriptor);
                     print_aftfield_radar_descriptor(aftsensordescriptor);
                }
                else if(input[0] == 'F' || input[0] == 'f'){
                     printf("FORWARD RADAR\n");
                     edit_forefield_radar_info(foresensordescriptor);
                     print_forefield_radar_descriptor(foresensordescriptor);
		 }
                 break;
           case 5:
                edit_forecellspacing_descriptor(foresensordescriptor);
                printf("FOREWARD RADAR\n");
                print_forecellspacing_descriptor(foresensordescriptor);
                printf("\n");
                printf("AFT RADAR\n");
                edit_aftcellspacing_descriptor(foresensordescriptor,
                                               aftsensordescriptor);
                print_aftcellspacing_descriptor(aftsensordescriptor);
                break;
           case 6:
                printf("WHICH PARAMETER NUMBER DO YOU WISH TO EDIT\n");
                gets(input);
                paramnum = atoi(input);
                printf("EDITING PARAMETER NUMBER %d\n",paramnum);
                paramnum = paramnum - 1;
                edit_foreparameter_descriptor(foresensordescriptor,&paramnum);
                print_foreparameter_descriptor(foresensordescriptor,&paramnum);
                printf("\n");
                edit_aftparameter_descriptor(aftsensordescriptor,
                                             foresensordescriptor,&paramnum);
                print_aftparameter_descriptor(aftsensordescriptor,&paramnum);
                break;
           case 7:
                edit_navagational_descriptor(navagationdesc);
                print_navagational_descriptor(navagationdesc);
                break;
           default:
                break;
      }/*switch*/

    }/*for*/

 }/*end edit_menu*/

/****************************************************************************/
/*                 SUBROUTINE PRINT TO SCREEN MENU                           */
/****************************************************************************/

 print_menu_screen(vheader,foresensordescriptor,aftsensordescriptor,
                   navagationdesc)
    struct volume_header *vheader;
    struct sensorDes *foresensordescriptor,*aftsensordescriptor;
    struct navaDesc  *navagationdesc;
{
    int choice,num,i;
    char input[80];
   
    for(;;){ 
      do{
        printf("********PRINT DESCRIPTORS MENU************\n");
        printf("0. EXIT PRINT MENU\n");
        printf("1. PRINT VOLUME DESCRIPTOR\n");
        printf("2. PRINT WAVEFORM DESCRIPTOR\n");
        printf("3. PRINT RADAR DESCRIPTOR\n");
        printf("4. PRINT FIELD RADAR DESCRIPTOR\n");
        printf("5. PRINT CELL SPACING DESCRIPTOR\n");
        printf("6. PRINT PARAMETER DESCRIPTORS\n");
        printf("7. PRINT NAVAGATIONAL DATA DESCRIPTOR\n");
        printf("8. PRINT ALL AT ONCE\n");
        printf("ENTER YOUR CHOICE\n");
        gets(input);
        choice = atoi(input);
        if(choice < 0 || choice > 7) printf("INVALID ENTRY\n");
    }while(choice < 0 || choice > 7);

    switch(choice){
        case 0:
            return;
            break;
        case 1:
            print_volume_descriptor(vheader);
            break;
	case 2:
            print_waveform_descriptor(vheader);
            break;
        case 3:
            print_foreradar_descriptor(foresensordescriptor);
            printf("\n");
            print_aftradar_descriptor(aftsensordescriptor);
            break;
        case 4:
            print_forefield_radar_descriptor(foresensordescriptor);
            sleep(5);
            printf("\n");
            print_aftfield_radar_descriptor(aftsensordescriptor);
            break;
        case 5:
            printf("FORWARD RADAR\n");
            print_forecellspacing_descriptor(foresensordescriptor);
            printf("\n");
            printf("AFT RADAR\n");
            print_aftcellspacing_descriptor(aftsensordescriptor);
            break;
        case 6:
            printf("WHICH PARAMETER DESCRIPTOR DO YOU WANT\n");
            gets(input);
            num = atoi(input);
            num = num - 1;
            printf("FORWARD RADAR\n");
            print_foreparameter_descriptor(foresensordescriptor,&num);
            printf("\n");
            printf("AFT RADAR\n");
            print_aftparameter_descriptor(aftsensordescriptor,&num);
            break;
        case 7:
            print_navagational_descriptor(navagationdesc);
            break;
	 case 8:
            print_volume_descriptor(vheader);
            print_waveform_descriptor(vheader);
            print_foreradar_descriptor(foresensordescriptor);
            print_aftradar_descriptor(aftsensordescriptor);
            printf("FORWARD RADAR\n");
            print_forecellspacing_descriptor(foresensordescriptor);
            printf("\n");
            printf("AFT RADAR\n");
            print_aftcellspacing_descriptor(foresensordescriptor);
            printf("\nHOW MAN PARAMETERS DO YOU HAVE\n");
            gets(input);
            num = atoi(input);
            printf("FORWARD RADAR\n");
            for(i = 0; i < num; i++){
                print_foreparameter_descriptor(foresensordescriptor,&i);
            }
            printf("\n");
            printf("AFT RADAR\n");
            for(i = 0; i < num; i++){
                print_aftparameter_descriptor(aftsensordescriptor,&i);
	    }
            print_navagational_descriptor(navagationdesc);
            break;        
        default:
            break;
	}/*switch*/
   }/*for*/
}/*end print_menu*/



/****************************************************************************/
/*                   SUBROUTINE PRINT MENU PRINTER                          */
/****************************************************************************/
 print_menu_printer(vheader,foresensordescriptor,aftsensordescriptor,
                    navagationdesc)

    struct volume_header *vheader;
    struct sensorDes *foresensordescriptor,*aftsensordescriptor;
    struct navaDesc  *navagationdesc;
{
    int choice,num,i;
    char input[80];

    printf("DESCRIPTORS WILL BE PRINTED ON THOR'S LASER PRINTER\n");
   
    for(;;){ 
      do{
        printf("********PRINT DESCRIPTORS MENU************\n");
        printf("0. EXIT PRINT MENU\n");
        printf("1. PRINT VOLUME DESCRIPTOR\n");
        printf("2. PRINT WAVEFORM DESCRIPTOR\n");
        printf("3. PRINT RADAR DESCRIPTOR\n");
        printf("4. PRINT FIELD RADAR DESCRIPTOR\n");
        printf("5. PRINT CELL SPACING DESCRIPTOR\n");
        printf("6. PRINT PARAMETER DESCRIPTORS\n");
        printf("7. PRINT NAVAGATIONAL DATA DESCRIPTOR\n");
        printf("8. PRINT ALL AT ONCE\n");
        printf("ENTER YOUR CHOICE\n");
        gets(input);
        choice = atoi(input);
        if(choice < 0 || choice > 8) printf("INVALID ENTRY\n");
    }while(choice < 0 || choice > 8);

    switch(choice){
        case 0:
            return;
            break;
        case 1:
            fprint_volume_descriptor(vheader);
            break;
	case 2:
            fprint_waveform_descriptor(vheader);
            break;
        case 3:
            fprint_foreradar_descriptor(foresensordescriptor);
            fprint_aftradar_descriptor(aftsensordescriptor);
            break;
	case 4:
            fprint_forefield_radar_descriptor(foresensordescriptor);
            fprint_aftfield_radar_descriptor(aftsensordescriptor);
            break;
        case 5:
            fprint_forecellspacing_descriptor(foresensordescriptor);
            fprint_aftcellspacing_descriptor(aftsensordescriptor);
            break;
        case 6:
            printf("WHICH PARAMETER DESCRIPTOR DO YOU WANT\n");
            gets(input);
            num = atoi(input);
            num = num - 1;
            fprint_foreparameter_descriptor(foresensordescriptor,&num);
            fprint_aftparameter_descriptor(aftsensordescriptor,&num);
            break;
        case 7:
            fprint_navagational_descriptor(navagationdesc);
            break;
	 case 8:
            fprint_volume_descriptor(vheader);
            fprint_waveform_descriptor(vheader);
            fprint_foreradar_descriptor(foresensordescriptor);
            fprint_aftradar_descriptor(aftsensordescriptor);
            fprint_forecellspacing_descriptor(foresensordescriptor);
            printf("\n");
            fprint_aftcellspacing_descriptor(aftsensordescriptor);      
            printf("\nHOW MAN PARAMETERS DO YOU HAVE\n");
            gets(input);
            num = atoi(input);
            for(i = 0; i < num; i++)
                fprint_foreparameter_descriptor(foresensordescriptor,&i);

            for(i = 0; i < num; i++)
                fprint_aftparameter_descriptor(aftsensordescriptor,&i);
            break;        
        default:
            break;
	}/*switch*/
   }/*for*/
}/*end print_menu_printer*/


/****************************************************************************/






