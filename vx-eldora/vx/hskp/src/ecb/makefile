.KEEP_STATE:

INCLUDE = /ELDORA/vx/hskp/include
BIN = /ELDORA/vx/hskp/bin

# use this to compile in debug printf statements
# CFLAGS= -I/vx/h -I${INCLUDE} -c -DECBDEBUG

# use this to compile without debug printf statements
CFLAGS= -I/vx/h -I${INCLUDE} -I/ELDORA/rpc/hskp -c

GETOBJS  = ecbGetBusStat.o ecbGetTsami.o ecbGetTsamr.o ecbGetDDSool.o
INTOBJS  = ecbCmpltIsr.o ecbErrorIsr.o ecbIntInit.o
SETOBJS  = ecbSetAtten.o ecbSetDDS.o ecbSetDDSfloat.o ecbSetDivN.o ecbSetIF.o ecbSetMux.o 
SHOWOBJS = ecbShowBim.o ecbShowBusStat.o ecbShowLast.o ecbShowMstat.o ecbShowTempi.o ecbShowTempr.o ecbShowRpc.o ecbShowDDSool.o
UTILOBJS = ecbClrFifos.o ecbGiveSem.o ecbMenu.o fifotst1.o ecbTestBim.o

default: ecbGets.o ecbInts.o ecbSets.o ecbShows.o ecbUtils.o

ecbGets.o: ${GETOBJS}
	ld68k -X -r -o ecbGets.o ${GETOBJS}
	cp ecbGets.o ${BIN}

ecbInts.o: ${INTOBJS}
	ld68k -X -r -o ecbInts.o ${INTOBJS}
	cp ecbInts.o ${BIN}

ecbSets.o: ${SETOBJS}
	ld68k -X -r -o ecbSets.o ${SETOBJS}
	cp ecbSets.o ${BIN}

ecbShows.o: ${SHOWOBJS}
	ld68k -X -r -o ecbShows.o ${SHOWOBJS}
	cp ecbShows.o ${BIN}

ecbUtils.o: ${UTILOBJS}
	ld68k -X -r -o ecbUtils.o ${UTILOBJS}
	cp ecbUtils.o ${BIN}

# modules in ecbGets.o:

ecbGetBusStat.o: ecbGetBusStat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbGetBusStat.c 

ecbGetTsami.o: ecbGetTsami.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbGetTsami.c 

ecbGetTsamr.o: ecbGetTsamr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbGetTsamr.c 

ecbGetDDSool.o: ecbGetDDSool.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbGetDDSool.c 


#modules in ecbInts.o:

ecbCmpltIsr.o: ecbCmpltIsr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/hskpInt.h /ELDORA/rpc/hskp/HskpStatus.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbCmpltIsr.c 

ecbErrorIsr.o: ecbErrorIsr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/hskpInt.h /ELDORA/rpc/hskp/HskpStatus.h
	g68 ${CFLAGS} ecbErrorIsr.c 

ecbIntInit.o: ecbIntInit.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbSem.h ${INCLUDE}/hskpInt.h /ELDORA/rpc/hskp/HskpStatus.h
	g68 ${CFLAGS} ecbIntInit.c 


# modules in ecbSets.o:

ecbSetAtten.o: ecbSetAtten.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetAtten.c 

ecbSetDDS.o: ecbSetDDS.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetDDS.c 

ecbSetDDSfloat.o: ecbSetDDSfloat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetDDSfloat.c 

ecbSetDivN.o: ecbSetDivN.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetDivN.c 

ecbSetIF.o: ecbSetIF.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetIF.c 

ecbSetMux.o: ecbSetMux.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h ${INCLUDE}/ecbErrBound.h
	g68 ${CFLAGS} ecbSetMux.c 


# modules in ecbShows.o:

ecbShowBim.o: ecbShowBim.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/hskpInt.h
	g68 ${CFLAGS} ecbShowBim.c 

ecbShowBusStat.o: ecbShowBusStat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowBusStat.c 

ecbShowLast.o: ecbShowLast.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowLast.c 

ecbShowMstat.o: ecbShowMstat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} ecbShowMstat.c 

ecbShowTempi.o: ecbShowTempi.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowTempi.c 

ecbShowTempr.o: ecbShowTempr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowTempr.c 

ecbShowRpc.o: ecbShowRpc.c ${INCLUDE}/ecbFunc.h /ELDORA/rpc/hskp/HskpStatus.h
	g68 ${CFLAGS} ecbShowRpc.c 

ecbShowDDSool.o: ecbShowDDSool.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowDDSool.c 


# modules in ecbUtils.o:

ecbClrFifos.o: ecbClrFifos.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} ecbClrFifos.c 

ecbGiveSem.o: ecbGiveSem.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbSem.h
	g68 ${CFLAGS} ecbGiveSem.c 

ecbMenu.o: ecbMenu.c ${INCLUDE}/ecbFunc.h
	g68 ${CFLAGS} ecbMenu.c 

ecbTestBim.o: ecbTestBim.c 
	g68 ${CFLAGS} ecbTestBim.c 

fifotst1.o: fifotst1.c ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} fifotst1.c
	cp fifotst1.o ${BIN}


