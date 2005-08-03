#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CXX= /usr/bin/gcc
CC = ${CXX}
#
OPT= -O6 -g
#
DEFINES=
#
# Warning !!!!! Hacked for dev work.
#
ELDORA= /ELDORA
#
INCLUDES= -I${ELDORA}/include -I${ELDORA}/rpc/include \
#
CXXFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CXXFLAGS} -g
#
LIBS=	-L${ELDORA}/rpc/lib -lHeader
#
DOBJS= disphead.o verify.o eldoraSwap.o

all: writeHeader disphead

writeHeader: writeHeader.o
	${CXX} -o $@ $@.o ${LIBS} -lstdc++ -g

disphead: ${DOBJS}
	${CC} -o $@ ${DOBJS} -g
