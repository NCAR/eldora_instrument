/**********************************************************************
 			MCPL Execution Support Routines
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/
/*
 2.00	KJL 	Mon Jan 22 11:01:52 EST 1990	Production Release
*/

#include "mcpl.h"
#include "mcplproto.h"


/******************** MapDualPort() ********************/
/* Map a local address to the offset used for VMEbus access.
**	Convert TPB pointer to VmeAddress.
** Globals:
**	UseLocal - if dual-port address mapping is required
**	LocalAddress - the base address for local RAM access
**	DualPortAddress - the base addr. for VME Ram access
*/

#ifdef ANSI
VmeAddress
MapDualPort(unsigned int BoardIndex, struct TPB *MappedTPB)  
#else
VmeAddress
MapDualPort(BoardIndex, MappedTPB)  
unsigned int BoardIndex; struct TPB *MappedTPB;  
#endif
{
	if (DISABLE_ADDR_MAPPING == BoardArray[BoardIndex].UseLocal)
	{
		return( (VmeAddress)MappedTPB ) ;
	}

	if (((VmeAddress)MappedTPB >= BoardArray[BoardIndex].LocalAddress) &&
	    ((VmeAddress)MappedTPB < 
	    (BoardArray[BoardIndex].LocalAddress 
	    + (VmeAddress)BoardArray[BoardIndex].DualPortLength)))
	{
		return((VmeAddress)
		       (((unsigned int) MappedTPB - 
			               BoardArray[BoardIndex].LocalAddress) +
		        (unsigned int) BoardArray[BoardIndex].DualPortAddress));

	} else {
		return( (VmeAddress)MappedTPB ) ;
	}

} /* MapDualPort() */



/******************** UnMapDualPort() ********************/
/* Map a VMEbus address to the offset used for local access.
**	Convert VmeAddress to TPB pointer.
** Globals:
**	UseLocal - if dual-port address mapping is required
**	LocalAddress - the base address for local RAM access
**	DualPortAddress - the base addr. for VME Ram access
*/

#ifdef ANSI
struct TPB *
UnMapDualPort(unsigned int BoardIndex, VmeAddress UnMapAddress)
#else
struct TPB *
UnMapDualPort(BoardIndex, UnMapAddress)
unsigned int BoardIndex; VmeAddress UnMapAddress;
#endif
{
	if (DISABLE_ADDR_MAPPING == BoardArray[BoardIndex].UseLocal)
	{
		return( (struct TPB *)UnMapAddress ) ;
	}

	if ((UnMapAddress >= BoardArray[BoardIndex].DualPortAddress) &&
	    (UnMapAddress < 
	    (BoardArray[BoardIndex].DualPortAddress 
	    + (VmeAddress)BoardArray[BoardIndex].DualPortLength)))
	{
		return((struct TPB *)(
			((unsigned int) UnMapAddress -
			(unsigned int) BoardArray[BoardIndex].DualPortAddress) +
			               BoardArray[BoardIndex].LocalAddress)) ;
	} else {
		return( (struct TPB *)UnMapAddress ) ;
	}

} /* UnMapDualPort() */



/******************** EvalLink() ********************/
/* Evaluate the links fields of a TPB for:
**	If Address = zero; Return BAD_LINK
**	else Return NOERR
*/

#ifdef ANSI
ErrorType
EvalLink(unsigned int BoardIndex, struct TPB * SubjectTPB)
#else
ErrorType
EvalLink(BoardIndex, SubjectTPB)
unsigned int BoardIndex; struct TPB * SubjectTPB;
#endif
{
	if (0 == SubjectTPB->LinkAddress) 
	{
		return( BAD_LINK ) ;
	}

	/* check longword alignment */
	if (CR_2_D32 == (GetLastCntlReg(BoardIndex) & CR_2_MASK)){
		if (((unsigned long)SubjectTPB->LinkAddress & 0x3) 
			!= 0x0)
		{
			return(BAD_ALIGNMENT) ;
		}
	}

	return( NOERROR ) ;
} /*EvalLink()*/



/******************** FindTailTPB() ********************/
/* Find Tail TPB in a linked chain 
**	UnMap and recurse down TPB Link fields;
**	Find TPB with WaitForTrigger field set;
** Return:
**	pointer to last TPB (i.e. TPB with wait for trigger) 
**	if Error; return TPBLink = 0 
** Error:
**	A TPB link fails EvalLink() 
*/

#ifdef ANSI
struct TPB *
FindTailTPB( unsigned int BoardIndex, VmeAddress StartTPB )
#else
struct TPB *
FindTailTPB( BoardIndex, StartTPB )
unsigned int BoardIndex; VmeAddress StartTPB ;
#endif
{
	struct TPB * CurrentTPB ;

	CurrentTPB = (struct TPB *) UnMapDualPort( BoardIndex, StartTPB ) ;


	if (NOERROR != EvalLink(BoardIndex, CurrentTPB) )
	{
		return(0) ;
	}

	if (TRUE == TestLinkAttrWaitForTrigger(CurrentTPB))
	{
		return(CurrentTPB) ;
	}

	return( FindTailTPB(BoardIndex, CurrentTPB->LinkAddress) ) ;
} /*FindTailTPB*/



/******************** WriteCntlRegImage() ********************/
#ifdef ANSI
void
WriteCntlRegImage( unsigned int BoardIndex )
#else
void
WriteCntlRegImage( BoardIndex )
unsigned int BoardIndex ;
#endif
{
	WriteCntlReg( BoardIndex, GetCntlRegImage(BoardIndex) ) ;
} /*WriteCntlRegImage*/



/******************** StartNode() ********************/
#ifdef ANSI
void
StartNode( unsigned int BoardIndex)
#else
void
StartNode( BoardIndex)
unsigned int BoardIndex;
#endif
{
	StartCommand( BoardIndex ) ;
} /*StartNode()*/



/******************** LoadTPBtoFIFO() ********************/
/* Load TPB into FIFO register
**	get data width (D32/D16) from Control Reg. 
**	PROBLEM: if FifoType and D32/D16 do not agree 
*/

#ifdef ANSI
void
LoadTPBtoFIFO( unsigned int BoardIndex, struct TPB * LoadTPB )
#else
void
LoadTPBtoFIFO( BoardIndex, LoadTPB )
unsigned int BoardIndex; struct TPB * LoadTPB ;
#endif
{
	int i ;

	/* Determine FIFO data width from BoardArray[].LastCntlReg */

	if (CR_2_D32 == 
		((GetLastCntlReg(BoardIndex) & CR_2_MASK)>>CR_2_SHFT) )
	{
		/* FIFO register in D32 mode */
		unsigned long *ptr ;

		ptr = (unsigned long *)LoadTPB ;
		for (i=0; i<8; i++,ptr++)
		{
			WriteFifoReg( BoardIndex, (FifoType)*ptr ) ;
		}

	} else {
		/* FIFO register in D16 mode */
		unsigned short *ptr ;

		ptr = (unsigned short *)LoadTPB ;
		for (i=0; i<16; i++,ptr++)
		{
			WriteFifoReg( BoardIndex, (FifoType)*ptr ) ;
		}
	}
} /*LoadTPBtoFIFO()*/



/******************** ResetNode() ********************/
#ifdef ANSI
ErrorType
ResetNode( unsigned int BoardIndex )
#else
ErrorType
ResetNode( BoardIndex )
unsigned int BoardIndex ;
#endif
{
	register int LoopCount;

	SetCommand(BoardIndex, CR_1_RESET) ;
	WriteCntlReg(BoardIndex, GetCntlRegImage(BoardIndex)) ;
        StartCommand( BoardIndex ) ;

	/* Wait for not BUSY */
	LoopCount = 0;
	while ( (LoopCount < 300) &&
	        (ReadStatusReg(BoardIndex) & SR_BUSY) == SR_BUSY) {
	   LoopCount++;
	}
	if ((ReadStatusReg(BoardIndex) & SR_BUSY) == SR_BUSY) {
	   return( BD_STUCK );
	}
	SetInitLoaded(BoardIndex, FALSE);
        return(NOERROR);
} /*ResetNode()*/



/******************** InitNode() ********************/

#ifdef ANSI
ErrorType
InitNode( unsigned int BoardIndex, struct TPB * InitTPB )
#else
ErrorType
InitNode( BoardIndex, InitTPB )
unsigned int BoardIndex; struct TPB * InitTPB ;
#endif
{
	VmeAddress VmeAddrofInitTPB ;
	int LoopCount;
	struct TPB * LastTPB ;

	/* Locate/Validate TPB link field(s) */
	VmeAddrofInitTPB = MapDualPort( BoardIndex, InitTPB ) ;
	if ( 0 == (LastTPB = FindTailTPB( BoardIndex, VmeAddrofInitTPB )) ) {
		return( BAD_LINK ) ;
	}

	/* If BUSY, must issue HOLD first */
	if ((ReadStatusReg(BoardIndex) & SR_BUSY) == SR_BUSY) {
	   /* Issue HOLD Command */
	   SetCommand( BoardIndex, CR_1_HOLD ) ;
	   WriteCntlReg( BoardIndex, GetCntlRegImage(BoardIndex) ) ;
	   StartCommand( BoardIndex ) ;
	   /* Wait for Status Register: NOT BUSY */
	   LoopCount = 0;
	   while ( (LoopCount < WAIT_FOR_NOT_BUSY) &&
		   (ReadStatusReg(BoardIndex) & SR_BUSY) == SR_BUSY) {
	      LoopCount++;
	   }
	   if ((ReadStatusReg(BoardIndex) & SR_BUSY) == SR_BUSY) {
	      return( BD_STUCK );
	   }
	}

	/* Issue Load Init TPB command */
	SetCommand( BoardIndex, CR_1_INIT_TPB ) ;
	WriteCntlReg( BoardIndex, GetCntlRegImage(BoardIndex) ) ;

	/* load first TPB into FIFO register */
	LoadTPBtoFIFO( BoardIndex, InitTPB ) ;

	/*  update global variables, start board, and return */
	SetCurrentLink(	BoardIndex, LastTPB->LinkAddress,
			(LinkAttributeType) ( LastTPB->LinkA_McplCW >> 16 )) ;

	/* Trigger Load Init TPB command */
	StartNode( BoardIndex ) ;     /* */
	return( NOERROR ) ;
} /*InitNode()*/



/******************** ExecuteTPB() ********************/
/* Run a TPB on the Node(BoardIndex)
** Procedure:
**	Verify and locate link chain end; abort with error: BAD_LINK 
**	Verify that ExeTPB address is at the CurrentLink address; 
**		abort with error: BAD_EXEC_PT 
**	Verify BoardIndex is WaitingForTrigger (hardware) before
**		triggering; abort with error: BD_NOT_WAITING 
**	Wait for any completion on any on-going execution 
**	Perform Trigger
**	Update CurrentLink globals; 
** Return:
**	NOERROR if TPB started
**	BAD_LINK if aborted because of bad link chain
**	BAD_EXEC_PT if abort because addr. of 
**		ExeTPB and CurrentLink mis-matched
**	BD_NOT_WAITING if abort because board was 
**		not waiting for trigger
*/

#ifdef ANSI
ErrorType
ExecuteTPB( unsigned int BoardIndex, struct TPB * ExeTPB )
#else
ErrorType
ExecuteTPB( BoardIndex, ExeTPB )
unsigned int BoardIndex; struct TPB * ExeTPB ;
#endif
{
	VmeAddress VmeAddrofExeTPB ;
	struct TPB * LastTPB ;

	VmeAddrofExeTPB = MapDualPort( BoardIndex, ExeTPB ) ;

	/* check longword alignment of ExeTPB */
	if (CR_2_D32 == (GetLastCntlReg(BoardIndex) & CR_2_MASK)){
		if (((unsigned long)ExeTPB & 0x3) 
			!= 0x0)
		{
			return(BAD_ALIGNMENT) ;
		}
	}

	/* Locate/Validate TPB link field(s) */
	if ( 0 == (LastTPB = FindTailTPB( BoardIndex, VmeAddrofExeTPB )) ) {
		return( BAD_LINK ) ;
	}

	/* Verify that ExeTPB is at the execution link (CurrentLink) */

	if ( GetCurrentLinkAddr(BoardIndex) != VmeAddrofExeTPB ) {
		return( BAD_EXEC_PT ) ;
	}

	/* Check for WaitingForTrigger Hardware flag */
	if (SR_WFT != (ReadStatusReg(BoardIndex)&SR_WFT)) {
#ifdef DEBUG
printf("No Hardware WFT flag\n");
#endif DEBUG
		return( BD_NOT_WAITING ) ;
	}

	/*  update global variables, start board, and return */
	SetCurrentLink(	BoardIndex, LastTPB->LinkAddress,
			(LinkAttributeType) ( LastTPB->LinkA_McplCW >>16 )) ;

	SetCommand( BoardIndex, CR_1_TRIG ) ;
	WriteCntlReg( BoardIndex, GetCntlRegImage(BoardIndex) ) ;
	StartNode( BoardIndex ) ;

	return( NOERROR ) ;
} /*ExecuteTPB()*/

/* end of mcplexe.c */
