#include <stdlib.h>
#include <unistd.h>
#define scope
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

int main(int argc, char** argv) {
    
    if (argc != 2) {
        printf("usage: dumpheader header_file\n");
        exit(1);
    }
    
    /* Determine Processor Type */
    processor_type = big_little();
    
    if (read_header(argv[1])) {
        exit(1);
    }
    
    unpack_hd(); /* byte swap header -- on little endian machine */
    fprint_volume(&volume, stdout);
    fprint_waveform(&waveform, stdout);
    int i;
    int j;
    for (i=0; i < actual_num_radars; i++) {
        fprint_radar(&radar[i], stdout);
        fprint_fieldradar(&fradar[i], stdout);
        fprint_cellspacing(&cell[i], stdout);
        for (j=0; j<actual_num_params[i]; j++) {
            fprint_parameter(&parameter[j][i], stdout);
        }
    }
    fprint_navigation(&navigation, stdout);
    
    return 0;
}
/*******************************************************************/
/****    Function to cleanup ascii arrays so they look better   ****/
/*******************************************************************/
void cleanup(char* array, int size)
{
    int i, endfound;

    endfound = 0;
    for (i=0; i<size; i++)
        if (endfound)
            *array++ = 0x20; /* fill end with space characters */
        else if ((*array > 0x1F) && (*array < 0x7F))
            array++;
        else {
            *array++ = 0x20;
            endfound = 1;
        }
}

