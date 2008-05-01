
static char FN[] = __FILE__;

#include <stdLoc.h>
#include <stdlib.h>
#include <tickLib.h>
#include <sysLib.h>
#include <vme.h>
#include <taskLib.h>

#include "ELDRP7.h"
#include "ELDORA_RP7.hh"
#include "QuadDSP32C.hh"
#include "CollDSP32C.hh"

ELDORA_RP7::ELDORA_RP7()
{
    quad[0] = new QuadDSP32C (1,1);
    quad[1] = new QuadDSP32C (2,1);
    quad[2] = new QuadDSP32C (3,1);
    quad[3] = new QuadDSP32C (4,1);
    quad[4] = new QuadDSP32C (5,1);
    quad[5] = new QuadDSP32C (1,3);
    quad[6] = new QuadDSP32C (2,3);
    coll = new CollDSP32C();

}
ELDORA_RP7::~ELDORA_RP7()
{
    int i;

   //  fprintf(stderr, "destructor called for 0x%x\n", this);
    for (i = 0; i < 7; ++i)
    {
	delete quad[i];
    }
    delete coll;
}
int ELDORA_RP7::ppp_tst(short ppp_num)
{
// memory test for pulse pair processor of frequency ppp_num

    QuadDSP32C *ppp_q = quad[ppp_num-1];
    ppp_q -> memtst();

}
int ELDORA_RP7::dp_tst(short dp_num)
{
// memory test for doppler processor of dp_num

    QuadDSP32C *dp_q = quad[dp_num + 4];
    dp_q -> memtst();

}
int ELDORA_RP7::coll_tst()
{
// memory and BIM tests for collator

    CollDSP32C *col = coll;
    col -> memtst();
}

// test HSDBA data path between digital IFs and Pulse Pair Processor

extern "C" void prog_digif(int,int,int,int);
extern "C" int latch_daisy(short);
int ELDORA_RP7::digif2ppp_tst(short ppp_num)
{
    int status;
    
    status=0;    
    QuadDSP32C *ppp_q = quad[ppp_num-1];
    
    prog_digif(5,4,150,10);        // initialize 5 Digif's so they don't generate gates continuously
    ppp_q->load(-1,"wait03.dmp");         // load PPP DSPs with dummy program
    ppp_q->go(-1);                        // start PPP DSPs
    ppp_q->daisy(0xf);                    // initialize daisy chain
    latch_daisy(ppp_num);                 // latch daisy chain
    ppp_q->hsdba_wr(0,-99);               // write data out DSP0 of PPP
    status+=ppp_q->hsdba_rd(1,1,-99);     // read data from DSP1 of PPP
    status+=ppp_q->hsdba_rd(2,1,-99);     // read data from DSP2 of PPP
    status+=ppp_q->hsdba_rd(3,1,-99);     // read data from DSP3 of PPP
    ppp_q->hsdba_wr(1,-99);               // write data out DSP1 of PPP
    status+=ppp_q->hsdba_rd(0,1,-99);     // read data from DSP0 of PPP
    return(status);
    
}
int ELDORA_RP7::dp2coll_tst(short dp_num)

// tests HSDBB data path between Doppler Processors and Collator 

{
    int status,tmp_stat;

    status = 0;
    // First Test Doppler Processor #1
    
    QuadDSP32C *dp_q = quad[dp_num+4];
    CollDSP32C *col = coll;
    dp_q->load(-1,"wait03.dmp"); // load all DP dsp's w/ dummy program
    col->load("wait13.dmp");    // load Collator w/ dummy program
    col->go();                  // start Collator
    dp_q->go(-1);                // start DP

    // Test interconnect DP DSP0 to Collator

    dp_q->hsdbb_wr(0,-99);
    tmp_stat=col->hsdbb_rd(-99);    
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: dp #%d, dsp 0. Rx: Coll, dsp 0 \n",dp_num);
    
    // Test interconnect DP DSP1 to Collator

    dp_q->hsdbb_wr(1,-234);
    tmp_stat=col->hsdbb_rd(-234);    
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: dp #%d, dsp 1. Rx: Coll, dsp 0 \n",dp_num);

    // Test interconnect DP DSP2 to Collator

    dp_q->hsdbb_wr(2,-67);
    tmp_stat=col->hsdbb_rd(-67);    
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: dp #%d, dsp 2. Rx: Coll, dsp 0 \n",dp_num);

    // Test interconnect DP DSP3 to Collator

    dp_q->hsdbb_wr(3,-371);
    tmp_stat=col->hsdbb_rd(-371);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: dp #%d, dsp 3. Rx: Coll, dsp 0 \n",dp_num);
    return(status);
}

// Test HSDBB interconnect between Pulse Pair Processor and Doppler Processors

int ELDORA_RP7::ppp2dp_tst(short ppp_num)
{
    int status,tmp_stat;

    status = 0;
    // instantiate quad objects

    QuadDSP32C *ppp_q1 = quad[0];  // PPP #1
    QuadDSP32C *ppp_q = quad[ppp_num-1];
    QuadDSP32C *dp_q1 = quad[5];
    QuadDSP32C *dp_q2 = quad[6];
    
    if(ppp_num != 1)
      ppp_q1->load(0,"wait03.dmp");  // load PPP #1 dsp 0 w/ dummy program
    ppp_q->load(-1,"wait03.dmp");  // load all PPP dsp's w/ dummy program
    dp_q1->load(-1,"wait03.dmp");  // load all DP #1 dsp's w/ dummy program
    dp_q2->load(-1,"wait03.dmp");  // load all DP #2 dsp's w/ dummy program
    dp_q1->daisy(0xf);             // enable all DSPs in daisy chain for DP #1
    dp_q2->daisy(0xf);             // enable all DSPs in daisy chain for DP #2
    ppp_q1->go(0);                 // start PPP #1 dsp 0
    ppp_q1->toggle_pdr();          // latch daisy chain value
    if(ppp_num != 1)
      ppp_q->go(-1);               // start PPP
    else
      {
	ppp_q->go(1);
	ppp_q->go(2);
	ppp_q->go(3);
      }
    dp_q1->go(-1);                 // start DP #1
    dp_q2->go(-1);                 // start DP #2
    taskDelay(1);                  // insure DSP's are running 

    // test interconnect between PPP DSP0 and DP

    ppp_q->hsdbb_wr(0,-99);
    tmp_stat=dp_q1->hsdba_rd(0,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #1, dsp 0 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(1,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #1, dsp 1 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(2,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #1, dsp 2 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(3,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #1, dsp 3 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(0,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #2, dsp 0 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(1,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #2, dsp 1 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(2,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #2, dsp 2 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(3,0,-99);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 0. Rx: dp #2, dsp 3 \n",ppp_num);

    // test interconnect between PPP DSP1 and DP

    ppp_q->hsdbb_wr(1,-234);
    tmp_stat=dp_q1->hsdba_rd(0,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #1, dsp 0 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(1,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #1, dsp 1 \n",ppp_num);    
    tmp_stat=dp_q1->hsdba_rd(2,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #1, dsp 2 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(3,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #1, dsp 3 \n",ppp_num);    
    tmp_stat=dp_q2->hsdba_rd(0,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #2, dsp 0 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(1,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #2, dsp 1 \n",ppp_num);    
    tmp_stat=dp_q2->hsdba_rd(2,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #2, dsp 2 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(3,0,-234);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 1. Rx: dp #2, dsp 3 \n",ppp_num);

    // test interconnect between PPP DSP2 and DP

    ppp_q->hsdbb_wr(2,-68);
    tmp_stat=dp_q1->hsdba_rd(0,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #1, dsp 0 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(1,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #1, dsp 1 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(2,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #1, dsp 2 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(3,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #1, dsp 3 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(0,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #2, dsp 0 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(1,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #2, dsp 1 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(2,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #2, dsp 2 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(3,0,-68);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 2. Rx: dp #2, dsp 3 \n",ppp_num);

    // test interconnect between PPP DSP3 and DP

    ppp_q->hsdbb_wr(3,-372);
    tmp_stat=dp_q1->hsdba_rd(0,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #1, dsp 0 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(1,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #1, dsp 1 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(2,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #1, dsp 2 \n",ppp_num);
    tmp_stat=dp_q1->hsdba_rd(3,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #1, dsp 3 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(0,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #2, dsp 0 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(1,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #2, dsp 1 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(2,0,-372);
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #2, dsp 2 \n",ppp_num);
    tmp_stat=dp_q2->hsdba_rd(3,0,-372);    
    status += tmp_stat;
    if(tmp_stat)
      printf("Failed. Xmit: ppp #%d, dsp 3. Rx: dp #2, dsp 3 \n",ppp_num);
    return(status);

}




