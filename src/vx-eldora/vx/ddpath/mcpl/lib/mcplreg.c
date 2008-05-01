/**********************************************************************
 			MCPL Register Access Routines 
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/

 /**
 2.00	KJL	Mon Jan 22 11:10:21 EST 1990	Production Release
 **/


#include "mcpl.h"
#include "mcplproto.h"


/******************** ReadCntlReg() ********************/

#ifdef ANSI
CntlRegType
ReadCntlReg(unsigned int BoardIndex)
#else
CntlRegType
ReadCntlReg(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(GetLastCntlReg(BoardIndex));
}
/******************** WriteCntlReg() ********************/

#ifdef ANSI
void
WriteCntlReg(unsigned int BoardIndex, CntlRegType Value)
#else
void
WriteCntlReg(BoardIndex, Value)
unsigned int BoardIndex; CntlRegType Value;
#endif
{
	*(CntlRegType *)( BoardArray[BoardIndex].BoardAddress 
			+ CNTLREGOFFSET) = Value ;
	SetLastCntlReg(BoardIndex, Value);
}


/******************** ReadStatusReg() ********************/

#ifdef ANSI
StatRegType 
ReadStatusReg(unsigned int BoardIndex)
#else
StatRegType 
ReadStatusReg(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return(*(StatRegType *)(BoardArray[BoardIndex].BoardAddress 
				+ STATREGOFFSET));
}


/******************** WriteFifoReg() ********************/

#ifdef ANSI
void
WriteFifoReg(unsigned int BoardIndex, FifoType Value)
#else
void
WriteFifoReg(BoardIndex, Value)
unsigned int BoardIndex; Value;
#endif
{
	*(FifoType *)(BoardArray[BoardIndex].BoardAddress 
			+ FIFOREGOFFSET) = Value;
}


/******************** ReadFifoReg() ********************/

#ifdef ANSI
FifoType 
ReadFifoReg(unsigned int BoardIndex)
#else
FifoType 
ReadFifoReg(BoardIndex)
unsigned int BoardIndex;
#endif
{
	return (*(FifoType *)(BoardArray[BoardIndex].BoardAddress 
			+ FIFOREGOFFSET));
}



/******************** StartCommand() ********************/
#ifdef ANSI
void
StartCommand(unsigned int BoardIndex)
#else
void
StartCommand(BoardIndex)
unsigned int BoardIndex;
#endif
{
	*(StartRegType *)(BoardArray[BoardIndex].BoardAddress 
			+ STARTREGOFFSET) = START_CODE;
}

 /**
 end of mcplreg.c
 **/
