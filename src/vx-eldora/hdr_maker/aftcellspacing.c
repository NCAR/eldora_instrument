#include <stdio.h>
#include "field.h"

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE CELL DESCRIPTOR                        */
/*****************************************************************************/
     initialize_aftcellspacing_descriptor(aftsensordescriptor)
    struct sensorDes *aftsensordescriptor;
{
   int size,i;


      aftsensordescriptor->cell.cell_spacing_des[0] = 'C';
      aftsensordescriptor->cell.cell_spacing_des[1] = 'S';
      aftsensordescriptor->cell.cell_spacing_des[2] = 'P';    
      aftsensordescriptor->cell.cell_spacing_des[3] = 'D';
      size = sizeof(aftsensordescriptor->cell);
      aftsensordescriptor->cell.cell_spacing_des_len = size;
      aftsensordescriptor->cell.num_segments = -999;
      aftsensordescriptor->cell.distToFirst = -999;
      for(i = 0; i < 6; i++) aftsensordescriptor->cell.spacing[i] = -999;
      for(i = 0; i < 6; i++) aftsensordescriptor->cell.num_cells[i] = -999;
}/*end initialize cell descriptor*/


/****************************************************************************/
/*              SUBROUTINE EDIT CELL SPACING  DESCRIPTOR                    */
/****************************************************************************/
      edit_aftcellspacing_descriptor(foresensordescriptor,aftsensordescriptor)
    struct sensorDes *foresensordescriptor;
    struct sensorDes *aftsensordescriptor;
      
   {

    int choice,i;
    short numsegs,dist,spacing,numcell;
    char input[80];
    int  size;

      aftsensordescriptor->cell.cell_spacing_des[0] = 'C';
      aftsensordescriptor->cell.cell_spacing_des[1] = 'S';
      aftsensordescriptor->cell.cell_spacing_des[2] = 'P';
      aftsensordescriptor->cell.cell_spacing_des[3] = 'D';
      size = sizeof(aftsensordescriptor->cell);
      aftsensordescriptor->cell.cell_spacing_des_len = size;
      aftsensordescriptor->cell.num_segments = 
                           foresensordescriptor->cell.num_segments;
      aftsensordescriptor->cell.distToFirst = 
                           foresensordescriptor->cell.distToFirst; 
      for(i = 0; i < 6; i++) aftsensordescriptor->cell.spacing[i] =
                             foresensordescriptor->cell.spacing[i];
      for(i = 0; i < 6; i++) aftsensordescriptor->cell.num_cells[i] = 
                             foresensordescriptor->cell.num_cells[i];

}/*end edit cell*/


/****************************************************************************/
/*                 SUBROUTINE PRINT CELL SPACING DESCRIPTOR                 */
/***************************************************************************/
   print_aftcellspacing_descriptor(aftsensordescriptor)
      struct sensorDes *aftsensordescriptor;    
    
  {
    int i;

     printf("CELL SPACING DESCRIPTOR        : %s\n",
                          aftsensordescriptor->cell.cell_spacing_des);
     printf("CELL SPACING DESCRIPTOR LENGTH : %d\n",
                          aftsensordescriptor->cell.cell_spacing_des_len);
     printf("NUMBER OF SEQMENTS THAT CONTAIN CELLS OF EQUAL WIDTHS : %d\n",
                          aftsensordescriptor->cell.num_segments);
     printf("DISTANCE FOR FIRST GATE IN METERS                     : %d\n",
                          aftsensordescriptor->cell.distToFirst);
     printf("WIDTH OF CELLS IN EACH SEGMENT IN METERS              :\n");
     for(i = 0; i < 6; i++)
        printf(" %d",aftsensordescriptor->cell.spacing[i]);

     printf("\nNUMBER OF CELL IN EACH SEGMENT\n");
     for(i = 0; i < 6; i++)
        printf(" %d",aftsensordescriptor->cell.num_cells[i]);
 }/*end print_cellspacing_descriptor*/                          


/****************************************************************************/
/*   SUBROUTINE TO PRINT CELL SPACING DESCRIPTOR TO LASER PRINTER           */
/***************************************************************************/
   fprint_aftcellspacing_descriptor(aftsensordescriptor)
      struct sensorDes *aftsensordescriptor;    
    
  {
    int i;
    FILE *printer,*popen();

    if((printer = popen("lpr -Pnec","w")) == NULL){
       printf("UNABLE TO ACCESS LASER PRINTER\n");
       return;
   }


    fprintf(printer,"CELL SPACING DESCRIPTOR        : %s\n",
                          aftsensordescriptor->cell.cell_spacing_des);
    fprintf(printer,"CELL SPACING DESCRIPTOR LENGTH : %d\n",
                          aftsensordescriptor->cell.cell_spacing_des_len);
    fprintf(printer,"NUMBER OF SEQMENTS THAT CONTAIN CELLS OF EQUAL WIDTHS : %d\n",
                          aftsensordescriptor->cell.num_segments);
     fprintf(printer,"PRINTER,DISTANCE FOR FIRST GATE(m)    : %d\n",
                          aftsensordescriptor->cell.distToFirst);
     fprintf(printer,"WIDTH OF CELLS IN EACH SEGMENT IN METERS              :\n");
     for(i = 0; i < 6; i++)
        fprintf(printer," %d",aftsensordescriptor->cell.spacing[i]);

     fprintf(printer,"\nNUMBER OF CELL IN EACH SEGMENT\n");
     for(i = 0; i < 6; i++)
        fprintf(printer," %d",aftsensordescriptor->cell.num_cells[i]);
     close(printer);
 }/*end print_cellspacing_descriptor*/                          



