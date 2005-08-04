/**********************************************************************
 			    MCPL library prototypes
                         A portion of the MCPL library
                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
 *********************************************************************/
#ifdef ANSI

/* "mcplexe.c" Routines
*/
VmeAddress	MapDualPort(unsigned int BoardIndex, struct TPB *MappedTPB) ;
struct TPB *	UnMapDualPort(unsigned int BoardIndex, VmeAddress UnMapAddress);
ErrorType	EvalLink(unsigned int BoardIndex, struct TPB * SubjectTPB) ;
struct TPB *	FindTailTPB( unsigned int BoardIndex, VmeAddress StartTPB ) ;
void		WriteCntlRegImage( unsigned int BoardIndex ) ;
void		StartNode( unsigned int BoardIndex) ;
void		LoadTPBtoFIFO( unsigned int BoardIndex, struct TPB * LoadTPB ) ;
ErrorType	ResetNode( unsigned int BoardIndex ) ;
ErrorType	InitNode( unsigned int BoardIndex, struct TPB * InitTPB ) ;
ErrorType	ExecuteTPB( unsigned int BoardIndex, struct TPB * ExeTPB ) ;

/* "mcplglobal.c" Routines
*/

void		InitLibGlobals( unsigned int BoardIndex, IV3272 BoardAddress);
void		SetDualPortAddr(unsigned int BoardIndex,
			VmeAddress DualPortAddress,
			unsigned int DualPortLength,
			unsigned int LocalAddress,
			BOOLEAN UseLocal) ;
struct TPB *	GetSysTPB(unsigned int BoardIndex) ;
VmeAttributeType	GetLocalAttr(unsigned int BoardIndex) ;
void		SetLocalAttr(unsigned int BoardIndex, 
			     VmeAttributeType SetAttribute) ;
CntlRegType	GetLastCntlReg(unsigned int BoardIndex) ;
void		SetLastCntlReg(unsigned int BoardIndex, CntlRegType Image) ;
void		InitCntlRegImage( unsigned int BoardIndex ) ;
CntlRegType	GetCntlRegImage(unsigned int BoardIndex) ;
void		SetCntlRegImage(unsigned int BoardIndex, CntlRegType Image) ;
void		SetInitLoaded(unsigned int BoardIndex, BOOLEAN LoadFlag) ;
void		SetWaitForTrig(unsigned int BoardIndex, BOOLEAN WaitFlag) ;
BOOLEAN		GetWaitForTrig(unsigned int BoardIndex) ;
void		SetCurrentLink(unsigned int BoardIndex, VmeAddress LinkAddress,
			LinkAttributeType LinkAttr) ;
VmeAddress	GetCurrentLinkAddr(unsigned int BoardIndex) ;
LinkAttributeType GetCurrentLinkAttr(unsigned int BoardIndex) ;
ErrorType	SetBusTimeout( unsigned int BoardIndex,
			unsigned int timeout_value,
			unsigned int timeout_enable) ;
ErrorType	SetBusRelease( unsigned int BoardIndex, 
			unsigned int release_mode ) ;
ErrorType	SetFifoSize( unsigned int BoardIndex, unsigned int fifo_size ) ;
ErrorType	SetCommand( unsigned int BoardIndex, unsigned int command ) ;
ErrorType	SetCntlFields(unsigned int BoardIndex,
			unsigned int timeout_value, 
			unsigned int timeout_enable, 
			unsigned int release_mode, 
			unsigned int fifo_size, 
			unsigned int command ) ;
void		InitNodeAddress(unsigned int BoardIndex) ;
void		SetNodeAddress( unsigned int BoardIndex, unsigned int Address ) ;
unsigned int	GetNodeAddress( unsigned int BoardIndex ) ;
void		RemoteLinkDefault(unsigned int BoardIndex, 
			VmeAddress LinkAddress,
			LinkAttributeType LinkAttribute) ;
void		LocalLinkDefault(unsigned int BoardIndex, 
			VmeAddress LinkAddress, 
			LinkAttributeType LinkAttribute) ;
void		RemoteMcplCWDefault(unsigned int BoardIndex,
			MCPLCWType ControlWord ) ;
void		LocalMcplCWDefault(unsigned int BoardIndex,
			MCPLCWType ControlWord ) ;
void		RemoteNackCountDefault(unsigned int BoardIndex, 
			NackCountType NackCount) ;
void 		LocalNackCountDefault(unsigned int BoardIndex, 
			NackCountType NackCount) ;
unsigned short	RemoteInterruptDefault(unsigned int BoardIndex,
			unsigned short Vector,		/* 0 to 255 */
			unsigned short Level,		/* 1 to 7 */
			unsigned short EnableCode) ;	/* 0 or 1 */
unsigned short	LocalInterruptDefault(unsigned int BoardIndex,
			unsigned short Vector,
			unsigned short Level,
			unsigned short EnableCode) ;
void		RemoteMboxDefault(unsigned int BoardIndex,
			VmeAddress MboxAddress,
			unsigned long MboxMode) ;		/* 0, 1, 2, 3 */
void		LocalMboxDefault(unsigned int BoardIndex,
			VmeAddress MboxAddress,
			unsigned long MboxMode) ;		/* 0, 1, 2, 3 */

/* "mcplreg.c" Routines
*/


CntlRegType	ReadCntlReg(unsigned int BoardIndex) ;
void		WriteCntlReg(unsigned int BoardIndex, CntlRegType Value) ;
StatRegType	ReadStatusReg(unsigned int BoardIndex) ; 
void		WriteFifoReg(unsigned int BoardIndex, FifoType Value) ;
FifoType	ReadFifoReg(unsigned int BoardIndex) ;
void		StartCommand(unsigned int BoardIndex) ;


/* "mcpltpb.c" Routines
*/


struct TPB *	BuildInitTPB(unsigned int BoardIndex, struct TPB *MyTPB) ;
ErrorType	BuildTableLoadTPB( unsigned int BoardIndex,
			struct TPB * LoaderTPB,
			struct TPB * TableTPB,
			unsigned int Node,
			unsigned int Entry ) ;
void		CopyTPB(struct TPB *FromTPB, struct TPB *ToTPB) ;
ErrorType	CopyLocalDefault(unsigned int BoardIndex, struct TPB *MyTPB) ;
ErrorType	CopyRemoteDefault(unsigned int BoardIndex, struct TPB *MyTPB) ;
void		SetSource(struct TPB *MyTPB,
			VmeAddress SourceAddress, 
			VmeAttributeType SourceAttribute) ;
void		SetSink(struct TPB *MyTPB,
			VmeAddress SinkAddress,
			VmeAttributeType SinkAttribute) ;
void		SetLength(struct TPB *MyTPB, unsigned int Length) ;
void		SetMcplCW(struct TPB *MyTPB, MCPLCWType MyMcplCW) ;
void 		SetNackCount(struct TPB *MyTPB, NackCountType NackCount) ;
void		SetLink(struct TPB *MyTPB,
			VmeAddress LinkAddress, 
			LinkAttributeType  LinkAttribute) ;
unsigned short	SetInterrupt(struct TPB *MyTPB,
			unsigned short Vector,		/* 0 - 256 */
			unsigned short Level,		/* 1 - 7 */
			unsigned short EnableCode)	;	/* 0 or 1 */
void		SetMbox(struct TPB *MyTPB,
			VmeAddress MboxAddress,
			unsigned long MboxMode) ;		/* 0, 1, 2, 3 */
VmeAddress	GetSrcAddress( struct TPB *MyTPB ) ;
VmeAddress	GetSnkAddress( struct TPB *MyTPB ) ;
VmeAttributeType GetSrcAttr( struct TPB *MyTPB ) ;
VmeAttributeType GetSnkAttr( struct TPB *MyTPB ) ;
unsigned long	GetLength( struct TPB *MyTPB ) ;
CompStatusType	GetCompStatus( struct TPB *MyTPB ) ;
VmeAddress	GetMboxAddress( struct TPB *MyTPB ) ;
NackCountType   GetNackCount( struct TPB *MyTPB ) ;
VmeAddress	GetLinkAddress( struct TPB *MyTPB ) ;
LinkAttributeType GetLinkAttr( struct TPB *MyTPB ) ;
MCPLCWType	GetMcplCW( struct TPB *MyTPB ) ;
LinkAttributeType SetLinkAttrWaitForTrigger( LinkAttributeType MyLinkAttr ) ;
LinkAttributeType ClearLinkAttrWaitForTrigger( LinkAttributeType MyLinkAttr );
BOOLEAN		TestLinkAttrWaitForTrigger ( struct TPB * MyTPB ) ;
MCPLCWType	BuildMcplCW ( unsigned int Node,
			unsigned int Entry,
			unsigned int Command,
			unsigned int Locked_Burst,
			unsigned int HandShakeMode,
		        unsigned int ConnectControl );

/* "mcpldump.c" Routines
*/


void DumpTPB( struct TPB *dp ) ;
void print_global( unsigned int BoardIndex ) ;
void PrintAttr(VmeAttributeType Attribute);
void PrintLinkAttribute(LinkAttributeType Attribute);
void PrintMcplCW(MCPLCWType McplCW);

#else

/* "mcplexe.c" Routines
*/

VmeAddress	MapDualPort(BoardIndex, MappedTPB) ;
struct TPB *	UnMapDualPort(BoardIndex, UnMapAddress) ;
ErrorType	EvalLink(SubjectTPB) ;
struct TPB *	FindTailTPB( BoardIndex, StartTPB ) ;
void		WriteCntlRegImage( BoardIndex ) ;
void		StartNode( BoardIndex) ;
void		LoadTPBtoFIFO( BoardIndex, LoadTPB ) ;
ErrorType	ResetNode( BoardIndex ) ;
ErrorType	InitNode( BoardIndex, InitTPB ) ;
ErrorType	ExecuteTPB( BoardIndex, ExeTPB ) ;

/* "mcplglobal.c" Routines
*/

void		InitLibGlobals( BoardIndex ) ;
void		SetDualPortAddr(BoardIndex, DualPortAddress, DualPortLength,
			LocalAddress, UseLocal) ;
struct TPB *	GetSysTPB(BoardIndex) ;
VmeAttributeType	GetLocalAttr(BoardIndex) ;
void			SetLocalAttr(BoardIndex, SetAttribute) ;
CntlRegType	GetLastCntlReg(BoardIndex) ;
void		SetLastCntlReg(BoardIndex, Image) ;
void		InitCntlRegImage( BoardIndex ) ;
CntlRegType	GetCntlRegImage(BoardIndex) ;
void		SetCntlRegImage(BoardIndex, Image) ;
void		SetInitLoaded(BoardIndex, LoadFlag) ;
void		SetWaitForTrig(BoardIndex, WaitFlag) ;
BOOLEAN		GetWaitForTrig(BoardIndex) ;
void		SetCurrentLink(BoardIndex, LinkAddress, LinkAttr) ;
VmeAddress	GetCurrentLinkAddr(BoardIndex) ;
LinkAttributeType GetCurrentLinkAttr(BoardIndex) ;
ErrorType	SetBusTimeout( BoardIndex, timeout_value, timeout_enable) ;
ErrorType	SetBusRelease( BoardIndex, release_mode ) ;
ErrorType	SetFifoSize( BoardIndex, fifo_size ) ;
ErrorType	SetCommand( BoardIndex, command ) ;
ErrorType	SetCntlFields(BoardIndex, timeout_value, 
		timeout_enable, release_mode, fifo_size, command ) ;
void		InitNodeAddress(BoardIndex) ;
void		SetNodeAddress( BoardIndex, Address ) ;
unsigned int	GetNodeAddress( BoardIndex ) ;
void		RemoteLinkDefault(BoardIndex, LinkAddress, LinkAttribute) ;
void		LocalLinkDefault(BoardIndex, LinkAddress, LinkAttribute) ;
void		RemoteMcplCWDefault(BoardIndex, ControlWord ) ;
void		LocalMcplCWDefault(BoardIndex, ControlWord ) ;
void		RemoteNackCountDefault(BoardIndex, NackCount) ;
void 		LocalNackCountDefault(BoardIndex, NackCount) ;
unsigned short	RemoteInterruptDefault(BoardIndex,
			Vector,		/* 0 to 255 */
			Level,		/* 1 to 7 */
			EnableCode) ;	/* 0 or 1 */
unsigned short	LocalInterruptDefault(BoardIndex, Vector, Level, EnableCode) ;
void		RemoteMboxDefault(BoardIndex, MboxAddress, MboxMode) ;
void		LocalMboxDefault(BoardIndex, MboxAddress, MboxMode) ;

/* "mcplreg.c" Routines
*/


CntlRegType	ReadCntlReg(BoardIndex) ;
void		WriteCntlReg(BoardIndex, Value) ;
StatRegType	ReadStatusReg(BoardIndex) ;
void		WriteFifoReg(BoardIndex, Value) ;
FifoType	ReadFifoReg(BoardIndex) ;
void		StartCommand(BoardIndex) ;


/* "mcpltpb.c" Routines
*/


struct TPB *	BuildInitTPB(BoardIndex,MyTPB) ;
ErrorType	BuildTableLoadTPB( BoardIndex,LoaderTPB, TableTPB, Node, Entry);
void		CopyTPB(FromTPB, ToTPB) ;
ErrorType	CopyLocalDefault(BoardIndex, MyTPB) ;
ErrorType	CopyRemoteDefault(BoardIndex, MyTPB) ;
void		SetSource(MyTPB, SourceAddress, SourceAttribute) ;
void		SetSink(MyTPB, SinkAddress, SinkAttribute) ;
void		SetLength(MyTPB, Length) ;
void		SetMcplCW(MyTPB, MyMcplCW) ;
void 		SetNackCount(MyTPB, NackCount) ;
void		SetLink(MyTPB, LinkAddress, LinkAttribute) ;
unsigned short	SetInterrupt(MyTPB,
			Vector,		/* 0 - 256 */
			Level,		/* 1 - 7 */
			EnableCode)	;	/* 0 or 1 */
void		SetMbox(MyTPB, MboxAddress, MboxMode) ;		
VmeAddress	GetSrcAddress( MyTPB ) ;
VmeAddress	GetSnkAddress( MyTPB ) ;
VmeAttributeType GetSrcAttr( MyTPB ) ;
VmeAttributeType GetSnkAttr( MyTPB ) ;
unsigned long	GetLength( MyTPB ) ;
CompStatusType	GetCompStatus( MyTPB ) ;
VmeAddress	GetMboxAddress( MyTPB ) ;
NackCountType   GetNackCount( MyTPB ) ;
VmeAddress	GetLinkAddress( MyTPB ) ;
LinkAttributeType GetLinkAttr( MyTPB ) ;
MCPLCWType	GetMcplCW( MyTPB ) ;
LinkAttributeType SetLinkAttrWaitForTrigger( MyLinkAttr ) ;
LinkAttributeType ClearLinkAttrWaitForTrigger( MyLinkAttr );
BOOLEAN		TestLinkAttrWaitForTrigger ( MyTPB ) ;
MCPLCWType	BuildMcplCW(Node, Entry, Command, 
			Locked_Burst, HandShakeMode, ConnectControl) ;

/* "mcpldump.c" Routines
*/

void DumpTPB( dp ) ;
void print_global( BoardIndex ) ;
void PrintAttr(Attribute);
void PrintLinkAttribute(Attribute);
void PrintMcplCW(McplCW);
#endif
