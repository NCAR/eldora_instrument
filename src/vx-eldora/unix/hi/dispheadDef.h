#include <stdio.h>
#include <fcntl.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/uio.h>

#define OK_RPC
#include "Volume.h"
#include "RadarDesc.h"
#include "Parameter.h"
#include "Platform.h"
#include "CellSpacing.h"
#include "Waveform.h"
#include "FieldRadar.h"
#include "NavDesc.h"
#include "InSitu.h"

#define MAX_RADAR   2
#define MAX_PARAM   10
#define FORE   0
#define AFT    1


struct param_units
{
    char name[8];
    char units[8];
    char description[40];
};
