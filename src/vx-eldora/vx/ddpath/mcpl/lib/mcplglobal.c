/**********************************************************************
 		    MCPL Library Global Data Access Utilities
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/

 /**
 2.00 	KJL	Mon Jan 22 10:51:17 EST 1990 Production Release
 **/

#include "mcpl.h"
#include "mcplproto.h"

/**
 Global Data Access Routines
 **/


/******************** InitLibGlobals() ********************/
#ifdef ANSI
void
InitLibGlobals( unsigned int BoardIndex, IV3272 BoardAddress )
#else
void
InitLibGlobals( BoardIndex, BoardAddress )
unsigned int BoardIndex ;
IV3272 BoardAddress ;
#endif
{
	struct TPB NullTPB ;

	/* Misc. Variables */
	BoardArray[BoardIndex].BoardAddress = BoardAddress;
	InitCntlRegImage(BoardIndex) ;
	InitNodeAddress(BoardIndex) ;

	/* System TPBs */
	NullTPB.SourceAddress	= 0x0 ;
	NullTPB.SinkAddress	= 0x0 ;
	NullTPB.SrcA_SnkA	= 0x0 ;
	NullTPB.Length		= 0x0 ;
	NullTPB.CompStatus_Nacks= 0x0 ;
	NullTPB.LinkAddress	= 0x0 ;
	NullTPB.LinkA_McplCW	= 0x0 ;
	NullTPB.MboxAddress	= 0x0 ;

	CopyTPB( &NullTPB, &BoardArray[BoardIndex].SysTPB ) ;
	CopyTPB( &NullTPB, &BoardArray[BoardIndex].RemoteDefault ) ;
	CopyTPB( &NullTPB, &BoardArray[BoardIndex].LocalDefault ) ;

	/* Flags */
	SetInitLoaded(BoardIndex, NOTLOADED);
	BoardArray[BoardIndex].GlobalsUp = TRUE ;
} /* InitLibGlobals() */

#ifdef ANSI
void
SetDualPortAddr(unsigned int BoardIndex,
		VmeAddress DualPortAddress,
		unsigned int DualPortLength,
		unsigned int LocalAddress,
		BOOLEAN UseLocal)		/* TRUE or FALSE */
#else
void
SetDualPortAddr(BoardIndex, DualPortAddress, DualPortLength,
		LocalAddress, UseLocal)
unsigned int BoardIndex;
VmeAddress DualPortAddress;
unsigned int DualPortLength;
unsigned int LocalAddress;
BOOLEAN UseLocal;
#endif
{
	BoardArray[BoardIndex].DualPortAddress = DualPortAddress;
	BoardArray[BoardIndex].DualPortLength = DualPortLength;
	BoardArray[BoardIndex].LocalAddress = LocalAddress;
	BoardArray[BoardIndex].UseLocal = UseLocal;
}


#ifdef ANSI
struct TPB *
GetSysTPB(unsigned int BoardIndex)
#else
struct TPB *
GetSysTPB(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(&BoardArray[BoardIndex].SysTPB);
}

#ifdef ANSI
VmeAttributeType
GetLocalAttr(unsigned int BoardIndex)
#else
VmeAttributeType
GetLocalAttr(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(BoardArray[BoardIndex].LocalAttr);
}

#ifdef ANSI
void
SetLocalAttr(unsigned int BoardIndex, VmeAttributeType SetAttribute)
#else
void
SetLocalAttr(BoardIndex, SetAttribute)
unsigned int BoardIndex; VmeAttributeType SetAttribute;
#endif
{
	BoardArray[BoardIndex].LocalAttr = SetAttribute ;
}

#ifdef ANSI
CntlRegType
GetLastCntlReg(unsigned int BoardIndex)
#else
CntlRegType
GetLastCntlReg(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(BoardArray[BoardIndex].LastCntlReg);
}

#ifdef ANSI
void
SetLastCntlReg(unsigned int BoardIndex, CntlRegType Image)
#else
void
SetLastCntlReg(BoardIndex, Image)
unsigned int BoardIndex; CntlRegType Image;
#endif
{
	BoardArray[BoardIndex].LastCntlReg = Image;
}

#ifdef ANSI
CntlRegType
GetCntlRegImage(unsigned int BoardIndex)
#else
CntlRegType
GetCntlRegImage(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(BoardArray[BoardIndex].CntlRegImage);
}

#ifdef ANSI
void
SetCntlRegImage(unsigned int BoardIndex, CntlRegType Image)
#else
void
SetCntlRegImage(BoardIndex, Image)
unsigned int BoardIndex; CntlRegType Image;
#endif
{
	BoardArray[BoardIndex].CntlRegImage = Image;
}

#ifdef ANSI
void
SetInitLoaded(unsigned int BoardIndex, BOOLEAN LoadFlag)
#else
void
SetInitLoaded(BoardIndex, LoadFlag)
unsigned int BoardIndex; BOOLEAN LoadFlag;
#endif
{
	BoardArray[BoardIndex].InitTPBLoaded = LoadFlag;
}

#ifdef ANSI
void
SetCurrentLink(unsigned int BoardIndex, VmeAddress LinkAddress,
	       LinkAttributeType LinkAttr)
#else
void
SetCurrentLink(BoardIndex, LinkAddress, LinkAttr)
unsigned int BoardIndex; VmeAddress LinkAddress; LinkAttributeType LinkAttr;
#endif
{
	BoardArray[BoardIndex].CurrentLinkAddress = LinkAddress;
	BoardArray[BoardIndex].CurrentLinkAttr = LinkAttr;
}

#ifdef ANSI
VmeAddress
GetCurrentLinkAddr(unsigned int BoardIndex)
#else
VmeAddress
GetCurrentLinkAddr(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(	BoardArray[BoardIndex].CurrentLinkAddress ) ;
}

#ifdef ANSI
LinkAttributeType
GetCurrentLinkAttr(unsigned int BoardIndex)
#else
LinkAttributeType
GetCurrentLinkAttr(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(	BoardArray[BoardIndex].CurrentLinkAttr ) ;
}
 
/******************** Control Register Image Routines ********************/

/******************** InitCntlRegImage() ********************/
/* Initialize CntlRegImage to default values
** as defined in mcpl.h header file
*/

#ifdef ANSI
void
InitCntlRegImage(unsigned int BoardIndex)
#else
void
InitCntlRegImage(BoardIndex)
unsigned int BoardIndex;
#endif
{
	CntlRegType Image;

	Image = (
		( CR_6_DEF ) |
		( CR_5_DEF ) |
		( CR_4_DEF ) |
		( CR_3_DEF ) |
		( CR_2_DEF ) |
		( CR_1_DEF ) ) ;
	SetCntlRegImage(BoardIndex, Image);
} /*InitCntlRegImage()*/


/******************** SetBusTimeout() ********************/
/* Set CntlRegImage timeout value -  field #5
** Set CntlRegImage timeout enable - field #4
*/

#ifdef ANSI
ErrorType
SetBusTimeout( unsigned int BoardIndex,
	       unsigned int timeout_value, unsigned int timeout_enable) 
#else
ErrorType
SetBusTimeout( BoardIndex, timeout_value, timeout_enable) 
unsigned int BoardIndex; unsigned int timeout_value, timeout_enable; 
#endif
{
	ErrorType ReturnCode ;

	if (NOERROR != (ReturnCode = 
		RANGE_CHK(CR_5_MIN, CR_5_MAX, timeout_value)))
	{
		return(ReturnCode);
	}
	if (NOERROR != (ReturnCode = 
		RANGE_CHK(CR_4_MIN, CR_4_MAX, timeout_enable)))
	{
		return(ReturnCode);
	}

	/* zero then set field #5 */
	BoardArray[BoardIndex].CntlRegImage &= ~CR_5_MASK ;
	BoardArray[BoardIndex].CntlRegImage |= (timeout_value<<CR_5_SHFT) ;

	/* zero then set field #4 */
	BoardArray[BoardIndex].CntlRegImage &= ~CR_4_MASK ;
	BoardArray[BoardIndex].CntlRegImage |= (timeout_enable<<CR_4_SHFT) ;

	return(ReturnCode) ;
} /*SetBusTimeout()*/



/******************** SetBusRelease() ********************/
/* Set CntlRegImage bus release mode - field #3
*/

#ifdef ANSI
ErrorType
SetBusRelease( unsigned int BoardIndex, unsigned int release_mode )
#else
ErrorType
SetBusRelease( BoardIndex, release_mode )
unsigned int BoardIndex; unsigned int release_mode ;
#endif
{
	ErrorType ReturnCode ;

	if (NOERROR != (ReturnCode = 
		RANGE_CHK(CR_3_MIN, CR_3_MAX, release_mode)))
	{
		return(ReturnCode);
	}

	/* zero then set field #3 */
	BoardArray[BoardIndex].CntlRegImage &= ~CR_3_MASK ;
	BoardArray[BoardIndex].CntlRegImage |= (release_mode<<CR_3_SHFT) ;

	return(ReturnCode) ;
} /*SetBusRelease()*/



/******************** SetFifoSize() ********************/
/* Set CntlRegImage FIFO size (width) - field #2 
*/

#ifdef ANSI
ErrorType
SetFifoSize( unsigned int BoardIndex, unsigned int fifo_size )
#else
ErrorType
SetFifoSize( BoardIndex, fifo_size )
unsigned int BoardIndex; unsigned int fifo_size ;
#endif
{
	ErrorType ReturnCode ;

	if (NOERROR != (ReturnCode = 
		RANGE_CHK(CR_2_MIN, CR_2_MAX, fifo_size)))
	{
		return(ReturnCode);
	}

	/* Check typedef FifoType compatiblity */
	if ((CR_2_DEF>>CR_2_SHFT) != fifo_size )
	{
		return(BAD_VALUE) ;
	}

	/* zero then set field #2 */
	BoardArray[BoardIndex].CntlRegImage &= ~CR_2_MASK ;
	BoardArray[BoardIndex].CntlRegImage |= (fifo_size<<CR_2_SHFT) ;

	return(ReturnCode) ;
} /*SetFifoSize*/


/******************** SetCommand() ********************/
/* Set CntlRegImage command - field #1 
*/

#ifdef ANSI
ErrorType
SetCommand( unsigned int BoardIndex, unsigned int command )
#else
ErrorType
SetCommand( BoardIndex, command )
unsigned int BoardIndex; unsigned int command ;
#endif
{
	ErrorType ReturnCode ;

	if (NOERROR != (ReturnCode = 
		RANGE_CHK(CR_1_MIN, CR_1_MAX, command)))
	{
		return(ReturnCode);
	}

	/* zero then set field #1 */
	BoardArray[BoardIndex].CntlRegImage &= ~CR_1_MASK ;
	BoardArray[BoardIndex].CntlRegImage |= (command<<CR_1_SHFT) ;

	return(ReturnCode) ;
} /*SetCommand()*/



/******************** SetCntlFields() ********************/
/* Range check each value.
/* Set all fields in CntlRegImage - fields #1-#6 
*/

#ifdef ANSI
ErrorType
SetCntlFields(unsigned int BoardIndex, unsigned int timeout_value, 
	      unsigned int timeout_enable, unsigned int release_mode, 
	      unsigned int fifo_size, unsigned int command )
#else
ErrorType
SetCntlFields(BoardIndex, timeout_value, timeout_enable, 
	      release_mode, fifo_size, command )
unsigned int BoardIndex; 
unsigned int timeout_value, timeout_enable, release_mode, fifo_size, command;
#endif
{
	ErrorType ReturnCode ;

	if (NOERROR != (ReturnCode =
		RANGE_CHK(CR_5_MIN, CR_5_MAX, timeout_value)))
	{
		return(ReturnCode);
	}
	
	if (NOERROR != (ReturnCode =
		RANGE_CHK(CR_4_MIN, CR_4_MAX, timeout_enable)))
	{
		return(ReturnCode);
	}

	if (NOERROR != (ReturnCode =
		RANGE_CHK(CR_3_MIN, CR_3_MAX, release_mode)))
	{
		return(ReturnCode);
	}
	if (NOERROR != (ReturnCode =
		RANGE_CHK(CR_2_MIN, CR_2_MAX, fifo_size)))
	{
		return(ReturnCode);
	}

	/* Check typedef FifoType compatiblity */
	if ((CR_2_DEF>>CR_2_SHFT) != fifo_size )
	{
		return(BAD_VALUE) ;
	}

	if (NOERROR != (ReturnCode =
		RANGE_CHK(CR_1_MIN, CR_1_MAX, command)))
	{
		return(ReturnCode);
	}

	SetCommand( BoardIndex, command );
	SetFifoSize( BoardIndex, fifo_size );
	SetBusRelease( BoardIndex, release_mode );
	SetBusTimeout( BoardIndex, timeout_value, timeout_enable);

	return(NOERROR) ;
} /*SetCntlFields*/

#ifdef ANSI
void
InitNodeAddress(unsigned int BoardIndex)
#else
void
InitNodeAddress(BoardIndex)
unsigned int BoardIndex;
#endif
{
	(void)SetNodeAddress(BoardIndex,
		(ReadStatusReg(BoardIndex)>>11)&0x1F);
}



/******************** SetNodeAddress() ********************/
/* Set the global variable NodeAddress
*/

#ifdef ANSI
void
SetNodeAddress( unsigned int BoardIndex, unsigned int Address )
#else
void
SetNodeAddress( BoardIndex, Address )
unsigned int BoardIndex; unsigned int Address ;
#endif
{
	BoardArray[BoardIndex].NodeAddress = Address ;
} /*SetNodeAddress()*/



/******************** GetNodeAddress() ********************/
/* Read NodeAddress from global variables
**
** return: Node Address
*/

#ifdef ANSI
unsigned int
GetNodeAddress(unsigned int BoardIndex)
#else
unsigned int
GetNodeAddress(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(BoardArray[BoardIndex].NodeAddress) ;
} /*GetNodeAddress()*/


/******************** Default TPB Setting ********************/

 /**
 Establishes user-defined defaults for globals:
 	LocalDefault
 	RemoteDefault
 Establiish default values for:
 	CurrentLinkAddress
 	CurrentLinkAttribute
 	CompStatus
 	MboxAddress
 **/

 /**
 RemoteLinkDefault
    sets the value of Link fields in Remote Default TPB
 **/
#ifdef ANSI
void 
RemoteLinkDefault(unsigned int BoardIndex, 
		  VmeAddress LinkAddress,
                  LinkAttributeType LinkAttribute)
#else
void 
RemoteLinkDefault(BoardIndex, LinkAddress, LinkAttribute)
unsigned int BoardIndex; VmeAddress LinkAddress; LinkAttributeType LinkAttribute;
#endif
{
	SetLink(&BoardArray[BoardIndex].RemoteDefault, 
		LinkAddress, LinkAttribute);
}

 /**
 LocalLinkDefault
    sets the value of Link fields in Local Default TPB
 **/
#ifdef ANSI
void 
LocalLinkDefault(unsigned int BoardIndex, 
                      VmeAddress LinkAddress, 
                      LinkAttributeType LinkAttribute)
#else
void 
LocalLinkDefault(BoardIndex, LinkAddress, LinkAttribute)
unsigned int BoardIndex; VmeAddress LinkAddress; LinkAttributeType LinkAttribute;
#endif
{
	SetLink(&BoardArray[BoardIndex].LocalDefault, 
		LinkAddress, LinkAttribute);
}

 /**
 RemoteMcplCWDefault
    Sets the remote default TPB with an McplCW
 **/

#ifdef ANSI
void
RemoteMcplCWDefault(unsigned int BoardIndex, MCPLCWType ControlWord)
#else
void
RemoteMcplCWDefault(BoardIndex, ControlWord)
unsigned int BoardIndex; MCPLCWType ControlWord;
#endif
{
	SetMcplCW( &BoardArray[BoardIndex].RemoteDefault,
		ControlWord ) ;
} /*RemoteMcplCWDefault()*/


 /**
 LocalMcplCWDefault
    Sets the remote default TPB with an McplCW
 **/
#ifdef ANSI
void
LocalMcplCWDefault(unsigned int BoardIndex, MCPLCWType ControlWord )
#else
void
LocalMcplCWDefault(BoardIndex, ControlWord )
unsigned int BoardIndex; MCPLCWType ControlWord ;
#endif
{
	SetMcplCW( &BoardArray[BoardIndex].LocalDefault,
		ControlWord ) ;
} /*LocalMcplCWDefault()*/

 /**
 RemoteNackCountDefault
    Sets the remote default TPB with an NackCount
 **/

#ifdef ANSI
void
RemoteNackCountDefault(unsigned int BoardIndex, NackCountType NackCount)
#else
void
RemoteNackCountDefault(BoardIndex, NackCount)
unsigned int BoardIndex; NackCountType NackCount;
#endif
{
	SetNackCount( &BoardArray[BoardIndex].RemoteDefault,
		NackCount ) ;
} /*RemoteNackCountDefault()*/


 /**
 LocalNackCountDefault
    Sets the remote default TPB with an NackCount
 **/
#ifdef ANSI
void
LocalNackCountDefault(unsigned int BoardIndex, NackCountType NackCount)
#else
void
LocalNackCountDefault(BoardIndex, NackCount)
unsigned int BoardIndex; NackCountType NackCount;
#endif
{
	SetNackCount( &BoardArray[BoardIndex].LocalDefault,
		NackCount) ;
} /*LocalNackCountDefault()*/



 /**
 RemoteInterruptDefault
    Sets the remote default TPB with interrupt vector base, IRQ level 
    and enable fields.
 Return: vector base merged with level yeilding VME vector
 **/
#ifdef ANSI
unsigned short 
RemoteInterruptDefault(unsigned int BoardIndex,
			    unsigned short Vector,	/* 0 to 255 */
			    unsigned short Level,	/* 1 to 7 */
			    unsigned short EnableCode)	/* 0 or 1 */
#else
unsigned short 
RemoteInterruptDefault(BoardIndex,
			    Vector,	/* 0 to 255 */
			    Level,	/* 1 to 7 */
			    EnableCode)	/* 0 or 1 */
unsigned int BoardIndex;
unsigned short Vector;	/* 0 to 255 */
unsigned short Level;	/* 1 to 7 */
unsigned short EnableCode;	/* 0 or 1 */
#endif
{
	return(SetInterrupt(&BoardArray[BoardIndex].RemoteDefault,
		      Vector, Level, EnableCode));
}

 /**
 LocalInterruptDefault
    Sets the local default TPB with interrupt vector, IRQ level 
    and enable field.
 Return: vector base merged with level yeilding VME vector
 **/
#ifdef ANSI
unsigned short 
LocalInterruptDefault(unsigned int BoardIndex,
		      unsigned short Vector,	/* 0 to 255 */
		      unsigned short Level,	/* 1 to 7 */
		      unsigned short EnableCode)	/* 0 or 1 */
#else
unsigned short 
LocalInterruptDefault(BoardIndex,
		      Vector,	/* 0 to 255 */
		      Level,	/* 1 to 7 */
		      EnableCode)	/* 0 or 1 */
unsigned int BoardIndex;
unsigned short Vector;	/* 0 to 255 */
unsigned short Level;	/* 1 to 7 */
unsigned short EnableCode;	/* 0 or 1 */
#endif
{
	return(SetInterrupt(&BoardArray[BoardIndex].LocalDefault,
		      Vector, Level, EnableCode));
}

 /**
 RemoteMboxDefault
    sets the value of Mailbox fields in Remote Default TPB
 **/
#ifdef ANSI
void 
RemoteMboxDefault(unsigned int BoardIndex,
		  VmeAddress MboxAddress,
		  unsigned long MboxMode) 		/* 0, 1, 2, 3 */
#else
void 
RemoteMboxDefault(BoardIndex,
		  MboxAddress,
		  MboxMode) 		/* 0, 1, 2, 3 */
unsigned int BoardIndex;
VmeAddress MboxAddress;
unsigned long MboxMode; 		/* 0, 1, 2, 3 */
#endif
{
	SetMbox(&BoardArray[BoardIndex].RemoteDefault, 
		MboxAddress, MboxMode);
}

 /**
 LocalMboxDefault
    sets the value of Mailbox fields in Local Default TPB
 **/
#ifdef ANSI
void 
LocalMboxDefault(unsigned int BoardIndex,
			VmeAddress MboxAddress,
			unsigned long MboxMode)	/* 0, 1, 2, 3 */
#else
void 
LocalMboxDefault(BoardIndex,
			MboxAddress,
			MboxMode)	/* 0, 1, 2, 3 */
unsigned int BoardIndex;
VmeAddress MboxAddress;
unsigned long MboxMode;	/* 0, 1, 2, 3 */
#endif
{
	SetMbox(&BoardArray[BoardIndex].LocalDefault, 
		MboxAddress, MboxMode);
}

/* end of mcplglobal.c */
