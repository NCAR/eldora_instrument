#include <stdio.h>
#include "field.h"

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE NAVAGATIONAL DESCRIPTOR               */
/*****************************************************************************/
     initialize_navagational_descriptor(navagationdesc)
    struct navDesc *navagationdesc;
{
   int size,i;


      navagationdesc->navagation.nav_descript_id[0] = 'N';
      navagationdesc->navagation.nav_descript_id[1] = 'D';
      navagationdesc->navagation.nav_descript_id[2] = 'D';
      navagationdesc->navagation.nav_descript_id[3] = 'S';
      size = sizeof(navagationdesc->navagation);
      navagationdesc->navagation.nav_descript_len = size;
      navagationdesc->navagation.ins_flag = 0;
      navagationdesc->navagation.gps_flag = 0;
      navagationdesc->navagation.minirims_flag = 0;
      navagationdesc->navagation.kalman_flag   = 0;
}/*end initialize cell descriptor*/


/****************************************************************************/
/*              SUBROUTINE EDIT NAVAGATIONAL DESCRIPTOR                     */
/****************************************************************************/
      edit_navagational_descriptor(navagationdesc)
    struct navDesc *navagationdesc;
      
   {

       int choice,flag,size;
       char input[3];


      navagationdesc->navagation.nav_descript_id[0] = 'N';
      navagationdesc->navagation.nav_descript_id[1] = 'D';
      navagationdesc->navagation.nav_descript_id[2] = 'D';
      navagationdesc->navagation.nav_descript_id[3] = 'S';
      size = sizeof(navagationdesc->navagation);
      navagationdesc->navagation.nav_descript_len = size;
       
       for(;;){
          do{
            printf("0. RETURN TO MAIN MENU\n");
            printf("1. SET THE INS FLAG\n");
            printf("2. SET THE GPS FLAG\n");
            printf("3. SET THE MINIRIMS FLAG\n");
            printf("4. SET THE KALMAN FLAG\n");
            printf("5. SET ALL FLAGS TO 1 AT ONCE\n");
            gets(input);
            choice = atoi(input);
            if(choice <0 || choice > 5) printf("INVALID ENTRY\n");
	} while(choice <0 || choice > 5);
        
     switch(choice){
          case 0: 
                return;                
                break;
          case 1:
                 printf("INS\n");
                 printf("ENTER 1 IF DATA IS BEING RECORDED\n");
                 printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
                 gets(input);
                 flag = atoi(input);
                 navagationdesc->navagation.ins_flag = flag;
                 break;
          case 2:     
                 printf("GPS\n");
                 printf("ENTER 1 IF DATA IS BEING RECORDED\n");
                 printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
                 gets(input);
                 flag = atoi(input);
                 navagationdesc->navagation.gps_flag = flag;
                break;
	  case 3:
                 printf("MINIRIMS\n");
                 printf("ENTER 1 IF DATA IS BEING RECORDED\n");
                 printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
                 gets(input);
                 flag = atoi(input);
                navagationdesc->navagation.minirims_flag = flag;
                break;
           case 4:
                 printf("KALMAN\n");
                 printf("ENTER 1 IF DATA IS BEING RECORDED\n");
                 printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
                 gets(input);
                 flag = atoi(input);
                navagationdesc->navagation.kalman_flag = flag;
                break;
           case 5:
                navagationdesc->navagation.ins_flag      = 1;
                navagationdesc->navagation.gps_flag      = 1;
                navagationdesc->navagation.minirims_flag = 1;
                navagationdesc->navagation.kalman_flag   = 1;
                break;
	    }/*switch*/
      }/*for*/
}/*end edit navagational descriptor*/


/****************************************************************************/
/*                 SUBROUTINE PRINT NAVAGATIONAL DATA DESCRIPTOR            */
/***************************************************************************/
   print_navagational_descriptor(navagationdesc)
      struct navDesc *navagationdesc;    
    
  {
    int i;

     printf("NAVAGATIONAL DATA DESCRIPTOR ID     :  ");
     for(i = 0; i < 4; i++){
         printf("%c",navagationdesc->navagation.nav_descript_id[i]);
     }
     printf("\nNAVAGATIONAL DATA DESCRIPTOR LENGTH : %d\n",
                          navagationdesc->navagation.nav_descript_len);
     printf("INS FLAG     : %d\n",navagationdesc->navagation.ins_flag);
     printf("GPS FLAG     : %d\n",navagationdesc->navagation.gps_flag);
     printf("MINIRIMS FLAG: %d\n",navagationdesc->navagation.minirims_flag);
     printf("KALMAN FLAG  : %d\n",navagationdesc->navagation.kalman_flag);

 }/*end print_navagational_descriptor*/                          


/****************************************************************************/
/*   SUBROUTINE TO PRINT NAVAGATIONAL DESCRIPTOR TO LASER PRINTER           */
/***************************************************************************/
   fprint_navagational_descriptor(navagationdesc)
      struct navDesc *navagationdesc;    
    
  {
    int i;
    FILE *printer,*popen();

    if((printer = popen("lpr -Pnec","w")) == NULL){
       printf("UNABLE TO ACCESS LASER PRINTER\n");
       return;
   }

     for(i = 0; i < 4; i++){
         fprintf(printer,"NAVAGATIONAL DATA DESCRIPTOR ID        : %c",
                          navagationdesc->navagation.nav_descript_id[i]);
     }
     fprintf(printer,"NAVAGATIONAL DATA DESCRIPTOR LENGTH : %d\n",
                          navagationdesc->navagation.nav_descript_len);
     fprintf(printer,"INS FLAG     : %d\n",navagationdesc->navagation.ins_flag);
     fprintf(printer,"GPS FLAG     : %d\n",navagationdesc->navagation.gps_flag);
     fprintf(printer,"MINIRIMS FLAG: %d\n",
                                      navagationdesc->navagation.minirims_flag);
     fprintf(printer,"KALMAN FLAG  : %d\n",
                                      navagationdesc->navagation.kalman_flag);
     close(printer);
 }/*end print_navagational_descriptor*/                          






