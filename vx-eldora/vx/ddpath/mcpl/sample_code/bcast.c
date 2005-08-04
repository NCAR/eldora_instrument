/***** CHANGE OVER to 2.00 Library 1/22/90 ************/
#include "/usr/mcpl/lib/mcpl.h"
#include "/usr/mcpl/lib/mcplproto.h"
#include "easisys.h"

 /** 
  MCPL Validation Support Routines Broadcast Support

  Copyright 1989, Kevin Lynch, Ironics Inc.
  IV-3220 addressed at VMEDUALPORT specified below
  IV-3272-M addressed at 0xE000 (accessed at 0xffefe000 in A16/D32)
 **/

#define VMEDUALPORT    (VmeAddress) 		0x12000000
/* #define MYBOARD 	(IV3272)		0xFFEFE000 /* My IV3272-M */
#define MYBOARD 	(unsigned int) 0 	/* 0xE000 */

#define MEG4 		0x400000 
#define LocalOffset 	0x0fff00

/* A32 D32 AM's */
#define VMEATTR		(VmeAttributeType) 	0x0d07
#define VMESCBLTATTR	(VmeAttributeType) 	0x1ba7
#define VMELINKATTR	(LinkAttributeType)	0x0d07
#define MBOXMODE 	0x3			/* A32, D16 */ 

#define MBX00 		(VmeAddress)(VMEDUALPORT + 0x3fff00)
#define MBX01 		(VmeAddress)(VMEDUALPORT + 0x3fff04)
#define MBX02 		(VmeAddress)(VMEDUALPORT + 0x3fff08)
#define MBX03 		(VmeAddress)(VMEDUALPORT + 0x3fff0C)
#define MBX04 		(VmeAddress)(VMEDUALPORT + 0x3fff10)

/* 0x30000 is the maximum size for 15 nodes */

#define SegmentSize 0x30000  /* Bytes */
#define TransferLength	((SegmentSize/4)-1)

#define XFERMODE 	TM_NCOMPELLED
#define RELMODE		TM_LOCKED
/* #define RELMODE		TM_BURST */

#define EntryIn (LocalNode)	/* Use for M-V */
#define EntryOut (LocalNode+15)  /* Use for V-M */

#define RemoteMemory (VmeAddress)(VMEDUALPORT+LocalOffset+ \
				  (SegmentSize*LocalNode))
#define LocalMemory RemoteMemory

 /**
 Globals
 **/

struct defaults {
	int Repeat;
	int QuiteMode;
	int Locked;
	int ShowTPB;
	int Entries;
	int PatternCheck;
	int Reload;
	int TLength;
	int Wait;
	int SlowI;
	int SlowT;
	int NackCount;
	int EditTPB;  
	};

extern struct defaults Defaults;
#define NADDRESS 32
#define MINADDRESS 1
#define MAXADDRESS 31
int Valid[NADDRESS];

ErrorType CheckError(ErrorType Value);

unsigned int LocalNode;		/* My node address */
unsigned short *Mbox0, *Mbox1, *Mbox2, *Mbox3, *Mbox4;

#define BcastLength 1023
char BcastMemory[(BcastLength+1)*4];
#define BcastMemoryAddr (VmeAddress)(VMEDUALPORT+BcastMemory)
#define EntryBcast 0
#define BcastVector 0x60
#define BcastIrq 0x6
#define BcastEnable 0x1		/* Enable = 1 */

int DoBCSend()
{
   int pass, ii, node;

   pass = 0;
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
      pass++;
      if (SendBcastSignal() == -2) {
		return(-1);
	    }
      if (statrx1() == 1) {
         printf("User break after %d passes\n", pass);
      	 return(1);
      }
   }
}

int
SendBcastSignal()
{
   char response[80];
   struct TPB *ExeTPB;
   int RetVal;
   unsigned short *Mbox;
   MCPLCWType McplCW;
   unsigned int MBXData;

   /* Build and Execute Bcast Signal TPB */

   ExeTPB = GetSysTPB(MYBOARD);
   CopyLocalDefault(MYBOARD, ExeTPB);
   if (Defaults.SlowI == 1) {
      SetSource(ExeTPB, BcastMemoryAddr, VMEATTR);
   } else {
      SetSource(ExeTPB, BcastMemoryAddr, VMESCBLTATTR);
   }
   SetSink(ExeTPB, MCPLADDR, MCPLATTR);
   SetLength(ExeTPB, 0);
   McplCW = BuildMcplCW(0, EntryBcast, BSIG, 
		RELMODE, TM_NCOMPELLED, TM_NOCONN);
   SetMcplCW(ExeTPB, McplCW);

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;

   if ( Defaults.ShowTPB == 1) {
      printf("\tBroadcast Send Signal :");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tBroadcast Send Signal :");
      ChangeTPB(ExeTPB);
   }

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != RetVal) {
      printf("ExecuteTPB failed, Bcast Signal ");
      CheckError(RetVal);
      return(-2);
   }
   /* Wait for completion */
   Timeout(Mbox);
   printmsg("BCast Send Signal"); CheckMBX(0, Mbox);
   MBXData = (unsigned int)*Mbox;

   switch (MBXData) {
   case BSIG_OK:RetVal = 1;
		break;
   default: 	RetVal = -1;
		break;
   }
   *Mbox = NO_MAIL;
   return(RetVal);
}
int DoBCData()
{
   int pass, ii, node;

   pass = 0;
   GenPattern(LocalMemory, 1);
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
      pass++;
      if (SendBcastData() == -2) {
                printf("Error break after %d passes\n", pass);
		return(-1);
	    }
      if (statrx1() == 1) {
         printf("User break after %d passes\n", pass);
      	 return(1);
      }
   }
}

int
SendBcastData()
{
   char response[80];
   struct TPB *ExeTPB;
   int RetVal;
   unsigned short *Mbox;
   MCPLCWType McplCW;
   unsigned int MBXData;

   /* Build and Execute Bcast Signal TPB */

   ExeTPB = GetSysTPB(MYBOARD);
   CopyLocalDefault(MYBOARD, ExeTPB);
   if (Defaults.SlowI == 1) {
      SetSource(ExeTPB, LocalMemory, VMEATTR);
   } else {
      SetSource(ExeTPB, LocalMemory, VMESCBLTATTR);
   }
   SetSink(ExeTPB, MCPLADDR, MCPLATTR);
   SetLength(ExeTPB, BcastLength); 
   McplCW = BuildMcplCW(0, EntryBcast, BDAT, 
		RELMODE, TM_NCOMPELLED, TM_CONN);
   SetMcplCW(ExeTPB, McplCW);

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;

   if ( Defaults.ShowTPB == 1) {
      printf("\tBroadcast Send Data :");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tBroadcast Send Data :");
      ChangeTPB(ExeTPB);
   }

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != RetVal) {
      printf("ExecuteTPB failed, Bcast Data ");
      CheckError(RetVal);
      return(-2);
   }
   /* Wait for completion */
   Timeout(Mbox);
   printmsg("BCast Send Data"); CheckMBX(0, Mbox);
   MBXData = (unsigned int)*Mbox;

   switch (MBXData) {
   case BDAT_OK:RetVal = 1;
		break;
   case NO_MAIL:printf("Bcast data No Mail!\n");
		RetVal = -1;
		break;
   default: 	RetVal = -1;
		break;
   }
   *Mbox = NO_MAIL;
   return(RetVal);
}

int
DoBCRecv()
{
   int retval;
   int pass;

   /* Load Broadcast Receive TPB */
   if ((retval = InitBcastTPB()) != 0) {
      printf("InitBcastTPB failed\n");
      return(retval);
   }
   Mbox4 = (unsigned short *)UnMapDualPort(MYBOARD, MBX04);
   *Mbox4 = NO_MAIL;
   pass = 0;
   printf("Waiting for Mail in Mbox4 (0x%x)\n", Mbox4);
   while (1) {
      if (statrx1() == 1) {
         printf("User break after %d BC receives\n", pass);
         return(1);
      }
      retval = Timeout(Mbox4);
      if (retval != 1) {   /* No Timeout Occured */
         printmsg("DoBCRecv"); CheckMBX(LocalNode, Mbox4);
         *Mbox4 = NO_MAIL;
	 pass++;
      }
   }
}

int
InitBcastTPB()
{
   struct TPB LocalTPB, *ExeTPB;
   unsigned short *Mbox;
   int RetVal;

   /* Build and Load Local Broadcast TPB */

   CopyLocalDefault(MYBOARD, &LocalTPB);
   SetSource(&LocalTPB, MCPLADDR, MCPLATTR);
   if (Defaults.SlowT == 1) {
      SetSink(&LocalTPB, BcastMemoryAddr, VMEATTR);
   } else {
      SetSink(&LocalTPB, BcastMemoryAddr, VMESCBLTATTR);
   }
   SetLength(&LocalTPB, BcastLength);
   SetLink(&LocalTPB, 0x0, 0x0) ;
   SetMbox(&LocalTPB, MBX04, VMEATTR);
   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &LocalTPB,LocalNode,EntryBcast);
   if (NOERROR != RetVal) {
      printf("BuildTableLoad local failed, %d\n", RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tLocal Bcast Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tLocal Bcast Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tLocal Bcast Table %d", EntryBcast);
      DumpTPB(&LocalTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tLocal Bcast Table %d", EntryBcast);
      ChangeTPB(&LocalTPB);
   }
 
   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != CheckError(RetVal)){
      return(-1);
   }

   /* Wait for load to completion */
   Timeout(Mbox);
   if (*Mbox != VME_OK) {
      printmsg("Local Bcast Loader "); CheckMBX(LocalNode, Mbox);
      return(-1);
   }
   printf("Local Bcast Loader Entry %d ",EntryBcast); CheckMBX(LocalNode, Mbox);
   return(0);
}

int
InitIRQBcastTPB()
{
   struct TPB LocalTPB, *ExeTPB;
   unsigned short *Mbox;
   int RetVal;

   /* Build and Load Local Broadcast TPB */

   CopyLocalDefault(MYBOARD, &LocalTPB);
   SetSource(&LocalTPB, MCPLADDR, MCPLATTR);
   if (Defaults.SlowT == 1) {
      SetSink(&LocalTPB, BcastMemoryAddr, VMEATTR);
   } else {
      SetSink(&LocalTPB, BcastMemoryAddr, VMESCBLTATTR);
   }
   SetLength(&LocalTPB, BcastLength);
   SetLink(&LocalTPB, 0x0, 0x0) ;
   SetMbox(&LocalTPB, MBX04, VMEATTR);
   RetVal = SetInterrupt(&LocalTPB, BcastVector, BcastIrq, BcastEnable);
   printf("SetInterrupt vector = 0x%x\n", RetVal);

   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &LocalTPB,LocalNode,EntryBcast);
   if (NOERROR != RetVal) {
      printf("BuildTableLoad local failed, %d\n", RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tLocal Bcast Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tLocal Bcast Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tLocal Bcast Table %d", EntryBcast);
      DumpTPB(&LocalTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tLocal Bcast Table %d", EntryBcast);
      ChangeTPB(&LocalTPB);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != CheckError(RetVal)){
      return(-1);
   }

   /* Wait for load to completion */
   Timeout(Mbox);
   if (*Mbox != VME_OK) {
      printmsg("Local Bcast Loader "); CheckMBX(LocalNode, Mbox);
      return(-1);
   }
   printf("Local Bcast Loader Entry %d ",EntryBcast); CheckMBX(LocalNode, Mbox);
   return(0);
}

int BCSigCount;
int BCDatCount;

int
DoBCInit()
{
   int retval;

   BCSigCount = 0;
   BCDatCount = 0;
   InitIRQBcastTPB();
   ivh_enb_irq(BcastIrq);		/* enable VME interrupt */
}

BCintr()
{
   unsigned short * Mbox;

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX04);
   printf("Bcast IRQ received 0x%x\n", *Mbox);
   switch (*Mbox) {
   case BSIG_RCV:	BCSigCount ++;
			break;
   case BDAT_RCV:	BCDatCount++;
			break;
   }
}

int
DoBCCount()
{
	printf("Broadcast Memory = 0x%x\n", BcastMemoryAddr);
	printf("Signal Interrupts Received = %d\n", BCSigCount);
	printf("Data Interrupts Received   = %d\n", BCDatCount);
}
