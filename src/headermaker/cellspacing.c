#include <stdlib.h>
#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE CELL DESCRIPTOR                        */
/*****************************************************************************/
void initialize_cellspacing(cellspacing_pntr)
    struct cell_spacing_d *cellspacing_pntr;
{
   int size,i;


      cellspacing_pntr->cell_spacing_des[0] = 'C';
      cellspacing_pntr->cell_spacing_des[1] = 'S';
      cellspacing_pntr->cell_spacing_des[2] = 'P';    
      cellspacing_pntr->cell_spacing_des[3] = 'D';
      size = sizeof(struct cell_spacing_d);
      cellspacing_pntr->cell_spacing_des_len = size;
      cellspacing_pntr->num_segments = -999;
      cellspacing_pntr->distToFirst = -999;
      for(i = 0; i < 6; i++) cellspacing_pntr->spacing[i] = -999;
      for(i = 0; i < 6; i++) cellspacing_pntr->num_cells[i] = -999;

}/*end initialize cell descriptor*/

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE CELLSPACING STATE                      */
/*****************************************************************************/

void initialize_cellspacing_state()
{
    cell_st.num_segments = 0;
    cell_st.distToFirst = 0;
    cell_st.spacing[0] = 0;
    cell_st.num_cells[0] = 0;

}/*end initialize cellspacing state*/


/****************************************************************************/
/*              SUBROUTINE EDIT CELL SPACING  DESCRIPTOR                    */
/****************************************************************************/
void edit_cellspacing(cellspacing_pntr)
    struct cell_spacing_d *cellspacing_pntr;
      
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
                 cellspacing_pntr->num_segments = numsegs;
                 break;
          case 2:     
                printf("ENTER DISTANCE\n");
                gets(input);
                dist = atoi(input);
                cellspacing_pntr->distToFirst = dist;
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
                          cellspacing_pntr->spacing[i] = spacing;
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
                        cellspacing_pntr->num_cells[i] = numcell;
                        i++;
		    }
		 } while( i < 6);
                  break;
	    }/*switch*/
      }/*for*/
}/*end edit cell*/


/****************************************************************************/
/*   SUBROUTINE TO PRINT CELL SPACING DESCRIPTOR                            */
/***************************************************************************/
void fprint_cellspacing(cellspacing_pntr, print_pntr)
      struct cell_spacing_d *cellspacing_pntr;    
      FILE *print_pntr;
  {
    int i;
    fprintf(print_pntr,"********** CELL SPACING DESCRIPTOR ***************\n");
    fprintf(print_pntr,
            "CSPDID    IDENTIFIER                                        : %c%c%c%c\n",
	    cellspacing_pntr->cell_spacing_des[0],
	    cellspacing_pntr->cell_spacing_des[1],
	    cellspacing_pntr->cell_spacing_des[2],
	    cellspacing_pntr->cell_spacing_des[3]);
    fprintf(print_pntr,
            "CSPDDESLEN CELL SPACING DESCRIPTOR LENGTH                   : %ld\n",
                          cellspacing_pntr->cell_spacing_des_len);
    fprintf(print_pntr,
            "CSPDNUMSEG NUM OF SEGMENTS CONTAINING CELLS OF EQUAL WIDTHS : %d\n",
                          cellspacing_pntr->num_segments);
    fprintf(print_pntr,
            "CSPD1STGAT DISTANCE FOR FIRST GATE(m)                       : %d\n",
                          cellspacing_pntr->distToFirst);
    fprintf(print_pntr,
            "CSPDWIDTH  WIDTH OF CELLS IN EACH SEGMENT IN METERS         : ");
     for(i = 0; i < 6; i++)
        fprintf(print_pntr,"%d ",cellspacing_pntr->spacing[i]);

    fprintf(print_pntr,"\n");
    fprintf(print_pntr,
            "CSPDNUMBER NUMBER OF CELL IN EACH SEGMENT                   : ");
     for(i = 0; i < 6; i++)
        fprintf(print_pntr,"%d ",cellspacing_pntr->num_cells[i]);
    fprintf(print_pntr,"\n");

 }/*end fprint_cellspacing*/


/****************************************************************************/
/*              SUBROUTINE CALCULATE CELLSPACING DESCRIPTOR                 */
/****************************************************************************/

int calculate_cellspacing()
{
   int i, number_to_calculate;

   number_to_calculate = 3;

   /* Currently the number of segments is fixed in the radar processor
      so it has to be set to 4 */

   cell[FORE].num_segments = 4;
   cell[AFT].num_segments = 4;
   cell_st.num_segments = 1;

   /* Distance to the first cell is set in the engineering window, the
      waveform descriptor calculation routine assures that this distance
      is met by adjusting where the starting gate sample begins, but the
      routine uses the numbers for the fore radar only.  The aft radar's
      distance to first should be altered here so that it is correct */

   if(cell_st.distToFirst != 0)
     number_to_calculate--;
   else if(engin_st.first_gate_dis > 0)
     {
      cell[FORE].distToFirst = engin_win.first_gate_dis;
      cell[AFT].distToFirst = engin_win.first_gate_dis +
	2.0 * 0.15 * (fradar[FORE].dly_tube_antenna - 
		      fradar[AFT].dly_tube_antenna);

      cell_st.distToFirst = 3;
      number_to_calculate--;
  }

/* For now, we are recording each gate independently, therefore the gate
   spacing is the cell spacing */

   if(cell_st.spacing[0] != 0)
     number_to_calculate--;
   else if(waveform_st.gate_dist1[0] > 0 && cell_st.num_segments > 0)
     {
	 for(i=0; i<6; i++)
	   {
	       if(i < cell[FORE].num_segments)
		 {
		     cell[FORE].spacing[i] = waveform.gate_dist1[1] * 2.5;
		     cell[AFT].spacing[i] = waveform.gate_dist1[1] * 2.5;
		 }
	       else
		 {
		     cell[FORE].spacing[i] = -999;
		     cell[AFT].spacing[i] = -999;
		 }
	   }

	 cell_st.spacing[0] = 3;
	 number_to_calculate--;
     }

/* For now, we are recording each gate independently, no range averaging
   is being performed.  Therefore all but three gates can be placed in
   the first segment, then one of the three can be placed in each
   remaining segment */

   if(cell_st.num_cells[0] != 0)
     number_to_calculate--;
   else if(engin_st.numgates > 0 )
     {
	 cell[FORE].num_cells[0] = engin_win.numgates - 3;
	 cell[FORE].num_cells[1] = 1;
	 cell[FORE].num_cells[2] = 1;
	 cell[FORE].num_cells[3] = 1;

	 cell[AFT].num_cells[0] = engin_win.numgates - 3;
	 cell[AFT].num_cells[1] = 1;
	 cell[AFT].num_cells[2] = 1;
	 cell[AFT].num_cells[3] = 1;

	 cell_st.num_cells[0] = 1;
	 number_to_calculate--;
     }

return(number_to_calculate);

}/*end calculate cellspacing*/


