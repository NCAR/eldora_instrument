.KEEP_STATE:

INCLUDE = /ELDORA/vx/hskp/include
BIN = /ELDORA/vx/hskp/bin
CFLAGS= -I/vx/h -I${INCLUDE} -c

INTOBJS  = ecbCmpltIsr.o ecbIntInit.o
SETOBJS  = ecbSetIF.o ecbSetDDS.o
GETOBJS  =
SHOWOBJS = ecbShowLast.o ecbShowMstat.o

default: ecbIntStuff.o ecbSets.o ecbShows.o fifotst1.o

ecbIntStuff.o: ${INTOBJS}
	ld68k -X -r -o ecbIntStuff.o ${INTOBJS}
	cp ecbIntStuff.o ${BIN}

ecbSets.o: ${SETOBJS}
	ld68k -X -r -o ecbSets.o ${SETOBJS}
	cp ecbSets.o ${BIN}

ecbShows.o: ${SHOWOBJS}
	ld68k -X -r -o ecbShows.o ${SHOWOBJS}
	cp ecbShows.o ${BIN}

ecbCmpltIsr.o: ecbCmpltIsr.c ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbCmpltIsr.c 

ecbIntInit.o: ecbIntInit.c ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h ${INCLUDE}/ecbStat.h ${INCLUDE}/hskpInt.h
	g68 ${CFLAGS} ecbIntInit.c 

ecbSetDDS.o: ecbSetDDS.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h
	g68 ${CFLAGS} ecbSetDDS.c 
	cp ecbSetDDS.o ${BIN}

ecbSetIF.o: ecbSetIF.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h ${INCLUDE}/ecbSem.h
	g68 ${CFLAGS} ecbSetIF.c 
	cp ecbSetIF.o ${BIN}

ecbShowLast.o: ecbShowLast.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbStat.h
	g68 ${CFLAGS} ecbShowLast.c 
	cp ecbShowLast.o ${BIN}

ecbShowMstat.o: ecbShowMstat.c ${INCLUDE}/ecbFunc.h ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} ecbShowMstat.c 
	cp ecbShowMstat.o ${BIN}

fifotst1.o: fifotst1.c ${INCLUDE}/ecbMaster.h
	g68 ${CFLAGS} fifotst1.c
	cp fifotst1.o ${BIN}


