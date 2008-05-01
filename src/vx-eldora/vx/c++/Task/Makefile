#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C++ 
CCC= gcc68k
LD= ld68k
#
OPT= -O2
#
DEFINES=
#
INCLUDES= -I/vx/h -I../include 
#
CCFLAGS= $(OPT) $(DEFINES) $(INCLUDES) #-DDEBUG
#
LIBS=
#
OBJS= Task.o
#
SRCS= Task.cc
#
HEADERS= Task.hh CDTask.hh
#
INCDIR= ../include
BINDIR= ../bin
#
CXX = ${CCC}
CXXFLAGS = ${CCFLAGS}

#
.KEEP_STATE:


install: $(OBJS)

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

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






