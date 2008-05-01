//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Cpu.cc
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
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#pragma implementation

#include "Cpu.hh"

STATUS Cpu::mailboxConnect(FUNCPTR routine, int arg, int mailbox)
{
    return(sysMailboxConnect(routine,arg));
}

STATUS Cpu::mailboxEnable(char *mailboxAdrs, void *vmeAdrs)
{
    return(sysMailboxEnable(mailboxAdrs));
}

STATUS Cpu::busRwd()
{
    return(ERROR);
}

STATUS Cpu::busRor()
{
    return(ERROR);
}

STATUS Cpu::busTmoEnable()
{
    return(ERROR);
}

STATUS Cpu::busTmoDisable()
{
    return(ERROR);
}

STATUS Cpu::busPriorityMode()
{
    return(ERROR);
}

STATUS Cpu::busRoundRobin()
{
    return(ERROR);
}

