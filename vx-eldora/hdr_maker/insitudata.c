#include <stdio.h>
#include "field.h"

/*****************************************************************************/
/*              SUBROUTINE INITIALIZE INSITU DATA DESCRIPTOR               */
/*****************************************************************************/
     initialize_insitudata_descriptor(insitudescriptor)
    struct  inSituDataDesc  *insitudescriptor;

{
   int size,i;
   
   insitudescriptor->insitudata.insitu_descript_id[0] = 'S';
   insitudescriptor->insitudata.insitu_descript_id[1] = 'I';
   insitudescriptor->insitudata.insitu_descript_id[2] = 'T';
   insitudescriptor->insitudata.insitu_descript_id[3] = 'U';
   sizeof(insitudescriptor->insitudata);
   insitudescriptor->insitudata.insitu_descript_len  = size;
   insitudescriptor->insitudata.number_params        = -999;
   

}/*end initialize insitu data  descriptor*/



   print_insitudata_descriptor(insitudescriptor)
    struct  inSituDataDesc  *insitudescriptor;
  {
 
     int i;

     printf("INSITU DATA DESCRIPTOR IDENTIFIER  :");
     for(i = 0; i < 4; i++){
         printf("%c", insitudescriptor->insitudata.insitu_descript_id[i]);
     }
     printf("\n");
     printf("INSITU DATA LENGTH  %d\n", 
                           insitudescriptor->insitudata.insitu_descript_len);
  }


