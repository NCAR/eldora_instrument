#include "ColorConverter.hh"

static unsigned short tstdata[1536];
static RadialData rd;
static HorizPoint dp;

extern "C" {
#include "timexLib.h"
#include "taskLib.h"
#include "string.h"
void contst(short, int);
};

void abeam(FAST ColorConverter &con);
void apoint(FAST ColorConverter &con);
unsigned char raw(FAST ColorConverter &con, FAST int i, FAST int p);

void contst(FAST short tstv, int size)
{
    FAST int j = 1536;
    FAST unsigned short *ptr = tstdata;

    for (FAST int i = 0; i < j; i++)
      *ptr++ = (tstv + i) % 101;

    float max[] = { 16.0, 16.0, 16.0 };
    float min[] = { -16.0, -16.0, -16.0 };

    float scales[] = { 1.0, 1.0, 1.0};
    float biases[] = { 16.0, 16.0, 16.0 };

    int offsets[] = { 0, 1, 2 };

    ColorConverter *con = new ColorConverter(31,max,min,scales,biases,offsets,
					     3,1);

    CELLSPACING cs;

    cs.num_segments = 1;
    cs.spacing[0] = 300;
    cs.num_cells[0] = 512;
    cs.distToFirst = 0;

    con->SetBeamSize(cs,size);

    timexN((FUNCPTR)abeam,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)apoint,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)raw,(int)con,45,0);
    taskDelay(60);

    con->Reset(31,max,min,scales,biases,offsets,3,2);

    con->SetBeamSize(cs,size);

    bzero((char *)&rd,sizeof(rd));

    timexN((FUNCPTR)abeam,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)apoint,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)raw,(int)con,45,1);
    taskDelay(60);

    con->Reset(31,max,min,scales,biases,offsets,3,3);

    con->SetBeamSize(cs,size);

    bzero((char *)&rd,sizeof(rd));

    timexN((FUNCPTR)abeam,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)apoint,(int)con);
    taskDelay(60);

    timexN((FUNCPTR)raw,(int)con,45,2);
    taskDelay(60);

    delete(con);
}

void abeam(FAST ColorConverter &con)
{
    con.GetBeam(tstdata,rd);
}

void apoint(FAST ColorConverter &con)
{
    con.GetPoint(tstdata,dp,0);
}

unsigned char raw(FAST ColorConverter &con, FAST int i, FAST int p)
{
    return(con.Raw(i,p));
}
