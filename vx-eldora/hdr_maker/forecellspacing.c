#include <stdio.h>
#include "field.h"

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE CELL DESCRIPTOR                        */
/*****************************************************************************/
     initialize_forecellspacing_descriptor(foresensordescriptor)
    struct sensorDes *foresensordescriptor;
{
   int size,i;


      foresensordescriptor->cell.cell_spacing_des[0] = 'C';
      foresensordescriptor->cell.cell_spacing_des[1] = 'S';
      foresensordescriptor->cell.cell_spacing_des[2] = 'P';    
      foresensordescriptor->cell.cell_spacing_des[3] = 'D';
      size = sizeof(foresensordescriptor->cell);
      foresensordescriptor->cell.cell_spacing_des_len = size;
      foresensordescriptor->cell.num_segments = -999;
      foresensordescriptor->cell.distToFirst = -999;
      for(i = 0; i < 6; i++) foresensordescriptor->cell.spacing[i] = -999;
      for(i = 0; i < 6; i++) foresensordescriptor->cell.num_cells[i] = -999;
}/*end initialize cell descriptor*/


/****************************************************************************/
/*              SUBROUTINE EDIT CELL SPACING  DESCRIPTOR                    */
/****************************************************************************/
      edit_forecellspacing_descriptor(foresensordescriptor)
    struct sensorDes *foresensordescriptor;
      
   {

    int choice,i;
    short numsegs,dist,spacing,numcell;
    char input[80];

       for(;;){
          do{
            printf("0. RETURN TO MAIN MENU\n");
            printf("1. ENTER NUMBER OF SEQMENTS\n");
            printf("2. ENTER DISTANCE TO FIRST GATE\n");
            printf("3. ENTER WIDTH OF CELL IN EACH SEGMENT\n");
            printf("4. ENTER NUMBER OF CELLS IN EACH SEGMENT\n");
            gets(input);
            choice = atoi(input);
            if(choice <0 || choice > 4) printf("INVALID ENTRY\n");
	} while(choice <0 || choice > 4);
        
     switch(choice){
          case 0: 
                return;
                break;
          case 1:
                 printf("ENTER NUMBER OF SEGMENTS\n");
                 gets(input);
                 numsegs = atoi(input);
                 foresensordescriptor->cell.num_segments = numsegs;
                 break;
          case 2:     
                printf("ENTER DISTTANCE\n");
                gets(input);
                dist = atoi(input);
                foresensordescriptor->cell.distToFirst = dist;
                break;
	  case 3:
                i = 0;
                 do{
                    printf("SPACING %d ?.  ENTER -99 TO QUIT\n",i + 1);
                    gets(input);
                    spacing = atoi(input);
                    if(spacing == -99){
                         i = 6;
		     }
                     else{
                          foresensordescriptor->cell.spacing[i] = spacing;
                          i++;
		      }
		  } while( i < 6);
                  break;
           case 4:
                 i = 0;
                 do{
                    printf("NUMBER OF CELLS IN SEGMENT  %d ?.  ENTER -99 TO QUIT\n",i + 1);
                    gets(input);
                    numcell = atoi(input);
                    if(numcell == -99){
                        i = 6;
		    }
                    else{
                        foresensordescriptor->cell.num_cells[i] = numcell;
                        i++;
		    }
		 } while( i < 6);
                  break;
	    }/*switch*/
      }/*for*/
}/*end edit cell*/


/****************************************************************************/
/*                 SUBROUTINE PRINT CELL SPACING DESCRIPTOR                 */
/***************************************************************************/
   print_forecellspacing_descriptor(foresensordescriptor)
      struct sensorDes *foresensordescriptor;    
    
  {
    int i;

     printf("CELL SPACING DESCRIPTOR        : %s\n",
                          foresensordescriptor->cell.cell_spacing_des);
     printf("CELL SPACING DESCRIPTOR LENGTH : %d\n",
                          foresensordescriptor->cell.cell_spacing_des_len);
     printf("NUMBER OF SEQMENTS THAT CONTAIN CELLS OF EQUAL WIDTHS : %d\n",
                          foresensordescriptor->cell.num_segments);
     printf("DISTANCE FOR FIRST GATE IN METERS                     : %d\n",
                          foresensordescriptor->cell.distToFirst);
     printf("WIDTH OF CELLS IN EACH SEGMENT IN METERS              :\n");
     for(i = 0; i < 6; i++)
        printf(" %d",foresensordescriptor->cell.spacing[i]);

     printf("\nNUMBER OF CELL IN EACH SEGMENT\n");
     for(i = 0; i < 6; i++)
        printf(" %d",foresensordescriptor->cell.num_cells[i]);
 }/*end print_cellspacing_descriptor*/                          


/****************************************************************************/
/*   SUBROUTINE TO PRINT CELL SPACING DESCRIPTOR TO LASER PRINTER           */
/***************************************************************************/
   fprint_forecellspacing_descriptor(foresensordescriptor)
      struct sensorDes *foresensordescriptor;    
    
  {
    int i;
    FILE *printer,*popen();

    if((printer = popen("lpr -Pnec","w")) == NULL){
       printf("UNABLE TO ACCESS LASER PRINTER\n");
       return;
   }


    fprintf(printer,"CELL SPACING DESCRIPTOR        : %s\n",
                          foresensordescriptor->cell.cell_spacing_des);
    fprintf(printer,"CELL SPACING DESCRIPTOR LENGTH : %d\n",
                          foresensordescriptor->cell.cell_spacing_des_len);
    fprintf(printer,"NUMBER OF SEQMENTS THAT CONTAIN CELLS OF EQUAL WIDTHS : %d\n",
                          foresensordescriptor->cell.num_segments);
     fprintf(printer,"PRINTER,DISTANCE FOR FIRST GATE(m)    : %d\n",
                          foresensordescriptor->cell.distToFirst);
     fprintf(printer,"WIDTH OF CELLS IN EACH SEGMENT IN METERS              :\n");
     for(i = 0; i < 6; i++)
        fprintf(printer," %d",foresensordescriptor->cell.spacing[i]);

     fprintf(printer,"\nNUMBER OF CELL IN EACH SEGMENT\n");
     for(i = 0; i < 6; i++)
        fprintf(printer," %d",foresensordescriptor->cell.num_cells[i]);
     close(printer);
 }/*end print_cellspacing_descriptor*/                          



