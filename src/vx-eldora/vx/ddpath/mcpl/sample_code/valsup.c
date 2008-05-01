#include "/usr/mcpl/lib/mcpl.h"
#include "/usr/mcpl/lib/mcplproto.h"

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

ErrorType
CheckError(ErrorType Value)
{
	if (NOERROR == Value) return(NOERROR);

	switch (Value) {
	case ERANGE:	printf("Out of Range\n");
			break;
	case BAD_VALUE: printf("Bad Value\n");
			break;
	case BAD_LINK:  printf("BAD_LINK\n");
			break;
	case BAD_EXEC_PT:
			printf("Current link and execution address mismatch\n");
			break;
	case BD_NOT_WAITING:
			printf("Board is not waiting for trigger\n");
			break;
	case UN_INIT:	printf("Globals not initialized\n");
			break;
	case BD_NOT_BUSY:
			printf("Load Init attempted when board is not BUSY\n");
			break;
	case BD_STUCK:  printf("No response to HOLD or BUSY\n");
			break;
	default:	printf("Unknown error value = 0x%x\n", Value);
			break;
	}
	return(Value);
}

#define TOCONST 0x400000
/* #define MYBOARD 	(IV3272)		0xFFEFE000 /* My IV3272-M */
#define MYBOARD 	(unsigned int) 0 	/* 0xE000 */

int 
Timeout(unsigned short *MBXAddr)
{
	int i;
/*	for (;;) { */
	for (i=TOCONST; i>0; i--) { 
		/*
		if ((ReadStatusReg(MYBOARD) & 0x01) == 0x00) {
			printf("i=%x SR=%x\n", i, ReadStatusReg(MYBOARD));
		}
		*/
		if (*MBXAddr != NO_MAIL)
			break;
	}
	if (*MBXAddr != NO_MAIL) {
		return(0);	/* No Timeout */
	} else {
		return(1);	/* Timeout */
	}
}

CheckMBX(unsigned int Node, unsigned short *MBXAddr)
{
	unsigned int MBXData;
	int Residual, ii;
	char Buffer[80];

	printnode(Node);

	if (Defaults.QuietMode == 0)
	   printf("MBX %d ", ((int)MBXAddr&0x3C)>>2);

	MBXData = (unsigned int)*MBXAddr;

	if ( (MBXData & STATUS_TIMEOUT) == STATUS_TIMEOUT) {
	   Residual = MBXData & 0x000F;
	   MBXData = STATUS_TIMEOUT;
	}
	switch (MBXData) {
	case STATUS_TIMEOUT:
			sprintf(Buffer, "Status Timeout %d\n", Residual);
			printmbx(Buffer);
			break;
	case NO_MAIL:	printmbx("NO MAIL\n");
			break;
	case VME_OK:	printmbx("VME_OK\n");
			break;
	case RCV_OK:	printmbx("RCV_OK\n");
			break;
	case XMIT_OK:	printmbx("XMIT_OK\n");
			break;
	case LATE_VME_ERROR:
			printmbx("\t\tLate VME Error\n");
			break;
	case TARGET_CRC_ERROR:	
			printmbx("\t\t\tTarget CRC\n");
			break;
	case INIT_CRC_ERROR:	
			printmbx("\t\t\tInitiator CRC\n");
			break;
	case MtoV_NACK:	printmbx("\t\tM-V NACK\n"); break;
	case VtoM_NACK:	printmbx("\t\tV-M NACK\n"); break;
	case VtoM_RTX_NACK: printmbx("\t\tV-M NACK\n"); break;
	case MtoV_TIMEOUT:
			printmbx("\t\tM-V CW Timeout\n");
			break;
	case VtoM_TIMEOUT:
			printmbx("\t\tV-M CW Timeout\n");
			break;
	case LOCAL_VME_ERROR:
			printmbx("Local VMEbus Error\n");
			break;
	case TARGET_VME_ERROR:
			printmbx("Target VMEbus Error\n");
			break;
	case LOCAL_VME_ON_READ:   printmbx("Local VME ERROR READ\n");
			break;
	case LOCAL_VME_ON_WRITE_1:   printmbx("Local VME ERROR WRITE 1\n");
			break;
	case LOCAL_VME_ON_WRITE_2:   printmbx("Local VME ERROR WRITE 2\n");
			break;
	case HOLD_OK:   printmbx("HOLD_OK\n");
			break;
	case INIT_OK:   printmbx("Init_OK\n");
			break;
	case INIT_NOT_BUSY:
			printmbx("Load Init attempted when not BUSY\n");
			break;
	case BSIG_OK:	printmbx("Broadcast Signal Sent\n");
			break;
	case BSIG_RCV:	printmbx("Broadcast Signal Received\n");
			break;
	case BDAT_OK:	printmbx("Broadcast Data Sent\n");
			break;
	case BDAT_RCV:	printmbx("Broadcast Data Received\n");
			break;
	default:	printf("0x%x\n", MBXData);
		 	break;
	}
}
DoDecode()
{
	char response[80];
	int McplCW;

	printf("MCPL CW: ");
	gets(response);
	sscanf(response,"%x", &McplCW);
	PrintMcplCW((MCPLCWType)McplCW);
	printf("\n");
}
ChangeTPB(dp)
struct TPB *dp;
{
	char response[80];
	response[0] = '1';
	while(strlen(response) != 0) {
	   ModifyTPB(dp);
	   DumpTPB(dp);
	   printf("\nEnter <cr> to execute, other to re-edit:\n");
	   gets(response);
	}
	printf("\nDrop EDIT mode??? (yes or no):\n");
	gets(response);
	if (response[0] == 'y') {
	   Defaults.EditTPB = 0;
	}
}
ModifyTPB(dp)
struct TPB *dp;
{
	char response[80];
	int  temp;
	printf(" (Modify TPB by longword):");
	printf("\n\n Source Address : %8x : ",dp->SourceAddress);
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->SourceAddress = temp;
	}

	printf(" Sink Address   : %8x : ",dp->SinkAddress);  
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->SinkAddress = temp;
	}

	printf(" Srce/Sink Attr : %8x : ",dp->SrcA_SnkA);
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->SrcA_SnkA = temp;
	}

	printf(" Count Length   : %8x : ",dp->Length);       
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->Length = temp;
	}

	printf(" CompStat/Nacks : %8x : ",dp->CompStatus_Nacks);
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->CompStatus_Nacks = temp;
	}

	printf(" Link Address   : %8x : ",dp->LinkAddress);  
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->LinkAddress = temp;
	}

	printf(" LinkA / McplCW : %8x : ",dp->LinkA_McplCW); 
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->LinkA_McplCW = temp;
	}

	printf(" Mbox Address   : %8x : ",dp->MboxAddress);  
	gets(response);
	if (response[0] == '.') {
	   return(0);
	}
	if(strlen(response) != 0) {
	   sscanf(response,"%x",&temp);
	   dp->MboxAddress = temp;
	}
}
