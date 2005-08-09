//  This looks like C, but it's really C++!!
//      $Id$
//
//      Module:          Tp41.hh
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
#ifndef _TP41_HH
#define _TP41_HH

#pragma interface

#include "Cpu.hh"

class Tp41: public Cpu {
  public:
    Tp41();

    enum statusbits { UserBits = 0xff000000, VmeAtt = 0x00040000,
                      VsbAtt = 0x00020000, VmeExt = 0x00080000,
                      VmeSlot1 = 0x00200000, VsbSlot1 = 0x00100000,
                      Async = 0x00400000, Fault = 0x00800000 };
    // Get status register.
    int statusReg() { return(*(int *)0x6000000); }
    int userSwitch() { return(statusReg() & Tp41::UserBits); }
    int vmeAttached() { return(statusReg() & Tp41::VmeAtt); }
    int vsbAttached() { return(statusReg() & Tp41::VsbAtt); }
    int vmeExtAttached() { return(statusReg() & Tp41::VmeExt); }
    int vmeSlot1() { return(statusReg() & Tp41::VmeSlot1); }
    int vsbSlot1() { return(statusReg() & Tp41::VsbSlot1); }
    int asyncReset() { return(statusReg() & Tp41::Async); }
    int fault() { return(statusReg() & Tp41::Fault); }
          
    // Dual porting memory.
    STATUS dualPort(void *localAddr, void *vmeAddr, int numSegs);
    STATUS dualPortStd(void *localAddr, void *vmeAddr, int numSegs);

    // Mailboxes.
    void clearMailbox(int mailbox)
      {
          *(char *)0x10000000 = 1;
      }
    STATUS mailboxConnect(FUNCPTR routine, int arg, int mailbox = 0);
    STATUS mailboxEnable(char *mailboxAdrs, void *vmeAdrs);
    STATUS stdMailboxEnable(char *mailboxAdrs, void *vmeAdrs);

    // CIOs.
    int rcio(int, char *);
    STATUS wcio(int, char *, unsigned char);

    // Paging bits.
    STATUS setVmePage(int page);
    int getVmePage(void);

    // Bus request modes.
    STATUS busRwd();
    STATUS busRor();

    // Bus timeout.
    STATUS busTmoEnable();
    STATUS busTmoDisable();

    // Bus arbitration mode.
    STATUS busPriorityMode();
    STATUS busRoundRobin();

  private:
    enum CioMasks { AttVmeStd = 0x2, AttVmeExt = 0x4, BusTmo = 0x8,
                    Ror = 0x10, Rrs = 0x80, VmePage0 = 0x1,
                    VmePage1 = 0x2, VmePage2 = 0x4, Berr = 0x80,
                    Sysfail = 0x1 };

    enum Cio { cio0 = 0xc800000, cio1 = 0xd000000, cio2 = 0xd800000,
               cio3 = 0xe000000 };

    enum Offsets { a_off = 8, b_off = 4, c_off = 0 };
    
    void setcio(int cio, char *port, unsigned char mask);

    void enableMbox();

    STATUS myDualPort(void *localAdrs, void *busAdrs, int adrSpace, int mbox);
};

#endif // _TP41_HH
