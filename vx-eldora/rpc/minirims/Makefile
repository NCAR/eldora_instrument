#
#       Makefile for [Pacakge? ]
#       Author: Rich Neitzel
#       Date:   Thu Jul 18 1996
#
# GNU C++ 
CCC= gcc
VXCC=gcc68k
CC= ${CC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
OPT= -O2
#
DEFINES=
#
ELDORA= /ELDORA
#
INCLUDES= -I${ELDORA}/rpc/include
#
VXINCLUDES= -I/vx/h -I/local/include/vx++ $(INCLUDES)
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
VXCCFLAGS= ${OPT} ${DEFINES} ${VXINCLUDES}
CFLAGS= ${CCFLAGS}
#
LIBS=
#
UNIXOBJS=xdr_MinirimsCommand.o MinirimsClnt.o
VXOBJS=xdr_MinirimsCommand68k.o MinirimsSvr.o
#
SRCS=
#
HEADERS=
#
INC=
#
BIN=
#
LIB=
#
LIBNAME=
#
# This include cuts around the GNU make use of CXX.
include /local/include/make/sun.cc.mk
#
.KEEP_STATE:

all: libMinirimsRpc.a libVxMinirimsRpc.a

libMinirimsRpc.a: $(UNIXOBJS)
	ar crv $@ $(UNIXOBJS)

libVxMinirimsRpc.a: $(VXOBJS)
	ar68k crv $@ $(VXOBJS)

xdr_MinirimsCommand68k.o:
	$(VXCC) $(VXCCFLAGS) -DWRS_SIDE -o $@ -c xdr_MinirimsCommand.cc

MinirimsSvr.o:
	$(VXCC) $(VXCCFLAGS) -DWRS_SIDE -o $@ -c MinirimsSvr.cc
