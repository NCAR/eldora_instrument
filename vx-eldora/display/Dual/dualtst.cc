#include "Dual.hh"

extern "C" {
#include "timexLib.h"
#include "stdioLib.h"
void dualtst(double alt);
};

static void once(Dual &);
static void around(Dual &);

static DualData data;
static unsigned short tdata[4096];

static CELLSPACING cs;

void dualtst(double alt)
{
    GraphicController agc((void *)0x80000000,1280,1024,2048,250);

    Dual dual(&agc,0,0);

    cs.num_segments = 1;
    cs.distToFirst = 0;
    cs.spacing[0] = 150;
    cs.num_cells[0] = 1024;

    float max[] = { 31.0, 31.0 };
    float min[] = { 0.0, 0.0 };
    float scales[] = { 1.0, 1.0 };
    float biases[] = { 0.0, 0.0 };
    int offsets[] = { 0, 1 };
    
    ColorConverter conv(31,max,min,scales,biases,offsets,2,2);
    
    dual.Converter(&conv);
    dual.SetBounds(cs,30000.0,0.0);
    dual.SetOffsets(0,1);
    
    dual.DrawTable(A_SET,32.0,-32.0,REFLECT);
    dual.DrawTable(B_SET,32.0,-32.0,VELOCITY);

    dual.DrawTitle();

    agc.setMask(0x80);
    
    dual.Display();

    data.altitude = (float)alt;

    FAST unsigned short *ptr = tdata;

    FAST int j = 4096;

    for (FAST int i = 0; i < j; i++)
      *ptr++ = (unsigned char)(i % 31);

    data.data = tdata;

    printf("One rotation: ");
    getchar();

    timexN((FUNCPTR)around,(int)&dual,0,0,0,0,0,0,0);

//     printf("One beam: ");
//     getchar();
//     data.angle = 80.0;
//     timexN((FUNCPTR)once,(int)&dual,0,0,0,0,0,0,0);
        
    printf("Done: ");
    getchar();
}

static void once(FAST Dual &dual)
{
    dual.DrawBeam(data);
}

static void around(FAST Dual &dual)
{
    float angle = 0.0;
    float stop = 360.0;
    float inc = 1.0;

    while (angle <= stop)
      {
	  data.angle = angle;

	  dual.DrawBeam(data);

	  angle += inc;
      }
}
