.KEEP_STATE:

INCLUDE = /ELDORA/vx/hskp/include
BIN = /ELDORA/vx/hskp/bin
CFLAGS= -I/vx/h -I${INCLUDE} -c

GETOBJS  = ecbGetTsami.o ecbGetTsamr.o
INTOBJS  = ecbCmpltIsr.o ecbIntInit.o
SETOBJS  = ecbSetIF.o ecbSetDDS.o ecbSetAtten.o
SHOWOBJS = ecbShowLast.o ecbShowMstat.o ecbShowTempi.o ecbShowTempr.o
UTILOBJS = ecbGiveSem.o fifotst1.o

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

ecbGetTsami.o: ecbGetTsami.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbGetTsami.c 

ecbGetTsamr.o: ecbGetTsamr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbGetTsamr.c 


#modules in ecbInts.o:

ecbCmpltIsr.o: ecbCmpltIsr.c ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbStat.h ${INCLUDE}/hskpInt.h
	g68 ${CFLAGS} ecbCmpltIsr.c 

ecbIntInit.o: ecbIntInit.c ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbStat.h ${INCLUDE}/hskpInt.h
	g68 ${CFLAGS} ecbIntInit.c 


# modules in ecbSets.o:

ecbSetDDS.o: ecbSetDDS.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbSetDDS.c 

ecbSetIF.o: ecbSetIF.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbSetIF.c 

ecbSetAtten.o: ecbSetAtten.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbSetAtten.c 


# modules in ecbShows.o:

ecbShowLast.o: ecbShowLast.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowLast.c 

ecbShowMstat.o: ecbShowMstat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowMstat.c 

ecbShowTempi.o: ecbShowTempi.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowTempi.c 

ecbShowTempr.o: ecbShowTempr.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbAdr.h
	g68 ${CFLAGS} ecbShowTempr.c 


# modules in ecbUtils.o:

ecbGiveSem.o: ecbGiveSem.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbSem.h
	g68 ${CFLAGS} ecbGiveSem.c 

fifotst1.o: fifotst1.c ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} fifotst1.c
	cp fifotst1.o ${BIN}


