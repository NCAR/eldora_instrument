/************************************************************************/
/*          Function foreaft gets an F or an A from the user            */
/*                 and returns either FORE or AFT                       */
/************************************************************************/
int foreaft()
{
    char input[80];

    for(;;)
      {

	  printf("Enter F For the fore radar ");
	  printf(" or enter A For the aft radar: ");
	  fgets(input, sizeof(input), stdin);
	  printf("\n");
	  if(input[0] == 'F' || input[0] == 'f')
	    return(FORE);
	  else if(input[0] == 'A' || input[0] == 'a')
	    return(AFT);
	  else
	    printf("invalid entry\n");
      }
}

/************************************************************************/
/*          Function param_num returns the number of the                */
/*            parameter selected by the user                            */
/************************************************************************/

int specific_param(radarnum)
  int radarnum;
    {
    char input[80];
    int num, i;
    
	for(i=0; i< actual_num_params[radarnum]; i++)
	  printf("%2d) is called: %c%c%c%c%c%c%c%c\n",i,
		 parameter[i][radarnum].parameter_name[0],
		 parameter[i][radarnum].parameter_name[1],
		 parameter[i][radarnum].parameter_name[2],
		 parameter[i][radarnum].parameter_name[3],
		 parameter[i][radarnum].parameter_name[4],
		 parameter[i][radarnum].parameter_name[5],
		 parameter[i][radarnum].parameter_name[6],
		 parameter[i][radarnum].parameter_name[7]
		 );
    do{
	printf("Maximum number of paramaters is %d\n",MAX_PARAM);
	printf("Enter the desired parameter number or %d to add a parameter",
	       actual_num_params[radarnum]);
	fgets(input, sizeof(input), stdin);
	num = atoi(input);
	if(num > actual_num_params[radarnum] || num < 0)
	  printf("\nParameter number must be: >=0 and <= %d",
		 actual_num_params[radarnum]);
    }while(num > actual_num_params[radarnum] || num < 0);
    if(num == actual_num_params[radarnum] && num < MAX_PARAM)
      {
	  actual_num_params[radarnum]++;
	  initialize_parameter(&parameter[num][radarnum]);
      }
    return(num);
}


/****************************************************************************/
/*                   SUBROUTINE PRINT MENU PRINTER                          */
/****************************************************************************/
 void print_menu_printer()
{
    int choice, i, j, radarnum, paramnum;
    char input[80];
    FILE *printer, *popen();

    if(( printer = popen("lp","w")) == NULL){
         printf("UNABLE TO ACCESS LASER PRINTER\n");
         return;
     }

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
        printf("7. PRINT NAVIGATIONAL DATA DESCRIPTOR\n");
        printf("8. PRINT ALL AT ONCE\n");
        printf("ENTER YOUR CHOICE\n");
        fgets(input, sizeof(input), stdin);
        choice = atoi(input);
        if(choice < 0 || choice > 8) printf("INVALID ENTRY\n");
    }while(choice < 0 || choice > 8);

    switch(choice){
	   case 0:
	        pclose(printer);
	        return;
		break;
           case 1: 
                fprint_volume(&volume,printer);
	        fprintf(printer,"\n\n");
                break;
           case 2:
                fprint_waveform(&waveform,printer);
	        fprintf(printer,"\n\n");
                break;
           case 3:
                radarnum = foreaft();
                fprint_radar(&radar[radarnum],printer);
	        fprintf(printer,"\n\n");
                break;
           case 4:
                radarnum = foreaft();
                fprint_fieldradar(&fradar[radarnum],printer);
	        fprintf(printer,"\n\n");
                break;
           case 5: 
                radarnum = foreaft();
                fprint_cellspacing(&cell[radarnum],printer);
	        fprintf(printer,"\n\n");
                break;
           case 6:
	        radarnum = foreaft();
                paramnum = specific_param(radarnum);
                fprint_parameter(&parameter[paramnum][radarnum],printer);
	        fprintf(printer,"\n\n");
                break;
           case 7: 
                fprint_navigation(&navigation,printer);
	        fprintf(printer,"\n\n");
                break;
           default:
                break;
	   case 8:
	     fprint_volume(&volume,printer);
	     fprintf(printer,"\n\n");
	     fprint_waveform(&waveform,printer);
	     fprintf(printer,"\n\n");
	     for(i=0; i < actual_num_radars; i++)
	       {
		   fprint_radar(&radar[i],printer);
		   fprintf(printer,"\n\n");
		   fprint_fieldradar(&fradar[i],printer);
		   fprintf(printer,"\n\n");
		   fprint_cellspacing(&cell[i],printer);
		   fprintf(printer,"\n\n");
		   for(j=0; j<actual_num_params[i]; j++)
		     {
			 fprint_parameter(&parameter[j][i],printer);
			 fprintf(printer,"\n\n");
		     }
	       }
            fprint_navigation(&navigation,printer);
	    fprintf(printer,"\n\n");
            break;

	}/*switch*/
   }/*for*/
}/*end print_menu_printer*/
