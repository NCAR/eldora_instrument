/**********************************************************************
 		        Useful MCPL Library diagnostics
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/

/* 
 2.00	KJL	Mon Jan 22 11:11:44 EST 1990	Production Release
*/

#include "mcpl.h"
#include "mcplproto.h"

/******************** DumpTPB( dp ) ********************/
#ifdef ANSI
void
DumpTPB( struct TPB *dp )
#else
void
DumpTPB( dp )
struct TPB *dp;
#endif
{
   printf("\nDumping TPB : %8x 	 :\n", dp);

   printf(" SourceAddress\t=>  %8x\n", dp->SourceAddress );
   printf(" SinkAddress\t=>  %8x\n", dp->SinkAddress);
   printf(" SrcA_SnkA\t=>  %8x\n", dp->SrcA_SnkA );

	PrintAttr(dp->SrcA_SnkA >> 16);
	printf(" ");
	PrintAttr(dp->SrcA_SnkA & 0xFFFF);
	printf("\n");

   printf(" Length\t\t=>  %8x\n", dp->Length);
   printf(" CompStatus_Nacks=>  %8x\n", dp->CompStatus_Nacks) ;
   printf(" LinkAddress\t=>  %8x\n", dp->LinkAddress) ;
   printf(" LinkA_McplCW\t=>  %8x\n", dp->LinkA_McplCW) ;

	PrintLinkAttribute(dp->LinkA_McplCW >> 16);
	printf(" ");
	PrintMcplCW(dp->LinkA_McplCW & 0xffff);
	printf("\n");

   printf(" MboxAddress\t=>  %8x\n", dp->MboxAddress) ;
} ; /* DumpTPB(dp) */



#ifdef ANSI
void
print_global( unsigned int BoardIndex )
#else
void
print_global( BoardIndex )
unsigned int BoardIndex ;
#endif
{
	printf("\n\n*** Dumping Global Variables: BoardArray[%x] ***\n\n",
		BoardIndex ) ;

	printf("Flags:\n") ;
	printf("\tGlobalsUp\t= %x;\n",BoardArray[BoardIndex].GlobalsUp) ;
	printf("\tInitTPBLoaded\t= %x;\n",BoardArray[BoardIndex].InitTPBLoaded) ;
	printf("\tUseLocal\t= %x;\n",BoardArray[BoardIndex].UseLocal) ;

	printf("\nGlobal TPBs:\n") ;
	printf("\n\tRemoteDefault\n") ;
	DumpTPB(&BoardArray[BoardIndex].RemoteDefault) ;
	printf("\n\tLocalDefault\n") ;
	DumpTPB(&BoardArray[BoardIndex].LocalDefault) ;
	printf("\n\tSysTPB\n") ;
	DumpTPB(&BoardArray[BoardIndex].SysTPB) ;

	printf("\nMisc.:\n\n") ;
	printf("&SysTPB\t= %x;\t\tNodeAddress\t= %x;\n",
		GetSysTPB(BoardIndex),
		GetNodeAddress(BoardIndex) );
	printf("LocalAttr\t= %x;\n", GetLocalAttr(BoardIndex)) ;

	printf("\n\tDual-Port mapping:\n") ;
	printf("DualPortAddress\t= %x;\tDualPortLength\t= %x;\n",
		BoardArray[BoardIndex].DualPortAddress,
		BoardArray[BoardIndex].DualPortLength );
	printf("LocalAddress\t= %x;\n",BoardArray[BoardIndex].LocalAddress );

	printf("\n\tCurrent Link:\n") ;
	printf("CurrentLinkAddress\t= %x;\tCurrentLinkAttr\t= %x;\n",
		GetCurrentLinkAddr(BoardIndex),
		GetCurrentLinkAttr(BoardIndex) ) ;

	printf("\n\tContrl Register Images:\n") ;
	printf("CntlRegImage\t= %x;\t\tLastCntReg\t= %x;\n",
		GetCntlRegImage(BoardIndex),
		GetLastCntlReg(BoardIndex) );

	printf("\n\n*** End of Global Dump; BoardArray[%x] ***\n\n", BoardIndex );
} 

#ifdef ANSI
void
PrintAttr(VmeAttributeType Attribute)
#else
void
PrintAttr(Attribute)
VmeAttributeType Attribute;
#endif
{
	char *Location;
	int BurstSize;

	printf("<");

	switch (Attribute & 0x0003) {
	case 0: printf("-");
		break;
	case 1: printf("MCPL");
		break;
	case 2: if ((Attribute & 0x0003) == 2) {
			printf("%s"
			,((Attribute & 0x4000)		? "":
					"DISPATCH TABLE"));
		}
		break;
	case 3: switch((Attribute & 0x0070) >> 4) {
		case 0:		BurstSize = 0;
				break;
		case 1:		BurstSize = 8;
				break;
		case 2:		BurstSize = 16;
				break;
		case 3:		BurstSize = 32;
				break;
		case 4:		BurstSize = 64;
				break;
		case 5:		BurstSize = 128;
				break;
		case 6:		BurstSize = 256;
				break;
		default:	BurstSize = 0;
		}
		printf("%s%s%02x"
		,((Attribute & 0x8000)		? "STATIC ":"NORM ")
		,"AM:", (Attribute & 0x3f00)>>8
		);

		if (Attribute & 0x0080) {
			printf("%s%s%d"
			," BLT "
			,"BS:", BurstSize 
			);
		}

		printf("%s%s"
		,((Attribute & 0x0004)		? " PIPE ":"")
		,"VME"
		);
		break;
	}
	printf("> ");
}

#ifdef ANSI
void
PrintLinkAttribute(LinkAttributeType Attribute)
#else
void
PrintLinkAttribute(Attribute)
LinkAttributeType Attribute;
#endif
{
	char *Location;
	int BurstSize;

	printf("<");

	switch (Attribute & 0x0003) {
	case 0: printf("-");
		break;
	case 1: printf("-");
		break;
	case 2: printf("-");
		break;
	case 3: switch((Attribute & 0x0070) >> 4) {
		case 0:		BurstSize = 0;
				break;
		case 1:		BurstSize = 8;
				break;
		case 2:		BurstSize = 16;
				break;
		case 3:		BurstSize = 32;
				break;
		case 4:		BurstSize = 64;
				break;
		case 5:		BurstSize = 128;
				break;
		case 6:		BurstSize = 256;
				break;
		default:	BurstSize = 0;
		}

		printf("%s%s%s%02x"
		,((Attribute & 0x8000)		? "STATIC ":"NORM ")
		,((Attribute & 0x4000)		? "IMMED ":"WFT ")
		,"AM:", (Attribute & 0x3f00)>>8
		);

		if (Attribute & 0x0080) {
			printf("%s%s%d"
			," BLT "
			,"BS:", BurstSize 
			);
		}

		printf("%s%s"
		,((Attribute & 0x0004)		? " PIPE ":"")
		,"VME"
		);
		break;
	}
	printf("> ");
}
	
#ifdef ANSI
void
PrintMcplCW(MCPLCWType McplCW)
#else
void
PrintMcplCW(McplCW)
MCPLCWType McplCW;
#endif
{
	char *Command;

	switch ((McplCW & 0x38) >> 3) {
	case 0:	Command = " DTL";
		break;
	case 1:	Command = " DTX";
		break;
	case 2:	Command = " RTX";
		break;
	case 3:	Command = " DTT";
		break;
	case 4:	Command = " DTD";
		break;
	case 5:	Command = " ???";
		break;
	case 6:	Command = " BCAST SIG";
		break;
	case 7:	Command = " BCAST DATA";
		break;
	}

	if (McplCW == 0) {
		printf("<->");
	} else {
		printf("<%s%d%s%d%s%s%s%s>"
		,"DEST:"
		,((McplCW & 0xf800)>>11) 
		," TABLE:"
		,((McplCW & 0x07c0)>>6)
		,Command
		,((McplCW & 0x0004)		? " BURST" : " LOCKED")
		,((McplCW & 0x0002)		? " NON-COMP" : " COMP")
		,((McplCW & 0x0001)		? " CONN" : " NO_CONN")
		);
	}
}
