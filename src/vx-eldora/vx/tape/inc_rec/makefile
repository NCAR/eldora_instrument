#
#	Makefile for Directory ELDORA/vx/tape/inc_rec
#	Author: Craig Walther
#	Date: 10/02/96
#
# GNU C 
CC= gcc68
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#	-fomit-frame-pointer
OPT= -O -fvolatile
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vw/h -I. -I/ELDORA/rpc/header2 -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/bin/vxheader.o /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS=
#
SRCS= 
#    
HEADERS= paraDef.h paraGbl.h recordDef.h recordGbl.h recordFuncs.h\
	recordIncs.h

# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/vx/hskp/bin

.KEEP_STATE:

compile: $(OBJS)

update:
	-for i in $(HEADERS); \
		do (diff $$i $(INC)/$$i >tmpdiff; \
			if test -s tmpdiff; \
			then \
				(echo "Copying $$i";cp $$i $(INC)) \
			fi;); \
	rm tmpdiff; \
	done;
		

asm:
	for i in $(SRCS); \
		do (echo "$(CC) -S $(CFLAGS) $$i";$(CC) -S $(CFLAGS) $$i); \
	done;

get_rcs:
	for i in $(HEADERS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(HEADERS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(HEADERS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,rich,eric -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h

copyem:
