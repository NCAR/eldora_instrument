#
#       Makefile for [Pacakge? ]
#       Author: Rich Neitzel
#       Date:   Fri Sep 30 1994
#
# GNU C++ 
CCC= gcc68k
CC= ${CC}
LD= ld68k
AR= ar68k
RANLIB= ranlib68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I/local/include/vx++ -I../include -I/ELDORA/vx/c++/include
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CFLAGS= ${CCFLAGS}
#
LIBS=
#
OBJS= ColorEditor.o
#
SRCS= ColorEditor.cc
#
HEADERS= ColorEditor.hh
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

all: $(OBJS)
