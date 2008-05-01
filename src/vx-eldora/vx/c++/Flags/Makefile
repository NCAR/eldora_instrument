#
#	Makefile for Flags class.
#	Author: R. Neitzel
#	Date: 27 Dec 1990
#
# This is where to put binary file.
BINDIR= /ELDORA/vx/c++/bin
#
# This is the directory for includes.
INCDIR= /ELDORA/vx/c++/include
#
# GNU C/C++
CCC= gcc68k
#
OPT= -O2 -fno-builtin -nostdinc -nostdinc++
#
INCLUDES= -I/vw/h
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
#
LIBS=
#
# Files to build.
OBJS= Flags.o
#
# Header files
HEADERS= Flags.hh
#
# Source files
SRCS= Flags.cc
#
# Manual page.
MAN= Flags.man
#
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}
.KEEP_STATE:

all: $(OBJS)

asm:
	for i in $(SRCS); \
		do (echo "$(CCC) -S $(CCFLAGS) $$i";$(CCC) -S $(CCFLAGS) $$i); \
	done;

get_rcs:
	for i in $(SRCS); \
		do (echo "co $$i";co $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co $$i";co $$i); \
	done;
	for i in $(MAN); \
		do (co $$i) ; done;

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;
	for i in $(MAN); \
		do (co -l $$i) ; done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(MAN); \
		do (ci $$i) ; done;

set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(MAN); \
		do (rcs -c" * " -L $$i) ; done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h *.man
