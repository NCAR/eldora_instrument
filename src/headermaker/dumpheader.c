#include <stdlib.h>
#include <unistd.h>
#define scope
#include "mkrDef.h"
#include "mkrGbl.h"
#include "mkrFunc.h"

int main(int argc,
         char** argv)
{

    int err = 0;
    int printHeader = 0;
    int printVerify = 0;

    
    // examine the command line arguments
    int argi = 1;
    if (argc < 2) {
        err++;
    } else {
        for (; argi < argc -1; argi++) {
            if (!strcmp(argv[argi], "-h"))
                printHeader = 1;
            else if (!strcmp(argv[argi], "-v"))
                printVerify = 1;
            else
                err++;
        }
    }

    if (err) {
        printf("usage: dumpheader [-h] [-v] header_file\n   -h print header\n   -v print verify output\n");
        exit(1);
    }

    /* Determine Processor Type */
    processor_type = big_little();

    // read the header
    if (read_header(argv[argi])) {
        exit(1);
    }

    unpack_hd(); /* byte swap header -- on little endian machine */
    if (printHeader) {
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
    }

    if (printVerify) {
        int num_errors;
        verify(1, &num_errors);
    }

    return 0;
}
/*******************************************************************/
/****    Function to cleanup ascii arrays so they look better   ****/
/*******************************************************************/
void cleanup(char* array,
             int size)
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

