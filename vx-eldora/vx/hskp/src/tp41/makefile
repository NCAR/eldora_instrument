.KEEP_STATE:

INCLUDE = /ELDORA/vx/hskp/include
BIN = /ELDORA/vx/hskp/bin
CFLAGS= -I/vx/h -I${INCLUDE} -c

default: hskpIntInit.o 

hskpIntInit.o: hskpIntInit.c ${INCLUDE}/hskpInt.h
	g68 ${CFLAGS} hskpIntInit.c 
	cp hskpIntInit.o ${BIN}

