/**********************************************************************
 			MCPL TPB construction utilities
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/

 /**
 2.00	KJL	Mon Jan 22 10:29:25 EST 1990	Production Release
 **/

#include "mcpl.h"
#include "mcplproto.h"

 /**
 BuildInitTPB
    construct an Init TPB from Local Default TPB,.
    Set Link address to SYS_TPB and link attribute to Local attribute.
 return: Address of SYS_TPB
 **/
#ifdef ANSI
struct TPB *
BuildInitTPB(unsigned int BoardIndex, 
	     struct TPB *MyTPB)
#else
struct TPB *
BuildInitTPB(BoardIndex, MyTPB)
unsigned int BoardIndex;
struct TPB *MyTPB;
#endif
{
	struct TPB *SysTPB;

	VmeAddress LinkAddr;
	VmeAttributeType LinkAttr;

	CopyLocalDefault(BoardIndex, MyTPB);
	SysTPB = GetSysTPB(BoardIndex);
	LinkAddr = MapDualPort(BoardIndex, SysTPB);
	LinkAttr = GetLocalAttr(BoardIndex);
	SetLink(MyTPB, LinkAddr, LinkAttr);
	return(SysTPB);
}


/******************** BuildTableLoadTPB() ********************/
/* Build a Table Loading TPB at &LoaderTPB
** Assumptions:
**	Addresses: LoaderTPB and TableTPB are local
**	Attributes: LoaderTPB and TableTPB are local
**	Length: TPB_X_LENGTH (i.e. one TPB to load)
**	CompStatus: LocalDefault
**	LinkAddress: SysTPB
**	LinkAttibutes: SysTPB plus Wait_for_Trigger
**	McplCW: Remote load if Node != BoardArray[BoardIndex].NodeAddress
**	MboxAddress: LocalDefault
*/

#ifdef ANSI
ErrorType
BuildTableLoadTPB( unsigned int BoardIndex,
		   struct TPB * LoaderTPB,
		   struct TPB * TableTPB,
		   unsigned int Node,
	           unsigned int Entry )
#else
ErrorType
BuildTableLoadTPB( BoardIndex, LoaderTPB, TableTPB, Node, Entry )
unsigned int BoardIndex;
struct TPB * LoaderTPB;
struct TPB * TableTPB;
unsigned int Node;
unsigned int Entry ;
#endif
{
	ErrorType ReturnCode ;
	VmeAttributeType MyLocalAttr ;
	MCPLCWType MyMcplCW ;

	if (NOERROR != 
		(ReturnCode = CopyLocalDefault( BoardIndex, LoaderTPB )) )
	{
		return( ReturnCode ) ;
	}

	MyLocalAttr = GetLocalAttr(BoardIndex) ;

	/* Set up Loader TPB fields */

	CopyLocalDefault(BoardIndex, LoaderTPB);
	SetSource(LoaderTPB, MapDualPort(BoardIndex, TableTPB), MyLocalAttr) ;
	SetLength(LoaderTPB, TPB_LENGTH) ;
	SetLink(LoaderTPB, MapDualPort(BoardIndex, GetSysTPB(BoardIndex)), 
		SetLinkAttrWaitForTrigger(MyLocalAttr) ) ;

	/* Set dependencies for local vs. remote loading */
	if (Node == GetNodeAddress(BoardIndex))
	{
		/* Build a local TPB loader */
		SetSink(LoaderTPB, (VmeAddress)Entry, TABLEATTR) ;
	} else {
		/* Build a remote TPB loader */
		SetSink(LoaderTPB, MCPLADDR, MCPLATTR) ;
		MyMcplCW = BuildMcplCW(Node, Entry, DTL, TM_BURST, 
			TM_COMPELLED, TM_CONN);
		SetMcplCW(LoaderTPB, MyMcplCW);
	}
	return( ReturnCode ) ;
} /*BuildTableLoadTPB()*/

 /**
 CopyTPB
    copies contents of FromTPB to ToTPB.  
 **/
#ifdef ANSI
void 
CopyTPB(struct TPB *FromTPB, struct TPB *ToTPB)
#else
void 
CopyTPB(FromTPB, ToTPB)
struct TPB *FromTPB, *ToTPB;
#endif
{
	*ToTPB = *FromTPB;
 /** 
 This sequence is for compilers that cannot do structure copies.
	ToTPB->SourceAddress 	= FromTPB->SourceAddress;
	ToTPB->SinkAddress 	= FromTPB->SinkAddress;
	ToTPB->SrcA_SnkA 	= FromTPB->SrcA_SnkA;
	ToTPB->Length 		= FromTPB->Length;
	ToTPB->CompStatus_Nacks	= FromTPB->CompStatus_Nacks;
	ToTPB->LinkAddress 	= FromTPB->LinkAddress;
	ToTPB->LinkA_McplCW 	= FromTPB->LinkA_McplCW;
	ToTPB->MboxAddress 	= FromTPB->MboxAddress;
 **/
}

 /**
 CopyLocalDefault
    copies contents of Local Default TPB to MyTPB
 return: UN_INIT
 **/
#ifdef ANSI
ErrorType 
CopyLocalDefault(unsigned int BoardIndex, struct TPB *MyTPB)
#else
ErrorType 
CopyLocalDefault(BoardIndex, MyTPB)
unsigned int BoardIndex; struct TPB *MyTPB;
#endif
{
	if (BoardArray[BoardIndex].GlobalsUp != TRUE) {
		return(UN_INIT);
	}
	CopyTPB(&BoardArray[BoardIndex].LocalDefault, MyTPB);
	return(NOERROR);
}

 /**
 CopyRemoteDefault
    copies contents of Remote Default TPB to MyTPB
 return: UN_INIT
 **/
#ifdef ANSI
ErrorType 
CopyRemoteDefault(unsigned int BoardIndex, struct TPB *MyTPB)
#else
ErrorType 
CopyRemoteDefault(BoardIndex, MyTPB)
unsigned int BoardIndex; struct TPB *MyTPB;
#endif
{
	if (BoardArray[BoardIndex].GlobalsUp != TRUE) {
		return(UN_INIT);
	}
	CopyTPB(&BoardArray[BoardIndex].RemoteDefault, MyTPB);
	return(NOERROR);
}

 /** SetSource 
    put Source fields in MyTPB 
 **/ 

#ifdef ANSI
void 
 SetSource(struct TPB *MyTPB, 
	   VmeAddress SourceAddress, 
	   VmeAttributeType SourceAttribute)
#else
void 
 SetSource(MyTPB, SourceAddress, SourceAttribute)
 struct TPB *MyTPB; 
 VmeAddress SourceAddress; 
 VmeAttributeType SourceAttribute;
#endif
{
	MyTPB->SourceAddress = SourceAddress;
	MyTPB->SrcA_SnkA = (unsigned long)
			   ((MyTPB->SrcA_SnkA & 0x0000FFFF) | 
			     (SourceAttribute << 16)); 
}

 /** SetSink
   put Sink fields in MyTPB
 **/
#ifdef ANSI
void 
SetSink(struct TPB *MyTPB,
	VmeAddress SinkAddress,
	VmeAttributeType SinkAttribute)
#else
void 
SetSink(MyTPB, SinkAddress, SinkAttribute)
struct TPB *MyTPB;
VmeAddress SinkAddress;
VmeAttributeType SinkAttribute;
#endif
{
	MyTPB->SinkAddress = SinkAddress;
	MyTPB->SrcA_SnkA = (unsigned long)
			   ((MyTPB->SrcA_SnkA & 0xFFFF0000) | 
			     SinkAttribute); 
}

 /** SetLength
   put Length field in MyTPB
 **/
#ifdef ANSI
void 
SetLength(struct TPB *MyTPB, unsigned int Length)
#else
void 
SetLength(MyTPB, Length)
struct TPB *MyTPB; unsigned int Length;
#endif
{
	MyTPB->Length = Length;
}

 /**
 SetMcplCW
   merge McplCW field into MyTPB
 **/
#ifdef ANSI
void 
SetMcplCW(struct TPB *MyTPB, MCPLCWType MyMcplCW)
#else
void 
SetMcplCW(MyTPB, MyMcplCW)
struct TPB *MyTPB; MCPLCWType MyMcplCW;
#endif
{
	MyTPB->LinkA_McplCW = (unsigned long)
			      ((MyTPB->LinkA_McplCW & 0xFFFF0000) | 
			      MyMcplCW); 
}

 /**
 SetNackCount
   put NackCount fields in MyTPB
 **/
#ifdef ANSI
void 
SetNackCount(struct TPB *MyTPB, NackCountType NackCount)
#else
void 
SetNackCount(MyTPB, NackCount)
struct TPB *MyTPB; NackCountType NackCount;
#endif
{
	MyTPB->CompStatus_Nacks = ((MyTPB->CompStatus_Nacks & 0xFFFF0000) | 
			     NackCount); 
}


 /** SetLink
   put Link fields in MyTPB
 **/
#ifdef ANSI
void 
SetLink(struct TPB *MyTPB,
             VmeAddress LinkAddress, 
             LinkAttributeType  LinkAttribute)
#else
void 
SetLink(MyTPB, LinkAddress, LinkAttribute)
struct TPB *MyTPB;
VmeAddress LinkAddress; 
LinkAttributeType  LinkAttribute;
#endif
{
	MyTPB->LinkAddress = LinkAddress;
	MyTPB->LinkA_McplCW = (unsigned int) 
			      (( MyTPB->LinkA_McplCW & 0x0000FFFF) |
			      (LinkAttribute << 16) );
}

 /**
 SetInterrupt
    Sets MyTPB with interrupt vector base, IRQ level 
    and enable field.
    Preserves contents of mailbox field
 Return: vector base merged with level yeilding VME vector
 **/
#ifdef ANSI
unsigned short 
SetInterrupt(struct TPB *MyTPB,
             unsigned short Vector,		/* 0 - 256 */
	     unsigned short Level,		/* 1 - 7 */
             unsigned short EnableCode)		/* 0 or 1 */
#else
unsigned short 
SetInterrupt(MyTPB, Vector, Level, EnableCode)
struct TPB *MyTPB;
unsigned short Vector;		/* 0 - 256 */
unsigned short Level;		/* 1 - 7 */
unsigned short EnableCode;	/* 0 or 1 */
#endif
{
	register CompStatusType CompStatus ;
	register unsigned short RtnVector;
	register int OldMbox;

	Level &= (unsigned short)0x7 ;

	if (Level != 0) {
		CompStatus = (CompStatusType) 1<<(Level+8);
	} else CompStatus = 0;
	OldMbox = ( (MyTPB->CompStatus_Nacks & 0x00060000) >> 17);
	CompStatus |= (CompStatusType) ((Vector&0xF8) | 
					(EnableCode&0x1) |
					((OldMbox&0x03) << 1) );
	MyTPB->CompStatus_Nacks &= 0x0000FFFF ;
	MyTPB->CompStatus_Nacks |= (unsigned long) (CompStatus << 16);
	RtnVector = (Vector & 0xF8) | Level;
	return(RtnVector);
}

 /**
 SetMbox
   put Mbox fields in MyTPB
 **/
#ifdef ANSI
void 
SetMbox(struct TPB *MyTPB,
             VmeAddress MboxAddress,
	     unsigned long MboxMode)		/* 0, 1, 2, 3 */
#else
void 
SetMbox(MyTPB, MboxAddress, MboxMode)
struct TPB *MyTPB;
VmeAddress MboxAddress;
unsigned long MboxMode;		/* 0, 1, 2, 3 */
#endif
{
	MyTPB->MboxAddress = MboxAddress ;
	MyTPB->CompStatus_Nacks &= 0xFFF9FFFF ;
	MyTPB->CompStatus_Nacks |= (MboxMode&0x3) << 17 ;
}

  /**
  TPB field retrieval 
  **/


/******************** GetSourceAddress() ********************/
#ifdef ANSI
VmeAddress
GetSourceAddress( struct TPB *MyTPB )
#else
VmeAddress
GetSourceAddress( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return(MyTPB->SourceAddress) ;
}



/******************** GetSinkAddress() ********************/
#ifdef ANSI
VmeAddress
GetSinkAddress( struct TPB *MyTPB )
#else
VmeAddress
GetSinkAddress( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return(MyTPB->SinkAddress) ;
}

/******************** GetSourceAttr() ********************/
#ifdef ANSI
VmeAttributeType
GetSourceAttr( struct TPB *MyTPB )
#else
VmeAttributeType
GetSourceAttr( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((VmeAttributeType)(MyTPB->SrcA_SnkA >> 16)) ;
}

/******************** GetSinkAttr() ********************/
#ifdef ANSI
VmeAttributeType
GetSinkAttr( struct TPB *MyTPB )
#else
VmeAttributeType
GetSinkAttr( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((VmeAttributeType)((MyTPB->SrcA_SnkA) & LOW16_MASK)) ;
}

/******************** GetLength() ********************/
#ifdef ANSI
unsigned long
GetLength( struct TPB *MyTPB )
#else
unsigned long
GetLength( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return(MyTPB->Length) ;
}

/******************** GetCompStatus() ********************/
#ifdef ANSI
CompStatusType
GetCompStatus( struct TPB *MyTPB )
#else
CompStatusType
GetCompStatus( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((CompStatusType)(MyTPB->CompStatus_Nacks >>16)) ;
}

/******************** GetNackCount() ********************/
#ifdef ANSI
NackCountType
GetNackCount( struct TPB *MyTPB )
#else
NackCountType
GetNackCount( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((NackCountType)(MyTPB->CompStatus_Nacks & LOW16_MASK )) ;
}

/******************** GetMboxAddress() ********************/
#ifdef ANSI
VmeAddress
GetMboxAddress( struct TPB *MyTPB )
#else
VmeAddress
GetMboxAddress( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return(MyTPB->MboxAddress) ;
}

/******************** GetLinkAddress() ********************/
#ifdef ANSI
VmeAddress
GetLinkAddress( struct TPB *MyTPB )
#else
VmeAddress
GetLinkAddress( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return(MyTPB->LinkAddress) ;
}

/******************** GetLinkAttr() ********************/
#ifdef ANSI
LinkAttributeType
GetLinkAttr( struct TPB *MyTPB )
#else
LinkAttributeType
GetLinkAttr( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((LinkAttributeType)(MyTPB->LinkA_McplCW >> 16)) ;
}

/******************** GetMcplCW() ********************/
#ifdef ANSI
MCPLCWType
GetMcplCW( struct TPB *MyTPB )
#else
MCPLCWType
GetMcplCW( MyTPB )
struct TPB *MyTPB ;
#endif
{
	return((MCPLCWType)((MyTPB->LinkA_McplCW) & LOW16_MASK)) ;
}

 /**
 TPB Sub-field support
 **/



/******************** SetLinkAttrWaitForTrigger() ********************/
#ifdef ANSI
LinkAttributeType
SetLinkAttrWaitForTrigger( LinkAttributeType MyLinkAttr )
#else
LinkAttributeType
SetLinkAttrWaitForTrigger( MyLinkAttr )
LinkAttributeType MyLinkAttr ;
#endif
{
	return(MyLinkAttr & ~WAITFORTRIG_MASK) ;
} /* SetLinkAttrWaitForTrigger() */



/******************** ClearLinkAttrWaitForTrigger() ********************/
#ifdef ANSI
LinkAttributeType
ClearLinkAttrWaitForTrigger( LinkAttributeType MyLinkAttr )
#else
LinkAttributeType
ClearLinkAttrWaitForTrigger( MyLinkAttr )
LinkAttributeType MyLinkAttr ;
#endif
{
	return(MyLinkAttr | WFT_FALSE) ;
} /* ClearLinkAttrWaitForTrigger() */



/******************** TestLinkAttrWaitForTrigger () ********************/
#ifdef ANSI
BOOLEAN
TestLinkAttrWaitForTrigger ( struct TPB * MyTPB )
#else
BOOLEAN
TestLinkAttrWaitForTrigger ( MyTPB )
struct TPB * MyTPB ;
#endif
{
	if ( 0x0 != ((WAITFORTRIG_MASK<<16) & MyTPB->LinkA_McplCW) )
	{
		return(FALSE) ;
	} else { return(TRUE) ;
	}
} /* TestLinkAttrWaitForTrigger */



/******************** BuildMcplCW () ********************/
#ifdef ANSI
MCPLCWType
BuildMcplCW( unsigned int Node,
	     unsigned int Entry,
	     unsigned int Command,
	     unsigned int Locked_Burst,
	     unsigned int HandShakeMode,
	     unsigned int ConnectControl )
#else
MCPLCWType
BuildMcplCW( Node, Entry, Command, Locked_Burst, HandShakeMode, ConnectControl)
unsigned int Node, Entry, Command, Locked_Burst, HandShakeMode, ConnectControl;
#endif
{
	unsigned int WorkWord ;

	WorkWord = (
		((Node<<CW_NODE_SHFT) & CW_NODE_MASK) |
		((Entry<<CW_ENT_SHFT) & CW_ENT_MASK ) |
		((Command<<CW_COM_SHFT) & CW_COM_MASK) |
		((Locked_Burst<<CW_BURST_SHFT) & CW_BURST_MASK) |
		((HandShakeMode<<CW_HAND_SHFT) & CW_HAND_MASK) |
		((ConnectControl<<CW_CONN_SHFT) & CW_CONN_MASK) );
	return( (MCPLCWType)WorkWord ) ;
}

/* end of mcpltpb.c */
