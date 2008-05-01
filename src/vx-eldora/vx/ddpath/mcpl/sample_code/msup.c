/*************  Updated for 2.0 Mcpl library *************/
/* 1/24/90	*/
#include "/usr/mcpl/lib/mcpl.h"
#include "/usr/mcpl/lib/mcplproto.h"
#include "easisys.h"

 /** 
  MCPL Validation Support Routines

  Copyright 1989, Kevin Lynch, Ironics Inc.
  IV-3220 addressed at VMEDUALPORT specified below
  IV-3272-M addressed at 0xE000 (accessed at 0xffefe000 in A16/D32)
 **/

#define VMEDUALPORT    (VmeAddress) 		0x12000000
/* #define MYBOARD 	(IV3272)		0xFFEFE000 /* My IV3272-M */

#define MYBOARDADDRESS 	(IV3272)		0xFFEFE000 /* My IV3272-M */
#define MYBOARD 	(unsigned int) 0 	/* 0xE000 */

#define CopyCodeAddress 0x12003000	/* VME address of code start */
#define CopyCodeLength  0x0000191f		/* Length */
#define CodeAddress 0x12002000		/* VME address of code start */
#define CodeLength  0x0003f7bf		/* Length from 0x2000 to 0xfff00 */

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
/* #define RELMODE		TM_LOCKED */
#define RELMODE		TM_BURST 

#define EntryIn (LocalNode)	/* Use for M-V */
#define EntryOut (LocalNode+15)  /* Use for V-M */

#define RemoteMemory (VmeAddress)(VMEDUALPORT+LocalOffset+ \
				  (SegmentSize*LocalNode))
#define LocalMemory RemoteMemory

#define BigArea1 (VmeAddress) (VMEDUALPORT+0x0fff00);
#define BigArea2 (VmeAddress) (VMEDUALPORT+0x267f00);
#define MaxBigHuge 0x168000

#define TestArea1 LocalMemory
#define TestArea2 LocalMemory + (VmeAddress)(SegmentSize/2)

 /**
 Globals
 **/

struct defaults {
	int Repeat;
	int QuietMode;
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
int LocalLoaded = 0 ;
#define NADDRESS 32
#define MINADDRESS 1
#define MAXADDRESS 31
int Valid[NADDRESS];
int RemoteLoadedIn[NADDRESS];
int RemoteLoadedOut[NADDRESS];
int RemoteLoadedRTX[NADDRESS];

ErrorType BuildTableUnloadTPB( IV3272 BoardAddr, 
			       struct TPB * LoaderTPB, 
			       struct TPB * TableTPB, 
			       unsigned int Entry );
ErrorType CheckError(ErrorType Value);

unsigned int LocalNode;		/* My node address */
unsigned short *Mbox0, *Mbox1, *Mbox2, *Mbox3, *Mbox4;

InitLibrary()
{
/* Set up Library */
	InitLibGlobals(MYBOARD, MYBOARDADDRESS); 
	SetLocalAttr(MYBOARD, VMEATTR); 
	SetDualPortAddr(MYBOARD, VMEDUALPORT, MEG4, 0, TRUE);  
	BuildDefaultTPB(); 

	LocalNode = GetNodeAddress(MYBOARD);  
	DoInit();  
}

int
DoClearTable()
{
	struct TPB *ExeTPB, TableTPB;
	unsigned short *Mbox;
	int RetVal, Table;
	char instring[100];

	printf("Enter table number : ");
	gets(instring);
	sscanf(instring, "%d", &Table);

	ExeTPB = GetSysTPB(MYBOARD);
	RetVal=BuildTableLoadTPB(MYBOARD,ExeTPB,&TableTPB,LocalNode,Table);
	SetLength(ExeTPB, 0);

	Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox = NO_MAIL;

      if (Defaults.ShowTPB == 1) {
	 printf("\tClear Table %d", Table);
	 DumpTPB(ExeTPB);
      }
      if (Defaults.EditTPB == 1) {
	 printf("\tClear Table %d", Table);
	 ChangeTPB(ExeTPB);
      }

      RetVal = ExecuteTPB(MYBOARD, ExeTPB);
      if (NOERROR != CheckError(RetVal)){
      	 return(-1);
      }
      /* Wait for load to completion */
      if (Timeout(Mbox)) {
      	 printf("Timeout waiting for Clear Table TPB\n");
         return(-1);
      }
      printf("Local Clear Table %d-", Table); CheckMBX(LocalNode, Mbox);
      *Mbox = NO_MAIL;
      LocalLoaded = 0;
}



DoHold()
{
   SetCommand( MYBOARD, CR_1_HOLD ) ;
   WriteCntlReg( MYBOARD, GetCntlRegImage(MYBOARD) ) ;
   StartCommand( MYBOARD ) ;
   SetInitLoaded(MYBOARD, NOTLOADED);
}

DoReset()
{
   if (ResetNode(MYBOARD) != NOERROR) {
	printf("Reset: Board stuck, can't reset\n");
   }
   Mbox0 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   printf("Reset Mailbox = 0x%x\n", *Mbox0);
   *Mbox0 = 0;
}

int
DoInit()
{
	int RetVal;
	struct TPB *InitTPB;

	LocalLoaded = 0;

	Mbox0 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox0 = 0;

	/* Issue HOLD Command */
        SetCommand( MYBOARD, CR_1_HOLD ) ;
        WriteCntlReg( MYBOARD, GetCntlRegImage(MYBOARD) ) ;
        StartCommand( MYBOARD ) ;

	if ( BoardArray[MYBOARD].InitTPBLoaded == LOADED) {
	   if (Timeout(Mbox0)) {
              printf("Timeout waiting for Hold Acknowledge\n");
	      if ( (ReadStatusReg(MYBOARD) & SR_BUSY) == SR_BUSY) {
	         printf("BUSY still asserted !!\n");
		 return(RetVal);
	      }
	      printf("Node IS in HOLD mode, continuing..\n");
	   } else {
              printf("Hold "); CheckMBX(LocalNode, Mbox0);
           }
	   *Mbox0 = 0;
           SetInitLoaded(MYBOARD, NOTLOADED);
	}

        /* Init TPB  Link (WFT) to SysTPB */
	InitTPB = GetSysTPB(MYBOARD);
	BuildInitTPB(MYBOARD, InitTPB);
	SetMbox(InitTPB, MBX00, MBOXMODE);

	if (Defaults.ShowTPB == 1 ) {
	   printf("\tInitTPB");
	   DumpTPB(InitTPB);
	}
	if (Defaults.EditTPB == 1 ) {
	   printf("\n\tInitTPB");
	   ChangeTPB(InitTPB);
	}

	Mbox0 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox0 = 0;

/* Load Init TPB and begin process */
	RetVal = InitNode(MYBOARD, InitTPB);
	if (NOERROR != RetVal) {
		printf("Execute Init TPB failed ");
		CheckError(RetVal);
		return(RetVal);
	}
	if (Timeout(Mbox0)) {
		printf("Timeout waiting for Init TPB\n");
		return(RetVal);
	}
	printmsg("Init"); CheckMBX(LocalNode, Mbox0);
	SetInitLoaded(MYBOARD, LOADED);
	return(NOERROR);
} /* end DoInit */

int
DoDTDis()
{
	int RetVal;
	struct TPB *InitTPB;
	MCPLCWType McplCW;

	LocalLoaded = 0;
	Mbox0 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox0 = 0;

	/* Issue HOLD Command */
	SetCommand( MYBOARD, CR_1_HOLD ) ;
	WriteCntlReg( MYBOARD, GetCntlRegImage(MYBOARD) ) ;
	StartCommand( MYBOARD ) ;

	if ( BoardArray[MYBOARD].InitTPBLoaded == LOADED) {
	   if (Timeout(Mbox0)) {
              printf("Timeout waiting for Hold Acknowledge\n");
	      if ( (ReadStatusReg(MYBOARD) & SR_BUSY) == SR_BUSY) {
	         printf("BUSY still asserted !!\n");
		 return(RetVal);
	      }
	      printf("Node IS in HOLD mode, continuing..\n");
	   } else {
              printf("Hold "); CheckMBX(LocalNode, Mbox0);
           }
	   *Mbox0 = 0;
	   SetInitLoaded(MYBOARD, NOTLOADED);
	}

        /* Init TPB  Link (WFT) to SysTPB */
	LocalLoaded = 0;
	InitTPB = GetSysTPB(MYBOARD);
	BuildInitTPB(MYBOARD, InitTPB);
	SetMbox(InitTPB, MBX00, MBOXMODE);

	McplCW = BuildMcplCW(0,0,DTD,0,0,0); /* Dispatch Table Disable */
	SetMcplCW(InitTPB, McplCW);

	if (Defaults.ShowTPB == 1 ) {
	   printf("\tInitTPB");
	   DumpTPB(InitTPB);
	}
	if (Defaults.EditTPB == 1 ) {
	   printf("\n\tInitTPB");
	   ChangeTPB(InitTPB);
	}

	Mbox0 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox0 = 0;

/* Load Init TPB and begin process */
	RetVal = InitNode(MYBOARD, InitTPB);
	if (NOERROR != RetVal) {
		printf("Execute Init TPB failed ");
		CheckError(RetVal);
		return(RetVal);
	}
	if (Timeout(Mbox0)) {
		printf("Timeout waiting for DTD Init TPB\n");
		return(RetVal);
	}
	printmsg("DTD Init"); CheckMBX(LocalNode, Mbox0);
	SetInitLoaded(MYBOARD, LOADED);
	return(NOERROR);
} /* end DoDTDis */

int
DoMonitor()
{
   StatRegType ControlSR, NewSR;

   /* Monitor the status register */
   printf("Status Register Monitor\n");
   /* Print out SR on start */
   ControlSR = ReadStatusReg(MYBOARD);
   printf("Current SR=\t0x%x\n", ControlSR);
   /* wait for it to change */
   while (1) {
      /* if it changes, print out new one */
      if (ControlSR != (NewSR = ReadStatusReg(MYBOARD)) ) {
	 printf("New SR=\t0x%x\n", NewSR);
	 ControlSR = NewSR;
      }
      /* if user input happens, break and return */
      if (statrx1() == 1) {
         printf("User break\n");
         return(1);
      }
   }
}

int
DoLocalXfer()
{
	IV3272 BoardAddr=MYBOARD; 
	VmeAddress FromArea=TestArea1, ToArea=TestArea2; 
	unsigned int Tsize=Defaults.TLength/2;

	struct TPB *ExeTPB;
	ErrorType RetVal;
	unsigned short *Mbox;
	int ii;

	ExeTPB = GetSysTPB(BoardAddr); 
	CopyLocalDefault(BoardAddr, ExeTPB);
	SetSource(ExeTPB, FromArea, VMESCBLTATTR);
	SetSink(ExeTPB, ToArea, VMESCBLTATTR);
	SetLength(ExeTPB, Tsize);

	Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
	*Mbox = NO_MAIL;

	if (Defaults.ShowTPB == 1) {
           printf("\tLocal Test XFER");
           DumpTPB(ExeTPB);
	}
	if (Defaults.EditTPB == 1) {
           printf("\n\tLocal Test XFER");
           ChangeTPB(ExeTPB);
	}

	for (ii=0; ii<Defaults.Repeat; ii++) {
		if (Defaults.PatternCheck == 1) {
			GenPattern(FromArea, 1);
			GenPattern(ToArea, 2);
		}
		*Mbox = NO_MAIL;
		RetVal = ExecuteTPB(BoardAddr, ExeTPB);
		if (NOERROR != CheckError(RetVal)){
			return(-1);
		}
		/* Wait for load to completion */
		if (Timeout(Mbox)) {
			printf("Timeout waiting for Local TPB\n");
			return(-1);
		}
		if (Defaults.QuietMode == 0) {
		    printf("%d, Local XFER -", ii+1); CheckMBX(LocalNode, Mbox);
		}
		if (statrx1() == 1) {
			printf("User break\n");
			return(1);
		}
		if (Defaults.PatternCheck == 1) {
			ComparePattern(ToArea, 1);
		}
	}
	return(NOERROR);
} /* end DoLocalXfer */

#define MAXENTRIES 32

/* Load every dispatch table entry, unload and verify */
int 
DoLocalLoad()
{
   struct TPB LocalTables[MAXENTRIES];
   struct TPB UnloadedTables[MAXENTRIES];
   int Results[MAXENTRIES];
   int BadCount;
   struct TPB *Loader;
   unsigned short *Mbox;
   int RetVal, entry;
   char response[80];

   /* For each local dispatch table, 0-31, make a unique pattern */
   for (entry=0; entry<Defaults.Entries; entry++) {
      CopyLocalDefault(MYBOARD, &LocalTables[entry]);
      SetSource(&LocalTables[entry], 0x55555555, VMEATTR);
      SetSink(&LocalTables[entry], 0xAAAAAAAA, VMEATTR);
      SetLength(&LocalTables[entry], entry);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;
   Loader = GetSysTPB(MYBOARD);

   /* Load each entry */
   for (entry=0; entry<Defaults.Entries; entry++) {
      BuildTableLoadTPB(MYBOARD, Loader, &LocalTables[entry], LocalNode, entry);

      if (Defaults.ShowTPB == 1) {
	 printf("\tentry %d Loader", entry);
	 DumpTPB(Loader);
      }
      if (Defaults.EditTPB == 1) {
	 printf("\n\tentry %d Loader", entry);
	 ChangeTPB(Loader);
      }

      RetVal = ExecuteTPB(MYBOARD, Loader);
      if (NOERROR != CheckError(RetVal)){
      	 return(-1);
      }
      /* Wait for load to completion */
      if (Timeout(Mbox)) {
      	 printf("Timeout waiting for Local TPB\n");
         return(-1);
      }
      printf("Local DT Load %d-", entry); CheckMBX(LocalNode, Mbox);
      *Mbox = NO_MAIL;
      if (statrx1() == 1) {
      	printf("User break\n");
      	return(0);
      }
   }

   /* Unload entries */
   for (entry=0; entry<Defaults.Entries; entry++) {
      BuildTableUnloadTPB(MYBOARD, Loader, &UnloadedTables[entry], entry);

      if (Defaults.ShowTPB == 1) {
	 printf("\tentry %d UnLoader", entry);
	 DumpTPB(Loader);
      }
      if (Defaults.EditTPB == 1) {
	 printf("\n\tentry %d UnLoader", entry);
	 ChangeTPB(Loader);
      }

      RetVal = ExecuteTPB(MYBOARD, Loader);
      if (NOERROR != CheckError(RetVal)){
      	 return(-1);
      }
      /* Wait for load to completion */
      if (Timeout(Mbox)) {
      	 printf("Timeout waiting for Local TPB\n");
         return(-1);
      }
      printf("Local DT Unload %d-", entry); CheckMBX(LocalNode, Mbox);
      *Mbox = NO_MAIL;
      if (statrx1() == 1) {
      	printf("User break\n");
      	return(0);
      }
   }
   BadCount = 0;
   for (entry=0; entry<Defaults.Entries; entry++) {
      if (LocalTables[entry] != UnloadedTables[entry]) {
	 BadCount++;
	 Results[entry] = 0;
      } else {
	 Results[entry] = 1;
      }
   }
   printf("%d of %d entries were BAD !!!\n",BadCount, Defaults.Entries);
   printf("Bad=");
   for (entry=0; entry<Defaults.Entries; entry++) {
      if (Results[entry] == 0) {
         printf("%d ", entry);
      }
   }
   printf("\n");

   while (1) {
	printf("Analyse ? ");
	gets(response);
	if (strlen(response) == 0) {
		break;
	}
	sscanf(response,"%d", &entry);
	if ( (entry >= 0) && (entry < MAXENTRIES) ) {
	   printf("\t%d Loaded", entry); DumpTPB(&LocalTables[entry]);
	   printf("\t%d UnLoaded", entry); DumpTPB(&UnloadedTables[entry]);
	} else {
	   printf("%d out of range\n", entry);
	}
   }
}

/*
  Valid addresses are 1, 2, 3, ... 29, 30, 31.	31 in total.
  The Valid structure contains  32 entries, entry zero is invalid
*/
   
int
DoRemoteConfig()
{
	int address, ii;
	char response[80];

	printf("Remote Configuration, you are node %d\n", LocalNode);
	for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
	   Valid[ii] = 0;
	   RemoteLoadedIn[ii]=0;
	   RemoteLoadedOut[ii]=0;
	   RemoteLoadedRTX[ii]=0;
	}

	while (1) {
	   printf("Enter Node addresses (<cr> when done ) : ");
	   gets(response);
	   if (strlen(response) == 0) {
	      break;
	   }
	   sscanf(response, "%d", &address);
	   if ( (address>=MINADDRESS) && (address<=MAXADDRESS) ) {
	      Valid[address] = 1;
	   } else {
	      printf("%d out of range\n", address);
	   }
	}
	printf("Valid nodes are:\n");
	for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
		if (Valid[ii] == 1) {
		   printf("%d ", ii);
		}
	}
	printf("\n");
}
ClearValid()
{
	int ii;

	for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
	   Valid[ii] = 0;
	}
}
int
CheckValid()
{
   int ii;
   int retval;

   retval=0;
   for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
      if (Valid[ii] == 1) {
         retval++;
      }
   }
   return(retval);
}

int 
DoRemoteLoad()
{
   struct TPB Tables[MAXENTRIES];
   struct TPB *Loader;
   unsigned short *Mbox;
   int node, RetVal, entry;
   char response[80];

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   /* For each remote dispatch table, 0-31, make a unique pattern */
   for (entry=0; entry<Defaults.Entries; entry++) {
      CopyLocalDefault(MYBOARD, &Tables[entry]);
      SetSource(&Tables[entry], 0x55555555, VMEATTR);
      SetSink(&Tables[entry], 0xAAAAAAAA, VMEATTR);
      SetLength(&Tables[entry], entry);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;
   Loader = GetSysTPB(MYBOARD);

   /* Loop for each Valid node */
   for (node=MINADDRESS; node<=MAXADDRESS; node++) {
      if (Valid[node] == 1) {
         for (entry=0; entry<Defaults.Entries; entry++) {
	      BuildTableLoadTPB(MYBOARD, Loader, &Tables[entry], node, entry);

	      if (Defaults.ShowTPB == 1) {
		 printf("\tRemote entry %d Loader", entry);
		 DumpTPB(Loader);
	      }
	      if (Defaults.EditTPB == 1) {
		 printf("\n\tRemote entry %d Loader", entry);
		 ChangeTPB(Loader);
	      }

	      RetVal = ExecuteTPB(MYBOARD, Loader);
	      if (NOERROR != CheckError(RetVal)){
		 return(-1);
	      }
	      /* Wait for load to completion */
	      if (Timeout(Mbox)) {
		 printf("Timeout waiting for Remote Load\n");
		 return(-1);
	      }
	      printf("Rem DT Load:%d-", entry); CheckMBX(node, Mbox);
	      *Mbox = NO_MAIL;
	      RemoteLoadedIn[node] = 0;
	      RemoteLoadedOut[node] = 0;
	      RemoteLoadedRTX[node] = 0;
	      if (statrx1() == 1) {
		printf("User break\n");
		return(1);
	      }
         } /* for entry */
      } /* if Valid[node] == 1 */
   }/* for nodes */
}

int
DoUnload()
{
   struct TPB Tables[33];
   struct TPB *Unloader;
   unsigned short *Mbox;
   int RetVal, entry;
   char response[80];

   Unloader = GetSysTPB(MYBOARD);
   BuildTableUnloadTPB(MYBOARD, Unloader, &Tables[0], 31);
   SetLength(Unloader, 0x107);
   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;
   RetVal = ExecuteTPB(MYBOARD, Unloader);
   if (NOERROR != CheckError(RetVal)){
     return(-1);
   }
   /* Wait for load to completion */
   if (Timeout(Mbox)) {
      printf("Timeout waiting for unload TPB\n");
      return(-1);
   }
   printf("Unload "); CheckMBX(LocalNode, Mbox);
   *Mbox = NO_MAIL;
   printf("Entries :  0 = DT 31\n");
   printf("           1 = RTX TPB\n");
   printf("           2 = stashed TPB\n");
   printf("          32 = Init TPB\n");
   while (1) {
	printf("Analyse ? ");
	gets(response);
	if (strlen(response) == 0) {
		break;
	}
	sscanf(response,"%d", &entry);
	if ( (entry >= 0) && (entry < 33) ) {
	   printf("\t%d Table", entry); DumpTPB(&Tables[entry]);
	} else {
	   printf("%d out of range\n", entry);
	}
   }
}

int 
DoLocalAnalyse()
{
   struct TPB LocalTables[MAXENTRIES];
   struct TPB UnloadedTables[MAXENTRIES];
   int Results[MAXENTRIES];
   int BadCount;
   struct TPB *Loader;
   unsigned short *Mbox;
   int RetVal, entry;
   char response[80];

   /* For each local dispatch table, 0-31, make a unique pattern */
/*
   for (entry=0; entry<Defaults.Entries; entry++) {
      CopyLocalDefault(MYBOARD, &LocalTables[entry]);
      SetSource(&LocalTables[entry], 0x55555555, VMEATTR);
      SetSink(&LocalTables[entry], 0xAAAAAAAA, VMEATTR);
      SetLength(&LocalTables[entry], entry);
   }
*/

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;
   Loader = GetSysTPB(MYBOARD);

   /* Unload entries */
   for (entry=0; entry<Defaults.Entries; entry++) {
      BuildTableUnloadTPB(MYBOARD, Loader, &UnloadedTables[entry], entry);

      if (Defaults.ShowTPB == 1) {
	 printf("\tentry %d UnLoader", entry);
	 DumpTPB(Loader);
      }
      if (Defaults.EditTPB == 1) {
	 printf("\n\tentry %d UnLoader", entry);
	 ChangeTPB(Loader);
      }

      RetVal = ExecuteTPB(MYBOARD, Loader);
      if (NOERROR != CheckError(RetVal)){
      	 return(-1);
      }
      /* Wait for load to completion */
      if (Timeout(Mbox)) {
      	 printf("Timeout waiting for Local TPB\n");
         return(-1);
      }
      printf("Local DT Unload %d ", entry); CheckMBX(LocalNode, Mbox);
      *Mbox = NO_MAIL;
      if (statrx1() == 1) {
      	printf("User break\n");
      	return(1);
      }
   }
/*
   BadCount = 0;
   for (entry=0; entry<Defaults.Entries; entry++) {
      if (LocalTables[entry] != UnloadedTables[entry]) {
	 BadCount++;
	 Results[entry] = 0;
      } else {
	 Results[entry] = 1;
      }
   }
   printf("%d of %d entries were BAD !!!\n",BadCount, Defaults.Entries);
   printf("Bad=");
   for (entry=0; entry<Defaults.Entries; entry++) {
      if (Results[entry] == 0) {
         printf("%d ", entry);
      }
   }
   printf("\n");
*/
   while (1) {
	printf("Analyse ? ");
	gets(response);
	if (strlen(response) == 0) {
		break;
	}
	sscanf(response,"%d", &entry);
	if ( (entry >= 0) && (entry < MAXENTRIES) ) {
	   /*
	   printf("\t%d Loaded", entry); DumpTPB(&LocalTables[entry]);
	   */
	   printf("\t%d UnLoaded", entry); DumpTPB(&UnloadedTables[entry]);
	} else {
	   printf("%d out of range\n", entry);
	}
   }
}

int 
DoMailCheck()
{
}

int DoLocalRemote()
{
   int pass, ii, node;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   pass = 0;
   GenPattern(LocalMemory, 1);
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
      pass++;
      for (node=MINADDRESS; node <= MAXADDRESS; node++) {
         if (Valid[node] == 1) {
	    if (SendTo(node) == -2) {
      	        printf("Error break after %d passes\n", pass);
		return(-1);
	    }
         }
         if (statrx1() == 1) {
      	   printf("User break after %d passes\n", pass);
      	   return(1);
         }
      }
   }
}

int DoLocalRemoteVmeirq()
{
   int pass, ii, node;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   pass = 0;
   GenPattern(LocalMemory, 1);
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
      pass++;
      for (node=MINADDRESS; node <= MAXADDRESS; node++) {
         if (Valid[node] == 1) {
	    if (SendToVmeirq(node) == -2) {
      	        printf("Error break after %d passes\n", pass);
		return(-1);
	    }
         }
         if (statrx1() == 1) {
      	   printf("User break after %d passes\n", pass);
      	   return(1);
         }
      }
   }
}

int DoCopyCode()
{
   int ii, node;
   int OldQuiet;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }

   OldQuiet = Defaults.QuietMode;
   Defaults.QuietMode = 0;
   for (node=MINADDRESS; node <= MAXADDRESS; node++) {
      if (Valid[node] == 1) {
         CopyCodeTo(node);
      }
   }
   Defaults.QuietMode = OldQuiet;
}
int
DoCheckCode()
{
   char response[80];
   int ii, RemNode;
   int *CheckAddr, *MyAddr;

   printf("Check code that came from node ? ");
   gets(response);
   if (strlen(response) == 0) {
	return(0);
   }
   sscanf(response, "%d", &RemNode);
   CheckAddr = (int *)(LocalOffset+(SegmentSize*RemNode));
   MyAddr = (int *)(CopyCodeAddress - VMEDUALPORT);
   printf("Checking addresses:\n");
   printf("My code at 0x%x for 0x%x lw's\n", MyAddr, CopyCodeLength+1);
   printf("Copied code at 0x%x for 0x%x lw's\n", CheckAddr, CopyCodeLength+1);
   for (ii=0; ii<=CopyCodeLength; ii++) {
      if (*MyAddr != *CheckAddr) {
	printf("Error at 0x%8lx is 0x%8lx should be 0x%8lx\n",
		CheckAddr, *CheckAddr, *MyAddr);
      }
      if (statrx1() == 1) {
         printf("User break\n");
      	 return(1);
      }
      MyAddr++; CheckAddr++;
   }
   return(0);
}

int DoLoad()
{
   int ii, node;
   int OldQuiet;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   OldQuiet = Defaults.QuietMode;
   Defaults.QuietMode = 0;
   for (node=MINADDRESS; node <= MAXADDRESS; node++) {
      if (Valid[node] == 1) {
         SendCodeTo(node);
      }
   }
   Defaults.QuietMode = OldQuiet;
}

int CRCCount;

int 
DoSendReceive()
{
   int retval, pass, ii, node;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   pass = 0;
   GenPattern(LocalMemory, 1);
   CRCCount = 0;
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
      pass++;
      for (node=MINADDRESS; node <= MAXADDRESS; node++) {
         if (Valid[node] == 1) {
	    wait();
   	    retval = SendTo(node);
	    switch (retval) {
      	    case -2:	printf("Really Bad error after %d passes\n", pass);
	    		return(-1);	/* fatal error */
	    case -1:	break;
	    case 1:	wait();		/* XMIT OK */
	       		if (ReceiveFrom(node) == 2) { /* CRC */
			   if (Defaults.PatternCheck == 1) 
			      ComparePattern(LocalMemory, 1); /* CRC */
                           GenPattern(LocalMemory, 1);
			}
			break;
	    case 2:	if (ReceiveFrom(node) == 2) {
	    		   printf("Double panic, CRC out, CRC in\n");
			}
			if (Defaults.PatternCheck == 1) {
			   ComparePattern(LocalMemory, 1); 
			}
                        GenPattern(LocalMemory, 1);
			break;
	    }
         }
         if (statrx1() == 1) {
      	    printf("User break after %d passes\n", pass);
      	    return(1);
         }
      }
   }
}
int 
DoRemoteLocal()
{
   int sret, pass, ii, node;

   if (CheckValid() == 0) {
	DoRemoteConfig();
   }
   pass = 0;
   for (ii=0; ii<Defaults.Repeat; ii++ ) {
   pass++;
      for (node=MINADDRESS; node <= MAXADDRESS; node++) {
         if (Valid[node] == 1) {
	    if (Defaults.PatternCheck == 1) {
               GenPattern(LocalMemory, 1);
	       if ( (sret = SendTo(node)) == -1) {
		  continue;
	       } else if (sret == -2) {
		  return(-1);
	       }
	       ClearPattern(LocalMemory);
	    }
	    wait();
	    if ( (sret = ReceiveFrom(node)) == -1) {
               if (statrx1() == 1) {
      	         printf("User break after %d passes\n", pass);
      	         return(1);
		}
		continue;
	    } else if (sret == -2) {
      	          printf("Error break after %d passes\n", pass);
		  return(-1);
	       }
            if (Defaults.PatternCheck == 1) {
	          ComparePattern(LocalMemory, 1);
	    }
            if (statrx1() == 1) {
      	      printf("User break after %d passes\n", pass);
      	      return(1);
            }
         }
      }
   }
}
int
DoChaining()
{
}

 /** Build init default for Local and Remote **/
BuildDefaultTPB()
{
	LocalMboxDefault(MYBOARD, MBX00, MBOXMODE);
	LocalInterruptDefault(MYBOARD, 0, 0, 0);
	LocalLinkDefault(MYBOARD, 
			MapDualPort(MYBOARD, GetSysTPB(MYBOARD)),
			VMELINKATTR);

	RemoteMboxDefault(MYBOARD, 0, 0);
	RemoteInterruptDefault(MYBOARD, 0, 0, 0);
	RemoteLinkDefault(MYBOARD, 0, 0);
}

ErrorType
BuildTableUnloadTPB( IV3272 BoardAddr,
		      struct TPB * LoaderTPB,
		      struct TPB * TableTPB,
	              unsigned int Entry )
{
	ErrorType ReturnCode ;
	VmeAttributeType MyLocalAttr ;
	MCPLCWType MyMcplCW ;

	if (NOERROR != 
		(ReturnCode = CopyLocalDefault( BoardAddr, LoaderTPB )) )
	{
		return( ReturnCode ) ;
	}

	MyLocalAttr = GetLocalAttr(BoardAddr) ;

	/* Set up Loader TPB fields */

	CopyLocalDefault(BoardAddr, LoaderTPB);
	SetSource(LoaderTPB, (VmeAddress)Entry, TABLEATTR) ;
	SetLength(LoaderTPB, TPB_LENGTH) ;
	SetLink(LoaderTPB, MapDualPort(BoardAddr, GetSysTPB(BoardAddr)), 
		SetLinkAttrWaitForTrigger(MyLocalAttr) ) ;
	SetSink(LoaderTPB, MapDualPort(BoardAddr, TableTPB), MyLocalAttr) ;
	return( ReturnCode ) ;
} /*BuildTableUnloadTPB()*/

 /** Pattern Management **/

ClearPattern(VmeAddress Area)
{
	int i;
	int *Pattern; 
	int int_count;

	int_count = Defaults.TLength+1;
	Pattern = (int *)UnMapDualPort(MYBOARD, Area);
	for (i=0; i<int_count; i++){
		Pattern[i] = (int) 0;
	}
}
GenPattern(VmeAddress Area, unsigned int Seed)
{
	int i;
	int *Pattern; 
	int int_count;

	int_count = Defaults.TLength+1;
	Pattern = (int *)UnMapDualPort(MYBOARD, Area);
	for (i=0; i<int_count; i++){
	   Pattern[i] = (int)( (i+1) | ((i+1)<<16) );
	}
}

int
ComparePattern(VmeAddress Area, unsigned int Seed)
{
	int ii, jj;
	int *Pattern; 
	int int_count;
	int ErrFound = 0;
	char dummy[10];

	int_count = Defaults.TLength+1;
	Pattern = (int *)UnMapDualPort(MYBOARD, Area);
	for (ii=0; ii<int_count; ii++){
	   if (Pattern[ii] != (int)( (ii+1) | ((ii+1)<<16) ) ){
   	      for (jj=ii-2; jj<=ii+5; jj++) {
		 ErrFound = 1;
 	         printf("BAD- :%8lx:%8lx: at %8lx", 
	                (jj+1) | ((jj+1)<<16), Pattern[jj], &Pattern[jj]);
		 if (jj == ii) printf(" ***\n");
		 else printf("\n");
	      }
/*	      printf("<cr> to continue :"); 	*/
/*	      gets(dummy);			*/
 	      printf("LAST-1 :%8lx:%8lx: at %8lx\n", 
	             ((int_count-2)+1) | (((int_count-2)+1)<<16), 
		     Pattern[int_count-2], 
		     &Pattern[int_count-2]);
 	      printf("LAST   :%8lx:%8lx: at %8lx\n", 
	             ((int_count-1)+1) | (((int_count-1)+1)<<16), 
		     Pattern[int_count-1], 
		     &Pattern[int_count-1]);
	      return(-1);
	   }
	}
	return(1);
}
/* SendTo - sends our pattern to the remote node
  Returns:	-2	Fatal error, No Mail
		-1	TIMEOUT not fatal
		 1	Xmit OK
		 2	CRC
		 3	NACK
 */

int
SendTo(unsigned int ToNode)
{
   char response[80];
   struct TPB *ExeTPB;
   int RetVal;
   unsigned short *Mbox;
   MCPLCWType McplCW;
   unsigned int MBXData;

   if (InitRemoteDispatchTo(ToNode) == -1) {
      printf("InitRemoteDispatchTo node %d failed, bailing out ...\n", ToNode);
      return(-1);
   }

   /* Build and Execute Send V-M TPB */

   ExeTPB = GetSysTPB(MYBOARD);
   CopyLocalDefault(MYBOARD, ExeTPB);
   if (Defaults.SlowI == 1) {
      SetSource(ExeTPB, LocalMemory, VMEATTR);
   } else {
      SetSource(ExeTPB, LocalMemory, VMESCBLTATTR);
   }
   SetSink(ExeTPB, MCPLADDR, MCPLATTR);
   SetLength(ExeTPB, Defaults.TLength);
   if (Defaults.Locked == 1)
   {
      McplCW = BuildMcplCW(ToNode, EntryIn, DTX, 
		TM_LOCKED, XFERMODE, TM_CONN);
   } else {
      McplCW = BuildMcplCW(ToNode, EntryIn, DTX, 
		TM_BURST, XFERMODE, TM_CONN);
   }
   SetMcplCW(ExeTPB, McplCW);
   SetNackCount(ExeTPB, Defaults.NackCount);

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;

   if ( Defaults.ShowTPB == 1) {
      printf("\tSendTo:");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tSendTo:");
      ChangeTPB(ExeTPB);
   }

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != RetVal) {
      printf("ExecuteTPB failed, Send to %d - ", ToNode);
      CheckError(RetVal);
      return(-2);
   }
   /* Wait for completion */
   Timeout(Mbox);
   printmsg("Send To"); CheckMBX(ToNode, Mbox);
   MBXData = (unsigned int)*Mbox;

   switch (MBXData) {
   case XMIT_OK:RetVal = 1;
		break;
   case LOCAL_VME_ERROR:printf("SendTo to %d, Local VMEbus error\n", ToNode);
		RetVal = -1;
		break;
   case VtoM_TIMEOUT: printf("SendTo Timeout to %d\n", ToNode);
		RetVal = -1;
		break;
   case INIT_CRC_ERROR: printf("init CRC sending to node %d\n", ToNode);
		RetVal = 2;
		break;
   case TARGET_CRC_ERROR: printf("target CRC sending to node %d\n", ToNode);
		RetVal = 2;
		break;
   case	VtoM_NACK:	RetVal = 3;
		break;
   case NO_MAIL: printf("Send to %d No Mail!!!\n", ToNode);
		 RetVal = -2;
		 break;
   default:	Valid[ToNode] = 0;
      		printf("0x%x-Node %d is now removed from Config\n", 
			(unsigned int)*Mbox, ToNode);
	printf("pres cr to continue ");
	gets(response);
	RetVal = -1;
	break;
}
*Mbox = NO_MAIL;
return(RetVal);
}

int IntrFlag;

t1_intr() /* this is the interrupt handler */
{
IntrFlag = 1;
}

/* SendToVmeirq - sends our pattern to the remote node
Returns:	-2	Fatal error
	-1	CRC or NACK or TIMEOUT not fatal
	 1	Xmit OK
*/

int
SendToVmeirq(unsigned int ToNode)
{
char response[80];
struct TPB *ExeTPB;
int RetVal;
unsigned short Vector, *Mbox;
MCPLCWType McplCW;
unsigned int MBXData;

if (InitRemoteDispatchTo(ToNode) == -1) {
printf("InitRemoteDispatchTo node %d failed, bailing out ...\n", ToNode);
return(-1);
}

/* Build and Execute Send V-M TPB */

ExeTPB = GetSysTPB(MYBOARD);
CopyLocalDefault(MYBOARD, ExeTPB);
if (Defaults.SlowI == 1) {
SetSource(ExeTPB, LocalMemory, VMEATTR);
} else {
SetSource(ExeTPB, LocalMemory, VMESCBLTATTR);
}
SetSink(ExeTPB, MCPLADDR, MCPLATTR);
SetLength(ExeTPB, Defaults.TLength);
if (Defaults.Locked == 1)
{
McplCW = BuildMcplCW(ToNode, EntryIn, DTX, TM_LOCKED, XFERMODE, TM_CONN);
} else {
McplCW = BuildMcplCW(ToNode, EntryIn, DTX, TM_BURST, XFERMODE, TM_CONN);
}
SetMcplCW(ExeTPB, McplCW);
SetNackCount(ExeTPB, Defaults.NackCount);
Vector = SetInterrupt(ExeTPB, 0x70, 0x05, 1);

Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
*Mbox = NO_MAIL;

if ( Defaults.ShowTPB == 1) {
printf("\tSendTo:");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tSendTo:");
ChangeTPB(ExeTPB);
}

IntrFlag = 0;
RetVal = ExecuteTPB(MYBOARD, ExeTPB);
if (NOERROR != RetVal) {
printf("ExecuteTPB failed, Send to %d - ", ToNode);
CheckError(RetVal);
return(-2);
}
/* Wait for flag from Interrupt handler */
while(IntrFlag == 0) ;
/* Wait for completion */
Timeout(Mbox);
printmsg("Send To"); CheckMBX(ToNode, Mbox);
MBXData = (unsigned int)*Mbox;

switch (MBXData) {
case XMIT_OK:RetVal = 1;
	break;
case VtoM_TIMEOUT: printf("SendTo IRQ Timeout to %d\n", ToNode);
	RetVal = -1;
	break;
case INIT_CRC_ERROR: printf("init CRC sending to node %d\n", ToNode);
	RetVal = -1;
	break;
case TARGET_CRC_ERROR: printf("target CRC sending to node %d\n", ToNode);
	RetVal = -1;
	break;
case	VtoM_NACK:	RetVal = -1;
	break;
case NO_MAIL: printf("Send to (vmeirq) %d No Mail!!!\n", ToNode);
	 RetVal = -1;
	 break;
default:	Valid[ToNode] = 0;
	printf("0x%x-Node %d is now removed from Config\n", 
		(unsigned int)*Mbox, ToNode);
	printf("pres cr to continue ");
	gets(response);
	RetVal = -1;
	break;
}
*Mbox = NO_MAIL;
return(RetVal);
}
/* CopyCodeTo - sends our Code to the remote node
*/

int
CopyCodeTo(unsigned int ToNode)
{
char response[80];
struct TPB *ExeTPB;
int RetVal;
unsigned short *Mbox;
MCPLCWType McplCW;
unsigned int MBXData;

if (InitCopyLoader(ToNode) == -1) {
printf("InitCopyLoader node %d failed, bailing out ...\n", ToNode);
return(-1);
}

/* Build and Execute Send Code V-M TPB */

ExeTPB = GetSysTPB(MYBOARD);
CopyLocalDefault(MYBOARD, ExeTPB);
SetSource(ExeTPB, CopyCodeAddress, VMESCBLTATTR);
SetSink(ExeTPB, MCPLADDR, MCPLATTR);
SetLength(ExeTPB, CopyCodeLength);
McplCW = BuildMcplCW(ToNode, EntryIn, DTX, RELMODE, XFERMODE, TM_CONN);
SetMcplCW(ExeTPB, McplCW);
SetNackCount(ExeTPB, Defaults.NackCount);

Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
*Mbox = NO_MAIL;

if ( Defaults.ShowTPB == 1) {
printf("\tSendCodeTo:");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tSendCodeTo:");
ChangeTPB(ExeTPB);
}

RetVal = ExecuteTPB(MYBOARD, ExeTPB);
if (NOERROR != RetVal) {
printf("ExecuteTPB failed, Send Code to %d - ", ToNode);
CheckError(RetVal);
return(-1);
}
/* Wait for completion */
Timeout(Mbox);
printmsg("Copy Code To"); CheckMBX(ToNode, Mbox);
MBXData = (unsigned int)*Mbox;
switch (MBXData) {
case XMIT_OK:break;
case INIT_CRC_ERROR:
case TARGET_CRC_ERROR:
case	VtoM_NACK:	break;
default:	Valid[ToNode] = 0;
	printf("Node %d is now removed from Remote Config\n", 
	   ToNode);
	break;
}
*Mbox = NO_MAIL;
return(0);
}
/* SendCodeTo - sends our Code to the remote node
*/

int
SendCodeTo(unsigned int ToNode)
{
char response[80];
struct TPB *ExeTPB;
int RetVal;
unsigned short *Mbox;
MCPLCWType McplCW;
unsigned int MBXData;

if (InitCodeLoader(ToNode) == -1) {
printf("InitCodeLoader node %d failed, bailing out ...\n", ToNode);
return(-1);
}

/* Build and Execute Send Code V-M TPB */

ExeTPB = GetSysTPB(MYBOARD);
CopyLocalDefault(MYBOARD, ExeTPB);
SetSource(ExeTPB, CodeAddress, VMESCBLTATTR);
SetSink(ExeTPB, MCPLADDR, MCPLATTR);
SetLength(ExeTPB, CodeLength);
McplCW = BuildMcplCW(ToNode, EntryIn, DTX, RELMODE, XFERMODE, TM_CONN);
SetMcplCW(ExeTPB, McplCW);
SetNackCount(ExeTPB, Defaults.NackCount);

Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
*Mbox = NO_MAIL;

if ( Defaults.ShowTPB == 1) {
printf("\tSendCodeTo:");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tSendCodeTo:");
ChangeTPB(ExeTPB);
}

RetVal = ExecuteTPB(MYBOARD, ExeTPB);
if (NOERROR != RetVal) {
printf("ExecuteTPB failed, Send Code to %d - ", ToNode);
CheckError(RetVal);
return(-1);
}
/* Wait for completion */
Timeout(Mbox);
printmsg("Send Code To"); CheckMBX(ToNode, Mbox);
MBXData = (unsigned int)*Mbox;
switch (MBXData) {
case XMIT_OK:break;
case TARGET_CRC_ERROR:
case INIT_CRC_ERROR:
case	VtoM_NACK:	break;
default:	Valid[ToNode] = 0;
	printf("Node %d is now removed from Remote Config\n", 
	   ToNode);
	break;
}
*Mbox = NO_MAIL;
return(0);
}

/* ReceiveFrom, gets a pattern from the remote node and places it in
* pattern area.
* -2 = really bad error
* -1 = bad error
*  0 = no error
*  2 = CRC Error
*/
int
ReceiveFrom(unsigned int FromNode)
{
char response[80];
int Residual, RetVal;
struct TPB *ExeTPB;
unsigned short *Mbox00, *Mbox01;
MCPLCWType McplCW;
unsigned int MBXData;

   if (InitRemoteDispatchFrom(FromNode) == -1) {
      printf("InitRemoteDispatchFrom failed, bailing out ...\n", FromNode);
      return(-1);
   }

   if (InitLocalDispatch() == -1) {
      printf("InitLocalDispatch failed, bailing out ...\n");
      return(-1);
   }
   /* Build and execute Triggering TPB */
   ExeTPB = GetSysTPB(MYBOARD);
   CopyLocalDefault(MYBOARD, ExeTPB);
   SetSource(ExeTPB, 0, VMEATTR);
   SetSink(ExeTPB, MCPLADDR, MCPLATTR);
   McplCW = BuildMcplCW(FromNode, EntryOut, DTT, 
		TM_LOCKED, TM_COMPELLED, TM_NOCONN);
   SetMcplCW(ExeTPB, McplCW);
   SetNackCount(ExeTPB, Defaults.NackCount);

   Mbox00 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   Mbox01 = (unsigned short *)UnMapDualPort(MYBOARD, MBX01);
   *Mbox00 = NO_MAIL;   /* DTX to FromNode */
   *Mbox01 = NO_MAIL;	/* Data Transfer */

   if (Defaults.ShowTPB == 1 ) {
      printf("\tReceive From:");
      DumpTPB(ExeTPB);
      }
   if (Defaults.EditTPB == 1 ) {
      printf("\n\tReceive From:");
      ChangeTPB(ExeTPB);
   }

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != RetVal) {
      printf("Error issuing receive DTT, %d\n", RetVal);
      return(-2);
   }

   /* Wait for DTX to remote to completion */
   Timeout(Mbox00);
   printmsg("DTT To "); CheckMBX(FromNode, Mbox00);
   MBXData = (unsigned int)*Mbox00;
   RetVal = 0;
   if ( (MBXData&0xfff0 == STATUS_TIMEOUT) ) {
      Residual = MBXData & 0x000F;
      MBXData = MBXData & 0xFFF0;
   }
   switch (MBXData) {
      case XMIT_OK: break;
      case STATUS_TIMEOUT:
	   printf("Status Timeout %d\n", Residual);
	   return(-1);
	   break;
      case VtoM_TIMEOUT: printf("ReceiveFrom DTT Timeout to %d\n", FromNode);
	   return(-1);
	   break;
      case NO_MAIL: printf("ReceiveFrom %d DTT No Mail\n", FromNode);
	   return(-1);
	   break;
      case VtoM_NACK:	return(-1);
	   break;
      default:	Valid[FromNode] = 0;
	      printf("DTT-0x%x Node %d is now removed from Remote Config\n", 
	             MBXData, FromNode);
	      printf("cr to continue ");
	      gets(response);
	      return(-1);
	      break;
      }

   Timeout(Mbox01);
   printmsg("Receive From "); CheckMBX(FromNode, Mbox01);
   MBXData = (unsigned int)*Mbox01;
   switch (MBXData) {
   case RCV_OK: break;
   case TARGET_CRC_ERROR: printf ("target CRC during receive from %d\n", FromNode);
	      return(2);
   case INIT_CRC_ERROR: printf ("init CRC during receive from %d\n", FromNode);
	      return(2);
   case NO_MAIL: printf("ReceiveFrom:%d No Mail!!!!\n",FromNode);
	    return(-1);
   case MtoV_NACK: return(-1);
   default:
      Valid[FromNode] = 0;
      printf("ReceiveFrom, 0x%x Node %d is now removed from Remote Config\n", 
        MBXData, FromNode);
      printf("cr to continue ");
      gets(response);
      return(-1);
      }
   return(0);
   }

/* InitRemoteDispatchFrom
* Load Remote dispatch table EntryOut V-M
* This routine must retry if a NACK is received
*/
int
InitRemoteDispatchFrom(unsigned int RemNode)
{
struct TPB RemoteTPB, *ExeTPB;
unsigned short *Mbox;
ErrorType RetVal;
MCPLCWType McplCW;
unsigned int dtl_ok, MBXData;

   /* Do nothing if Reload=0 and Remote has valid entry */
   if ( ( Defaults.Reload == 0) && (RemoteLoadedOut[RemNode] == 1) ) {
      return(0);
   }

   /* Build and Load Remote (V-M)*/
   CopyRemoteDefault(MYBOARD, &RemoteTPB);
   if (Defaults.SlowI == 1) {
      SetSource(&RemoteTPB, RemoteMemory, VMEATTR);
   } else {
      SetSource(&RemoteTPB, RemoteMemory, VMESCBLTATTR);
   }
   SetSink(&RemoteTPB, MCPLADDR, MCPLATTR);
   SetLength(&RemoteTPB, Defaults.TLength);
   if (Defaults.Locked == 1)
   {
      McplCW = BuildMcplCW(LocalNode, EntryIn, DTX, TM_LOCKED, XFERMODE, TM_CONN);
   } else {
      McplCW = BuildMcplCW(LocalNode, EntryIn, DTX, TM_BURST, XFERMODE, TM_CONN);
   }
   SetMcplCW(&RemoteTPB, McplCW);
   SetMbox(&RemoteTPB, 0x12001800, VMEATTR);
   SetNackCount(&RemoteTPB, Defaults.NackCount*10);

   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD,ExeTPB,&RemoteTPB,RemNode,EntryOut);
   if (NOERROR != RetVal) {
      printf("BuildTableLoad remote failed, %d\n", RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote V-M Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote V-M Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tRemoteTPB");
      DumpTPB(&RemoteTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemoteTPB");
      ChangeTPB(&RemoteTPB);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   dtl_ok = 0;
   while (dtl_ok == 0) {
      *Mbox = NO_MAIL;
      RetVal = ExecuteTPB(MYBOARD, ExeTPB);
      if (NOERROR != CheckError(RetVal)){
          return(-1);
      }
      /* Check completion */
      Timeout(Mbox);
      printmsg("Loading V-M "); CheckMBX(RemNode, Mbox);
      MBXData = (unsigned int)*Mbox;
      switch (MBXData) {
      case   XMIT_OK: dtl_ok = 1;
	      break;
      case   VtoM_NACK:  break;
      default:	      printf("Node %d going InValid\n", RemNode);
	      Valid[RemNode] = 0;
	      return(-1);
      }
      *Mbox = NO_MAIL;
      /* if user input happens, break and return */
      if (statrx1() == 1) {
          printf("User break\n");
          return(1);
      }
   }
   RemoteLoadedRTX[RemNode]=0;
   RemoteLoadedOut[RemNode]=1;
   return(0);
}

/* InitRemoteDispatchTo
* Load Remote dispatch tables EntryIn M-V FOR TARGET
*/
int
InitRemoteDispatchTo(unsigned int RemNode)
{
struct TPB RemoteTPB, *ExeTPB;
unsigned short *Mbox;
ErrorType RetVal;
MCPLCWType McplCW;
unsigned int dtl_ok, MBXData;

   /* Do nothing if Reload=0 and Remote has valid entry */
   if ( ( Defaults.Reload == 0) && (RemoteLoadedIn[RemNode] == 1) ) {
      return(0);
   }

   /* Build and Load Remote (M-V)*/
   CopyRemoteDefault(MYBOARD, &RemoteTPB);
   SetSource(&RemoteTPB, MCPLADDR, MCPLATTR);
   if (Defaults.SlowT == 1) {
      SetSink(&RemoteTPB, RemoteMemory, VMEATTR);
      } else {
      SetSink(&RemoteTPB, RemoteMemory, VMESCBLTATTR);
   }
   SetLength(&RemoteTPB, Defaults.TLength);

   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &RemoteTPB, RemNode, EntryIn);
   if (NOERROR != RetVal) {
      printf("Error building remote loader - "); CheckError(RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote M-V TPB");
      DumpTPB(&RemoteTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote M-V TPB");
      ChangeTPB(&RemoteTPB);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   dtl_ok = 0;
   while (dtl_ok == 0) {
      *Mbox = NO_MAIL;
      RetVal = ExecuteTPB(MYBOARD, ExeTPB);
      if (NOERROR != CheckError(RetVal)){
          return(-1);
      }
      /* Check completion */
      Timeout(Mbox);
      printmsg("Loading M-V"); CheckMBX(RemNode, Mbox);
      MBXData = (unsigned int)*Mbox;
      switch (MBXData) {
      case   XMIT_OK: dtl_ok = 1;
	            break;
      case   VtoM_NACK:  break;
      case   VtoM_TIMEOUT:
	            *Mbox = NO_MAIL;
	            Valid[RemNode] = 0;
	            return(-1);
	            break;
      default:	      printf("Node %d going InValid\n", RemNode);
	            Valid[RemNode] = 0;
	            return(-1);
      }
      *Mbox = NO_MAIL;
   }
   RemoteLoadedIn[RemNode]=1;
   return(0);
}

/* InitCopyLoader
* Load Remote dispatch tables EntryIn M-V FOR TARGET  code load 
*/
int
InitCopyLoader(unsigned int RemNode)
{
struct TPB RemoteTPB, *ExeTPB;
unsigned short *Mbox;
ErrorType RetVal;
MCPLCWType McplCW;
unsigned int dtl_ok, MBXData;

/* Build and Load Remote (M-V)*/
CopyRemoteDefault(MYBOARD, &RemoteTPB);
SetSource(&RemoteTPB, MCPLADDR, MCPLATTR);
SetSink(&RemoteTPB, RemoteMemory, VMESCBLTATTR);
SetLength(&RemoteTPB, CopyCodeLength);

ExeTPB = GetSysTPB(MYBOARD);
RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &RemoteTPB, RemNode, EntryIn);
if (NOERROR != RetVal) {
printf("Error building remote loader - "); CheckError(RetVal);
return(-1);
}

if ( Defaults.ShowTPB == 1) {
printf("\tRemote Code Loader");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tRemote Code Loader");
ChangeTPB(ExeTPB);
}
if ( Defaults.ShowTPB == 1) {
printf("\tRemote CodeLoad TPB");
DumpTPB(&RemoteTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tRemote CodeLoad TPB");
ChangeTPB(&RemoteTPB);
}

Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
dtl_ok = 0;
while (dtl_ok == 0) {
*Mbox = NO_MAIL;
RetVal = ExecuteTPB(MYBOARD, ExeTPB);
if (NOERROR != CheckError(RetVal)){
 return(-1);
}
/* Check completion */
Timeout(Mbox);
printmsg("Loading Code M-V"); CheckMBX(RemNode, Mbox);
MBXData = (unsigned int)*Mbox;
switch (MBXData) {
case   XMIT_OK: dtl_ok = 1;
	      break;
case   VtoM_NACK:  break;
default:	      printf("Node %d going InValid\n", RemNode);
	      Valid[RemNode] = 0;
	      return(-1);
}
*Mbox = NO_MAIL;
}
return(0);
}

/* InitCodeLoader
* Load Remote dispatch tables EntryIn M-V FOR TARGET  code load 
*/
int
InitCodeLoader(unsigned int RemNode)
{
struct TPB RemoteTPB, *ExeTPB;
unsigned short *Mbox;
ErrorType RetVal;
MCPLCWType McplCW;
unsigned int dtl_ok, MBXData;

RemoteLoadedIn[RemNode]=0;
/* Build and Load Remote (M-V)*/
CopyRemoteDefault(MYBOARD, &RemoteTPB);
SetSource(&RemoteTPB, MCPLADDR, MCPLATTR);
SetSink(&RemoteTPB, CodeAddress, VMESCBLTATTR);
SetLength(&RemoteTPB, CodeLength);

ExeTPB = GetSysTPB(MYBOARD);
RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &RemoteTPB, RemNode, EntryIn);
if (NOERROR != RetVal) {
printf("Error building remote loader - "); CheckError(RetVal);
return(-1);
}

if ( Defaults.ShowTPB == 1) {
printf("\tRemote Code Loader");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tRemote Code Loader");
ChangeTPB(ExeTPB);
}
if ( Defaults.ShowTPB == 1) {
printf("\tRemote CodeLoad TPB");
DumpTPB(&RemoteTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tRemote CodeLoad TPB");
ChangeTPB(&RemoteTPB);
}

Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
dtl_ok = 0;
while (dtl_ok == 0) {
*Mbox = NO_MAIL;
RetVal = ExecuteTPB(MYBOARD, ExeTPB);
if (NOERROR != CheckError(RetVal)){
 return(-1);
}
/* Check completion */
Timeout(Mbox);
printmsg("Loading Code  M-V "); CheckMBX(RemNode, Mbox);
MBXData = (unsigned int)*Mbox;
switch (MBXData) {
case   XMIT_OK: dtl_ok = 1;
	      break;
case   VtoM_NACK:  break;
default:	      printf("Node %d going InValid\n", RemNode);
	      Valid[RemNode] = 0;
	      return(-1);
}
*Mbox = NO_MAIL;
}
return(0);
}

int
InitLocalDispatch()
{
struct TPB LocalTPB, *ExeTPB;
unsigned short *Mbox;
int RetVal;

if ( (Defaults.Reload == 0) && (LocalLoaded == 1) ) {
return(0);
}

/* Build and Load Local TPB (M-V) */
CopyLocalDefault(MYBOARD, &LocalTPB);
SetSource(&LocalTPB, MCPLADDR, MCPLATTR);
if (Defaults.SlowT == 1) {
SetSink(&LocalTPB, LocalMemory, VMEATTR);
} else {
SetSink(&LocalTPB, LocalMemory, VMESCBLTATTR);
}
SetLength(&LocalTPB, Defaults.TLength);
SetLink(&LocalTPB, 0x0, 0x0) ;
SetMbox(&LocalTPB, MBX01, VMEATTR);

ExeTPB = GetSysTPB(MYBOARD);
RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &LocalTPB,LocalNode,EntryIn);
if (NOERROR != RetVal) {
printf("BuildTableLoad local failed, %d\n", RetVal);
return(-1);
}

if ( Defaults.ShowTPB == 1) {
printf("\tLocal Loader");
DumpTPB(ExeTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tLocal Loader");
ChangeTPB(ExeTPB);
}
if ( Defaults.ShowTPB == 1) {
printf("\tLocal Table %d", EntryIn);
DumpTPB(&LocalTPB);
}
if ( Defaults.EditTPB == 1) {
printf("\n\tLocal Table %d", EntryIn);
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
printmsg("Local Loader "); CheckMBX(LocalNode, Mbox);
return(-1);
}
printf("Local DT Loader Entry %d ", EntryIn); CheckMBX(LocalNode, Mbox);
LocalLoaded = 1;
return(0);
}

CancelLoads()
{
int ii;

for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
RemoteLoadedIn[ii]=0;
RemoteLoadedOut[ii]=0;
RemoteLoadedRTX[ii]=0;
}
LocalLoaded = 0;
}

/* wait ticks as specified in Defaults.Wait */
wait()
{
int start;
start = lbolt;
while (start+Defaults.Wait > lbolt) ;
}
printlocalmemory()
{
printf("FYI LocalMemory = 0x%x\n", LocalMemory);
}
printmsg(char *string)
{
if (Defaults.QuietMode == 1)
return;
printf("%-20s", string);
}

#define ThreeTab "\t\t\t"
#define FiveTab "\t\t\t\t\t"

printnode(unsigned int Node)
{
int jj, ii;
char outstring[10];

if (Defaults.QuietMode == 1)
return;

for (ii=1; ii<Node; ii++) {
printf(" ");
}
printf("%d", Node);
for (jj=ii+1; ii<10; ii++) {
printf(" ");
}
}

printmbx(char *string)
{
if (Defaults.QuietMode == 1)
      return;
printf(string);
}

int 
DoRTXTest()
{
int sret, pass, ii, node;

if (CheckValid() == 0) {
DoRemoteConfig();
}
pass = 0;
for (ii=0; ii<Defaults.Repeat; ii++ ) {
pass++;
for (node=MINADDRESS; node <= MAXADDRESS; node++) {
 if (Valid[node] == 1) {
    if ( (sret = RTXFrom(node)) == -2) {
          printf("Error Break after %d passes\n", pass);
	  return(-1);
    } 
 }
 if (statrx1() == 1) {
    printf("User break after %d passes\n", pass);
    return(1);
 }
}
}
}
/* RTXFrom, gets a pattern from the remote node and places it in
* pattern area.
*/
int
RTXFrom(unsigned int FromNode)
{
char response[80];
int RetVal;
struct TPB *ExeTPB;
unsigned short *Mbox00, *Mbox01;
MCPLCWType McplCW;
unsigned int MBXData;

   if (InitRTX(FromNode) == -1) {
      printf("InitRTX failed, bailing out ...\n", FromNode);
      return(-2);
   }

   /* Build and Load Local TPB (M-V) */
   ExeTPB = GetSysTPB(MYBOARD);
   CopyLocalDefault(MYBOARD, ExeTPB);
   SetSource(ExeTPB, MCPLADDR, MCPLATTR);
   if (Defaults.SlowT == 1) {
      SetSink(ExeTPB, LocalMemory, VMEATTR);
   } else {
      SetSink(ExeTPB, LocalMemory, VMESCBLTATTR);
   }
   SetLength(ExeTPB, Defaults.TLength);
   McplCW=BuildMcplCW(FromNode, EntryOut, DTX, TM_LOCKED, XFERMODE, TM_NOCONN);
   SetMcplCW(ExeTPB, McplCW);
   SetNackCount(ExeTPB, Defaults.NackCount);

   Mbox00 = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox00 = NO_MAIL;   /* DTX to FromNode */

   if (Defaults.ShowTPB == 1 ) {
      printf("\tReceive From:");
      DumpTPB(ExeTPB);
   }
   if (Defaults.EditTPB == 1 ) {
      printf("\n\tReceive From:");
      ChangeTPB(ExeTPB);
   }

   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != RetVal) {
      printf("Error issuing receive DTX(RTX test), %d\n", RetVal);
      return(-2);
   }

   /* Wait for DTX from to completion */
   Timeout(Mbox00);
   printmsg("DTX(RTX) To "); CheckMBX(FromNode, Mbox00);
   MBXData = (unsigned int)*Mbox00;
   RetVal = 0;
   switch (MBXData) {
   case RCV_OK: break;
   case INIT_CRC_ERROR: printf ("CRC during receive from %d\n", FromNode);
	return(-1);
   case TARGET_CRC_ERROR: printf ("CRC during receive from %d\n", FromNode);
	return(-1);
   case NO_MAIL: printf("NO_MAIL, RTX from %d not received, attempting Init\n",
			FromNode);
   	         if ( ( RetVal = DoInit() ) != NOERROR ) {
		    printf("Init failed returned 0x%\n", RetVal);
		 }
		 return(-2);
		 break;
   case	MtoV_NACK: return(-1);

   default:	Valid[FromNode] = 0;
	printf("0x%x Node %d is now removed from Remote Config\n", 
		MBXData, FromNode);
	printf("cr to continue ");
	gets(response);
	return(-1);
	break;
   }
   return(0);
}

/* InitRTX
 * Load Remote dispatch table EntryOut V-M RTX
 * This routine must retry if a NACK is received
 */
int
InitRTX(unsigned int RemNode)
{
   struct TPB RemoteTPB, *ExeTPB;
   unsigned short *Mbox;
   ErrorType RetVal;
   MCPLCWType McplCW;
   unsigned int dtl_ok, MBXData;


   /* Do nothing if Reload=0 and Remote has valid entry */
   if ( ( Defaults.Reload == 0) && (RemoteLoadedRTX[RemNode] == 1) ) {
      return(0);
   }

   /* Build and Load Remote (V-M)*/
   CopyRemoteDefault(MYBOARD, &RemoteTPB);
   if (Defaults.SlowI == 1) {
      SetSource(&RemoteTPB, RemoteMemory, VMEATTR);
   } else {
      SetSource(&RemoteTPB, RemoteMemory, VMESCBLTATTR);
   }
   SetSink(&RemoteTPB, MCPLADDR, MCPLATTR);
   SetLength(&RemoteTPB, Defaults.TLength);
   if (Defaults.Locked == 1)
   {
      McplCW = BuildMcplCW(LocalNode, RemNode, RTX, 
		TM_LOCKED, XFERMODE, TM_CONN);
   } else {
      McplCW = BuildMcplCW(LocalNode, RemNode, RTX, 
		TM_BURST, XFERMODE, TM_CONN);
   }
   SetMcplCW(&RemoteTPB, McplCW);
   SetMbox(&RemoteTPB, 0, 0);
   SetNackCount(&RemoteTPB, Defaults.NackCount);

   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD,ExeTPB,&RemoteTPB,RemNode,EntryOut);
   if (NOERROR != RetVal) {
      printf("BuildTableLoad remote failed, %d\n", RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote V-M Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote V-M Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tRemoteTPB");
      DumpTPB(&RemoteTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemoteTPB");
      ChangeTPB(&RemoteTPB);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   dtl_ok = 0;
   while (dtl_ok == 0) {
      *Mbox = NO_MAIL;
      RetVal = ExecuteTPB(MYBOARD, ExeTPB);
      if (NOERROR != CheckError(RetVal)){
         return(-1);
      }
      /* Check completion */
      Timeout(Mbox);
      printmsg("Loading RTX V-M "); CheckMBX(RemNode, Mbox);
      MBXData = (unsigned int)*Mbox;
      switch (MBXData) {
      case   XMIT_OK: dtl_ok = 1;
		      break;
      case   VtoM_NACK:  break;
      default:	      printf("Node %d going InValid\n", RemNode);
                      Valid[RemNode] = 0;
                      return(-1);
      }
      *Mbox = NO_MAIL;
      /* if user input happens, break and return */
      if (statrx1() == 1) {
         printf("User break\n");
         return(1);
      }
   }
   RemoteLoadedOut[RemNode]=0;
   RemoteLoadedRTX[RemNode]=1;
   return(0);
}
int
DoAutoConfig()
{
	int address, ii;
	char response[80];
	int OldQuiet;

	while (DoInit() != NOERROR) {
           if (statrx1() == 1) {
              printf("User break issueing init in au\n");
              return(1);
	   }
	}
	   
	printf("Auto Configuration, you are node %d\n", LocalNode);
	OldQuiet = Defaults.QuietMode;
	Defaults.QuietMode = 1;
	for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
	   if (ii == LocalNode) {
		continue;
	   }
	   Valid[ii] = 0;
	   RemoteLoadedIn[ii]=0;
	   if (InitRemoteDispatchAU(ii) != -1) {
	      Valid[ii] = 1;
	   }
	   RemoteLoadedIn[ii]=0;
	   RemoteLoadedOut[ii]=0;
	   RemoteLoadedRTX[ii]=0;
	}

	Defaults.QuietMode = OldQuiet;
	printf("Valid nodes are:\n");
	for (ii=MINADDRESS; ii<=MAXADDRESS; ii++) {
		if (Valid[ii] == 1) {
		   printf("%d ", ii);
		}
	}
	printf("\n");
}
/* AU tester
* Load Remote dispatch tables EntryIn M-V FOR TARGET
*/
int
InitRemoteDispatchAU(unsigned int RemNode)
{
struct TPB RemoteTPB, *ExeTPB;
unsigned short *Mbox;
ErrorType RetVal;
MCPLCWType McplCW;
unsigned int dtl_ok, MBXData;

   /* Build and Load Remote (M-V)*/
   CopyRemoteDefault(MYBOARD, &RemoteTPB);
   SetSource(&RemoteTPB, MCPLADDR, MCPLATTR);
   if (Defaults.SlowT == 1) {
      SetSink(&RemoteTPB, RemoteMemory, VMEATTR);
      } else {
      SetSink(&RemoteTPB, RemoteMemory, VMESCBLTATTR);
   }
   SetLength(&RemoteTPB, Defaults.TLength);

   ExeTPB = GetSysTPB(MYBOARD);
   RetVal=BuildTableLoadTPB(MYBOARD, ExeTPB, &RemoteTPB, RemNode, EntryIn);
   if (NOERROR != RetVal) {
      printf("Error building remote loader - "); CheckError(RetVal);
      return(-1);
   }

   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote Loader");
      DumpTPB(ExeTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote Loader");
      ChangeTPB(ExeTPB);
   }
   if ( Defaults.ShowTPB == 1) {
      printf("\tRemote M-V TPB");
      DumpTPB(&RemoteTPB);
   }
   if ( Defaults.EditTPB == 1) {
      printf("\n\tRemote M-V TPB");
      ChangeTPB(&RemoteTPB);
   }

   Mbox = (unsigned short *)UnMapDualPort(MYBOARD, MBX00);
   *Mbox = NO_MAIL;
   RetVal = ExecuteTPB(MYBOARD, ExeTPB);
   if (NOERROR != CheckError(RetVal)){
       return(-1);
   }
   /* Check completion */
   Timeout(Mbox);
   printmsg("Loading M-V"); CheckMBX(RemNode, Mbox);
   MBXData = (unsigned int)*Mbox;
   switch (MBXData) {
   case   XMIT_OK: break;
   case   VtoM_NACK:  break;
   case   VtoM_TIMEOUT:
	         *Mbox = NO_MAIL;
	         return(-1);
	         break;
   default:      printf("Node %d going InValid\n", RemNode);
	         return(-1);
   }
   *Mbox = NO_MAIL;
   return(0);
}
