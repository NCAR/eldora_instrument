#define scope extern
#include <stdlib.h>

#include "mkrDef.h"
#include "mkrGbl.h"
/* #include "mkrFunc.h" */

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE NAVIGATIONAL DESCRIPTOR               */
/*****************************************************************************/
void initialize_navigation(nav_pntr)
    struct nav_descript *nav_pntr; {
    int size;

    nav_pntr->nav_descript_id[0] = 'N';
    nav_pntr->nav_descript_id[1] = 'D';
    nav_pntr->nav_descript_id[2] = 'D';
    nav_pntr->nav_descript_id[3] = 'S';
    size = sizeof(struct nav_descript);
    nav_pntr->nav_descript_len = size;
    nav_pntr->ins_flag = 0;
    nav_pntr->gps_flag = 0;
    nav_pntr->minirims_flag = 0;
    nav_pntr->kalman_flag = 0;
}/*end initialize cell descriptor*/

/****************************************************************************/
/*              SUBROUTINE EDIT NAVIGATIONAL DESCRIPTOR                     */
/****************************************************************************/
void edit_navigation(nav_pntr)
    struct nav_descript *nav_pntr;

{

    int choice, flag, size;
    char input[3];

    nav_pntr->nav_descript_id[0] = 'N';
    nav_pntr->nav_descript_id[1] = 'D';
    nav_pntr->nav_descript_id[2] = 'D';
    nav_pntr->nav_descript_id[3] = 'S';
    size = sizeof(struct nav_descript);
    nav_pntr->nav_descript_len = size;

    for (;;) {
        do {
            printf("0. RETURN TO MAIN MENU\n");
            printf("1. SET THE INS FLAG\n");
            printf("2. SET THE GPS FLAG\n");
            printf("3. SET THE MINIRIMS FLAG\n");
            printf("4. SET THE KALMAN FLAG\n");
            printf("5. SET ALL FLAGS TO 1 AT ONCE\n");
            fgets(input, sizeof(input), stdin);
            choice = atoi(input);
            if (choice <0 || choice > 5)
                printf("INVALID ENTRY\n");
        } while (choice <0 || choice > 5);

        switch (choice) {
        case 0:
            return;
            break;
        case 1:
            printf("INS\n");
            printf("ENTER 1 IF DATA IS BEING RECORDED\n");
            printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
            fgets(input, sizeof(input), stdin);
            flag = atoi(input);
            nav_pntr->ins_flag = flag;
            break;
        case 2:
            printf("GPS\n");
            printf("ENTER 1 IF DATA IS BEING RECORDED\n");
            printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
            fgets(input, sizeof(input), stdin);
            flag = atoi(input);
            nav_pntr->gps_flag = flag;
            break;
        case 3:
            printf("MINIRIMS\n");
            printf("ENTER 1 IF DATA IS BEING RECORDED\n");
            printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
            fgets(input, sizeof(input), stdin);
            flag = atoi(input);
            nav_pntr->minirims_flag = flag;
            break;
        case 4:
            printf("KALMAN\n");
            printf("ENTER 1 IF DATA IS BEING RECORDED\n");
            printf("ENTER 0 IF DATA IS NOT BEING RECORDED\n");
            fgets(input, sizeof(input), stdin);
            flag = atoi(input);
            nav_pntr->kalman_flag = flag;
            break;
        case 5:
            nav_pntr->ins_flag = 1;
            nav_pntr->gps_flag = 1;
            nav_pntr->minirims_flag = 1;
            nav_pntr->kalman_flag = 1;
            break;
        }/*switch*/
    }/*for*/
}/*end edit navigational descriptor*/

/****************************************************************************/
/*   SUBROUTINE TO PRINT NAVIGATIONAL DESCRIPTOR                            */
/***************************************************************************/
void fprint_navigation(nav_pntr,
                       print_pntr)
    struct nav_descript *nav_pntr;FILE *print_pntr; {
    int i;
    fprintf(print_pntr, "********* NAVIGATIONAL DATA DESCRIPTOR **********\n");
    fprintf(print_pntr, "NDDSID     Identifier                 : ");
    for (i = 0; i < 4; i++)
        fprintf(print_pntr, "%c", nav_pntr->nav_descript_id[i]);
    fprintf(print_pntr, "\n");
    fprintf(print_pntr, "NDDSDESLEN NAV DATA DESCRIPTOR LENGTH : %ld\n",
            nav_pntr->nav_descript_len);
    fprintf(print_pntr, "NDDSINSFLG INS FLAG                   : %d\n", nav_pntr->ins_flag);
    fprintf(print_pntr, "NDDSGPSFLG GPS FLAG                   : %d\n", nav_pntr->gps_flag);
    fprintf(print_pntr, "NDDSRIMFLG MINIRIMS FLAG              : %d\n", nav_pntr->minirims_flag);
    fprintf(print_pntr, "NDDSKALFLG KALMAN FLAG                : %d\n", nav_pntr->kalman_flag);
}/*end print_navigational_descriptor*/

