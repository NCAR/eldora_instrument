#define scope extern
#include "mkrDef.h"
#include "mkrGbl.h"
/* #include "mkrFunc.h" */

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE INSITU DATA DESCRIPTOR               */
/*****************************************************************************/
void initialize_insitudata(insitu_pntr)
    struct insitu_descript *insitu_pntr;

{
    int size, i, j;

    insitu_pntr->insitu_descript_id[0] = 'S';
    insitu_pntr->insitu_descript_id[1] = 'I';
    insitu_pntr->insitu_descript_id[2] = 'T';
    insitu_pntr->insitu_descript_id[3] = 'U';
    size = sizeof(struct insitu_descript);
    insitu_pntr->insitu_descript_len = size;
    insitu_pntr->number_params = 0;
    for (i=0; i<256; i++) {
        for (j=0; j<8; j++) {
            insitu_pntr->params[i].name[j] = 0x20;
            insitu_pntr->params[i].units[j] = 0x20;
        }
    }
}/*end initialize insitu data  descriptor*/

/*****************************************************************************/
/*              SUBROUTINE PRINT  INSITU DATA DESCRIPTOR                     */
/*****************************************************************************/

void fprint_insitudata(insitu_pntr,
                       print_pntr)
    struct insitu_descript *insitu_pntr;FILE *print_pntr; {

    int i;

    fprintf(print_pntr, 
            "SITUID    INSITU DATA DESCRIPTOR IDENTIFIER  :");
    for (i = 0; i < 4; i++) {
        fprintf(print_pntr, "%c", insitu_pntr->insitu_descript_id[i]);
    }
    fprintf(print_pntr, "\n");
    fprintf(print_pntr,
            "SITUDATLEN INSITU DATA LENGTH  %ld\n",
            insitu_pntr->insitu_descript_len);
}
