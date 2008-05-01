#include "/usr/mcpl/lib/mcpl.h"
#include "/usr/mcpl/lib/mcplproto.h"
main()
{
	char response[80];
	int McplCW;

	printf("MCPL CW: ");
	gets(response);
	while (response[0] != 'q') {
	   sscanf(response,"%x", &McplCW);
	   printf("mcplcw = 0x%x\n", McplCW);
	   PrintMcplCW((MCPLCWType)McplCW);
	   printf("\n");
	   printf("MCPL CW: ");
	   gets(response);
	}
}
PrintMcplCW(McplCW)
MCPLCWType McplCW;
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
	case 4:	Command = " DISABLE DT";
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
