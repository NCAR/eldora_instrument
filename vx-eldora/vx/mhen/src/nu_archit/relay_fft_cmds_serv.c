
/* FFT RPC */

/* THIS IS THE SERVER SIDE */

#define OK_RPC
#include <stdioLib.h>
#include "vx_fft_cmds.h"
#include "RadarGbls.h"



#define FN		"relay_fft_cmds_1()"
int  
*relay_fft_cmds_1(indata)
    char *indata; 
{
    register fft_cmds *ptr = (fft_cmds *)indata;
    static int result;

    static int num_cmds = 0;
    ts_flag = ptr -> fft_cmd_type;
    ts_samps = ptr -> fft_dataset_size;

    /* PROCESS MSG */
    switch ( ptr->fft_cmd_type ) {

	case 0 :
	    break;

	case 1 :
            break; 

	default :
	    printf("%s: WARNING: BAD FFT MESSAGE <%d>  \n", FN,
							ptr->fft_cmd_type);
	    break;

    }
    switch ( ptr->fft_dataset_size ) {

	case 16 :
	    break;
	case 32 :
	    break;
	case 64 :
	    break;
	case 128 :
	    break;
	case 256 :
	    break;
	case 512 :
	    break;
	case 1024 :
	    break;
	case 2048 :
	    break;
	default :
	    fprintf(stderr, "%s: WARNING: BAD FFT MESSAGE <%d>  \n", FN,
							ptr->fft_dataset_size);
	    break;
	}

    result = 1;
    return(&result);
}
