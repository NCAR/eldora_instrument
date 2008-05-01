//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Cpu.hh
//      Original Author: Rich Neitzel
//      Copywrited by the National Center for Atmospheric Research
//      Date:            $Date$
//
// revision history
// ----------------
// $Log$
//
//
// description:
//        
//
#ifndef _CPU_HH
#define _CPU_HH

#pragma interface

#include <vxWorks.h>
#include <sysLib.h>

class Cpu {
  public:
    Cpu() {}

    char *   sysModel() { return(sysModel()); }
    char *   memTop() { return(sysMemTop()); }
    STATUS   toMonitor(int startType) { return(sysToMonitor(startType)); }
    int      procNumGet() { return(sysProcNumGet()); }
    void     procNumSet(int procNum) { sysProcNumSet(procNum); }
    BOOL     sysBusTas(char *adrs) { return(sysBusTas(adrs)); }
    STATUS   nvRamGet(char *string, int strLen, int offset)
      { return(sysNvRamGet(string,strLen,offset)); }
    STATUS   nvRamSet(char *string, int strLen, int offset)
      { return(sysNvRamSet(string,strLen,offset)); }
    STATUS   scsiInit() { return(sysScsiInit()); }
    STATUS   clkConnect(FUNCPTR routine, int arg)
     { return(sysClkConnect(routine,arg)); }
    void     clkDisable() { sysClkDisable(); }
    void     clkEnable() { sysClkEnable(); }
    int      clkRateGet() { return(sysClkRateGet()); }
    STATUS   clkRateSet(int ticksPerSecond)
      { return(sysClkRateSet(ticksPerSecond)); }
    STATUS   auxClkConnect(FUNCPTR routine, int arg)
      { return(sysAuxClkConnect(routine,arg)); }
    void     auxClkEnable() {  sysAuxClkEnable(); }
    void     auxClkDisable() { sysAuxClkDisable(); }
    int      sysAuxClkRateGet() { return(sysAuxClkRateGet()); }
    STATUS   auxClkRateSet(int ticksPerSecond)
      { return(sysAuxClkRateSet(ticksPerSecond)); }
    STATUS   localToBusAdrs (int adrsSpace, char *localAdrs,
                                char **pBusAdrs)
      { return(sysLocalToBusAdrs(adrsSpace,localAdrs,pBusAdrs)); }
    STATUS   busToLocalAdrs (int adrsSpace, char *busAdrs,
                                char **pLocalAdrs)
      { return(sysBusToLocalAdrs(adrsSpace,busAdrs,pLocalAdrs)); }
    STATUS   intDisable(int intLevel) { return(sysIntDisable(intLevel)); }
    STATUS   intEnable(int intLevel) { return(sysIntEnable(intLevel)); }
    int      busIntAck(int intLevel) { return(sysBusIntAck(intLevel)); }
    STATUS   busIntGen(int level, int vector)
    { return(sysBusIntGen(level,vector)); }

    // Due to WRS's brain damage on some processors, I want these to be
    // virtual so we can use working code.
    virtual STATUS   mailboxConnect(FUNCPTR routine, int arg, int mailbox = 0);

    virtual STATUS   mailboxEnable(char *mailboxAdrs, void *vmeAdrs = NULL);

    // These routines are not part of the standard WRS system interface,
    // but they should be. They return ERROR if the function wouldn't work
    // on a given processor, else they return OK.
    // Bus request modes.
    virtual STATUS busRwd();
    virtual STATUS busRor();

    // Bus timeout.
    virtual STATUS busTmoEnable();
    virtual STATUS busTmoDisable();

    // Bus arbitration mode.
    virtual STATUS busPriorityMode();
    virtual STATUS busRoundRobin();
};

#endif // _CPU_HH
