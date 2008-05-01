/*
 *      $Id$
 *
 *      Module:          mini_dport.c
 *      Original Author: Rich Neitzel
 *      Copywrited by the National Center for Atmospheric Research
 *      Date:            $Date$
 *
 * revision history
 * ----------------
 * $Log$
 *
 * description:
 *        Dual ports the TP41 memory starting at local address
 *  0x40200000 to adress 0 on the VMEbus. This is seen by other TP41s
 * as local address 0x80000000. We are currently dual porting all of
 * the 2 Mb of memory available.
 */
static char rcsid[] = "$Date$ $RCSfile$ $Revision$";

#include "tp41Lib.h"

void mini_dport()
{
  dport((char *)0x40200000,(void *)0x0,4);
  wcio(0,"a",0xc8);
}
