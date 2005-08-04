/* CheckOut V8
 * MCPL Validation
 * Interactive Function Tester
 * V8 changes to 2.0 library
 */ 
#define VERSION "8.2"

/*
KJL:Wed Jul  5 09:53:23 EDT 1989
KJL:Thu Jul  6 09:56:22 EDT 1989
KJL:Mon Jul 10 10:30:59 EDT 1989
KJL:Wed Jul 19 16:07:45 EDT 1989
KJL:Thu Jul 20 14:13:21 EDT 1989
KJL:Tue Aug 15 11:02:35 EDT 1989
KJL:Wed Aug 16 15:21:14 EDT 1989
KJL:Fri Sep  8 14:22:46 EDT 1989
KJL:Mon Sep 18 14:03:16 EDT 1989
KJL:Thu Sep 21 14:35:26 EDT 1989
*/

#include "easisys.h"
#include <stdio.h>

/******************************************************************************

This program provides an interactive exerciser for MCPL funtions.  

Funtions supported:

1. Initialization
	- Load Init TPB
	- Receive InitAck Interrupt & Mailbox

2. Broadcast (To Be Suppplied)
	- Load Broadcast TPB 
	- Receive Broadcast Interrupt & Mailbox
	- Receive Broadcast Data

3. Perform local DMA transfers
	- Range of Addresses and Sizes
	- Range of Addressing mechanisms (scblt, A32, A24, etc)

4. Local Dispatch Loading
	- Load all entries
	- Unload entries and verify

4.1. Local Dispatch Table Analysis
	- Unload all entries and verify

5. Remote Configuration
	- Specify valid remote node addresses
	- Specify valid remote VMEbus addresses
	- Automatic attempt to verify readiness of remotes, adjust
		valid list accordingingly.

5.1 Remote Dispatch table load
	- load all entries on remotely configured machines

6. Mailbox checkout
	- Attempt to generate as many mailbox errors as possible.

7. Local to Remote Tranfers
	- Range of Addresses and Sizes
	- Range of Addressing mechanisms (scblt, A32, A24, etc)
	- Range of MCPL transfer mechanisms
8. Remote to Local Transfers
	- Range of Addresses and Sizes
	- Range of Addressing mechanisms (scblt, A32, A24, etc)
	- Range of MCPL transfer mechanisms
9. TPB chaining

10. Help and Exit

11. Display/Set Defaults

12. Monitor the status register for changes that may occur during MCPL
operations.

13. Send then receive from Valid list.

14. Load code, from 0x2000, 

15. Copy code/CheckCode.

16. Broadcast recive, Broadcast Send

17. Broadcast Data Send

18. Broadcast interrupts

19. rtx test

20. expand mcpl control word

21. Auto Configure

22. Init does HOLD and RESET command added.
23. VME interrupt.
24. Edit TPB
25. Clear Table Entry
26. Hold command
27. CONN control for MCPL CW
28. DTD test
29. RTX, Stashed, Init TPB unloading
*******************************************************************************/

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

struct defaults Defaults;


/* These constants are used to index the Commands array */

#define   NoCommand	-1
#define   Init 		0
#define   LocalXfer	1
#define   LocalLoad	2
#define   RemoteConfig	3
#define   RemoteLoad	4
#define   MailCheck	5
#define   LocalRemote	6
#define   RemoteLocal	7
#define   Chaining	8
#define   SetDefaults   9
#define   LocalAnalyse  10
#define	  Monitor	11
#define   SendReceive   12
#define   Load		13
#define   CopyCode      14
#define   CheckCode     15
#define   BroadcastSend 16
#define   BroadcastReceive 17
#define   BroadcastData 18
#define   BroadcastInit 19
#define   BroadcastCount 20
#define   RTXTest	21
#define   Decode	22
#define   AutoConfig	23
#define   Reset		24
#define   VmeIrq	25
#define   ClearTable	26
#define   Hold		27
#define   DTDis		28
#define   Unload	29
#define   Exit		30
#define   Help		31
#define   LASTCOMM 	Help /* This must be the last command */

int DoInit();
int DoLocalXfer();
int DoLocalLoad();
int DoRemoteConfig();
int DoRemoteLoad();
int DoMailCheck();
int DoLocalRemote();
int DoLocalRemoteVmeirq();
int DoRemoteLocal();
int DoChaining();
int DoDefaults();
int DoLocalAnalyse();
int DoMonitor();
int DoSendReceive();
int DoLoad();
int DoCopyCode();
int DoCheckCode();
int DoHelp();
int DoExit();
int DoBCSend();
int DoBCRecv();
int DoBCData();
int DoBCInit();
int DoBCCount();
int DoRTXTest();
int DoDecode();
int DoAutoConfig();
int DoReset();
int DoClearTable();
int DoHold();
int DoDTDis();
int DoUnload();

struct commands {
   char *description;
   char *cmdstr;
   int (*handler)();
   } Commands[LASTCOMM+1];

extern int LocalNode;

main()
{

   ivh_enb_irq(5);	/* enable VME interrrupt 6 */
   ivh_enb_lrq(7);	/* enable abort switch */
    
   InitCommands(); 
   InitLibrary();
   ClearValid(); 

   printf("\nWelcome to MCPL exerciser (Node %d) (version %s)\n", 
		LocalNode, VERSION);

   /* Get Command & Process It */
   for (;;) {
      Execute(GetCommand());
   }
}


InitCommands()
{
	int ii;

	Commands[Init].cmdstr = "init";
	Commands[Init].description = "Initialize Local Board";
	Commands[Init].handler = DoInit;

	Commands[LocalXfer].cmdstr = "lxfer";
	Commands[LocalXfer].description = "Local Transfer";
	Commands[LocalXfer].handler = DoLocalXfer;

	Commands[LocalLoad].cmdstr = "lload";
	Commands[LocalLoad].description = "Local Dispatch Table Load";
	Commands[LocalLoad].handler = DoLocalLoad;

	Commands[RemoteConfig].cmdstr = "rconfig";
	Commands[RemoteConfig].description = "Remote Configuration";
	Commands[RemoteConfig].handler = DoRemoteConfig;

	Commands[RemoteLoad].cmdstr = "rload";
	Commands[RemoteLoad].description = "Remote Dispatch Table Load";
	Commands[RemoteLoad].handler = DoRemoteLoad;

	Commands[MailCheck].cmdstr = "mcheck";
	Commands[MailCheck].description = "Mailbox Checkout";
	Commands[MailCheck].handler = DoMailCheck;

	Commands[LocalRemote].cmdstr = "ltor";
	Commands[LocalRemote].description = "LocalToRemote";
	Commands[LocalRemote].handler = DoLocalRemote;

	Commands[RemoteLocal].cmdstr = "rtol";
	Commands[RemoteLocal].description = "Remote To Local";
	Commands[RemoteLocal].handler = DoRemoteLocal;

	Commands[Chaining].cmdstr = "chain";
	Commands[Chaining].description = "TPB Chaining";
	Commands[Chaining].handler = DoChaining;

	Commands[SetDefaults].cmdstr = "default";
	Commands[SetDefaults].description = "Display and set defaults";
	Commands[SetDefaults].handler = DoDefaults;

	Commands[LocalAnalyse].cmdstr = "lanalysis";
	Commands[LocalAnalyse].description = "Analyse local Dispatch table";
	Commands[LocalAnalyse].handler = DoLocalAnalyse;

	Commands[Monitor].cmdstr = "monitor";
	Commands[Monitor].description = "Monitor Status register for change";
	Commands[Monitor].handler = DoMonitor;

	Commands[SendReceive].cmdstr = "sendreceive";
	Commands[SendReceive].description = "Send then receive";
	Commands[SendReceive].handler = DoSendReceive;

	Commands[Load].cmdstr = "load";
	Commands[Load].description = "Load Code";
	Commands[Load].handler = DoLoad;

	Commands[CopyCode].cmdstr = "copycode";
	Commands[CopyCode].description = "CopyCode Code";
	Commands[CopyCode].handler = DoCopyCode;

	Commands[CheckCode].cmdstr = "checkcode";
	Commands[CheckCode].description = "CheckCode ";
	Commands[CheckCode].handler = DoCheckCode;

	Commands[BroadcastSend].cmdstr = "bsend";
	Commands[BroadcastSend].description = "Broadcast Send ";
	Commands[BroadcastSend].handler = DoBCSend;

	Commands[BroadcastReceive].cmdstr = "brecv";
	Commands[BroadcastReceive].description = "Broadcast Receive ";
	Commands[BroadcastReceive].handler = DoBCRecv;

	Commands[BroadcastData].cmdstr = "bdata";
	Commands[BroadcastData].description = "Broadcast Data Send ";
	Commands[BroadcastData].handler = DoBCData;

	Commands[BroadcastInit].cmdstr = "binit";
	Commands[BroadcastInit].description = "Broadcast Interrupt Initialize";
	Commands[BroadcastInit].handler = DoBCInit;

	Commands[BroadcastCount].cmdstr = "bcount";
	Commands[BroadcastCount].description = "Broadcast Interrupt Counter";
	Commands[BroadcastCount].handler = DoBCCount;

	Commands[RTXTest].cmdstr = "recv";
	Commands[RTXTest].description = "RTX Test";
	Commands[RTXTest].handler = DoRTXTest;

	Commands[Decode].cmdstr = "code";
	Commands[Decode].description = "Decode MCPL word";
	Commands[Decode].handler = DoDecode;

	Commands[AutoConfig].cmdstr = "autoconfig";
	Commands[AutoConfig].description = "Automatic configuration";
	Commands[AutoConfig].handler = DoAutoConfig;

	Commands[Reset].cmdstr = "reset";
	Commands[Reset].description = "Issue Reset";
	Commands[Reset].handler = DoReset;

	Commands[VmeIrq].cmdstr = "vmeirq";
	Commands[VmeIrq].description = "Test VME IRQ";
	Commands[VmeIrq].handler = DoLocalRemoteVmeirq;

	Commands[ClearTable].cmdstr = "cleartab";
	Commands[ClearTable].description = "Clear Table Entry";
	Commands[ClearTable].handler = DoClearTable;

	Commands[Hold].cmdstr = "hold";
	Commands[Hold].description = "Issue Hold";
	Commands[Hold].handler = DoHold;

	Commands[DTDis].cmdstr = "dtd";
	Commands[DTDis].description = "Perform Init with DTD";
	Commands[DTDis].handler = DoDTDis;

	Commands[Unload].cmdstr = "unload";
	Commands[Unload].description = "unload hidden tables";
	Commands[Unload].handler = DoUnload;

	Commands[Exit].cmdstr = "exit";
	Commands[Exit].description = "Exit";
	Commands[Exit].handler = DoExit;

	Commands[Help].cmdstr = "help";
	Commands[Help].description = "Help";
	Commands[Help].handler = DoHelp;

	Defaults.Repeat = 100000000;
	Defaults.SlowI = 0;
	Defaults.SlowT = 0;
	Defaults.ShowTPB = 0;
	Defaults.Entries = 32;
	Defaults.PatternCheck = 1;
	Defaults.Reload = 0;
	Defaults.Wait = 0;
	Defaults.NackCount = 1000;
	Defaults.QuietMode = 0;
	Defaults.Locked = 0;
	Defaults.EditTPB = 0;

#define SegmentSize 0x30000  /* Bytes */
#define TransferLength	((SegmentSize/4)-1)

	Defaults.TLength = TransferLength;
	/* Special !!! */
	/* Defaults.TLength = 0x1ff; */

}

/* Returns Commands index */
int 
GetCommand()
{
   int CIndex;
   char response[80];

   CIndex = NoCommand;
   while (CIndex == NoCommand) {
	   /* Display Prompt */
	   printf("MCPL:%d: ", LocalNode);
	   /* Get Input  (gets discards newline) */
	   gets(response);
	   /* Parse It and assign command code */
	   if (strlen(response) != 0) {
	      CIndex = parsecommand(response);
	   }
   }
   return(CIndex);
}

/* returns Command Index */
int
parsecommand(char *response)
{
   int ii, RetComm;

   RetComm = NoCommand;


   for (ii=0; ii<=LASTCOMM; ii++) {


      if ((Commands[ii].handler != (int *)0) &&
          (strncmp(response, Commands[ii].cmdstr, strlen(response)) == 0)){

         if (RetComm == NoCommand) {
	    RetComm = ii;
	    continue;
	 } else {
	    printf("%s is an AMBIGUOUS command\n", response);
	    return(NoCommand);
	 }
      }
   }
   if (RetComm == NoCommand) {
      printf("%s is an UNKNOWN command\n", response);
   }
   return(RetComm);
}

int
Execute(int CIndex)
{
	(*(Commands[CIndex].handler))();
	return(0);
}

int
DoHelp()
{
   int ii;

   printf("MCPL exerciser:\n\n");
   for (ii=0; ii<=LASTCOMM; ii++) {
      if (Commands[ii].handler != (int *)0) {
	printf("%s - %s\n", Commands[ii].cmdstr, Commands[ii].description);
      }
   }
}

int
DoExit()
{
	printf("bye...\n");
	exit(1);
}

int DoDefaults()
{
	char response[80];

	printdefaults();

	printf("Repeat : %d : ", Defaults.Repeat);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.Repeat);
	}

	printf("Quiet Mode : %d : ", Defaults.QuietMode);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.QuietMode);
	}

	printf("Locked Mode : %d : ", Defaults.Locked);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.Locked);
	}

	printf("NackCount : %d : ", Defaults.NackCount);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.NackCount);
	}

	printf("SlowI : %d : ", Defaults.SlowI);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.SlowI);
		CancelLoads();
	}

	printf("SlowT : %d : ", Defaults.SlowT);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.SlowT);
		CancelLoads();
	}

	printf("Pattern Check : %d : ", Defaults.PatternCheck);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.PatternCheck);
	}

	printf("Reload : %d : ", Defaults.Reload);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.Reload);
	}

	printf("TLength (hex) : %x : ", Defaults.TLength);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%x", &Defaults.TLength);
		CancelLoads();
	}

	printf("Wait ticks : %d : ", Defaults.Wait);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.Wait);
	}

	printf("Show TPB : %d : ", Defaults.ShowTPB);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.ShowTPB);
	}

	printf("Edit TPB : %d : ", Defaults.EditTPB);
	gets(response);
	if (response[0] == '.') {
	   printdefaults();
	   return(0);
	}
	if (strlen(response) != 0) {
		sscanf(response, "%d", &Defaults.EditTPB);
	}

	printdefaults();
}

printdefaults()
{
	printf("   Repeat Quiet Locked NackC SlowI SlowT Check Reload TLength Wait Show Edit\n");
	printf("%9d %5d %6d %5d %5d %5d %5d %6d %7x %4d %4d %4d\n", 
		Defaults.Repeat, 
		Defaults.QuietMode,
		Defaults.Locked,
		Defaults.NackCount,
		Defaults.SlowI,
		Defaults.SlowT,
		Defaults.PatternCheck,
		Defaults.Reload,
		Defaults.TLength,
		Defaults.Wait,
		Defaults.ShowTPB,
		Defaults.EditTPB
		);
	printlocalmemory();
}

abo_intr()
{
	DoExit();
}
