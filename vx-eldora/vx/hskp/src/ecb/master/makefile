#
#	Makefile for Directory ELDORA/vx/hskp/src/ecb
#	Author: Craig Walther
#	Date: 9/2/93
#
# GNU C 
CC= gcc68
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#	-fomit-frame-pointer
OPT= -O -fvolatile -fno-builtin -nostdinc
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vw/h -I. -I/ELDORA/rpc/header -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include -I/ELDORA/rpc/hskp
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/bin/vxheader.o /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS=  alldptest.o dptest.o eedump.o eetest.o eestorenum.o eetosram.o\
fifotst.o fifotst1.o ldsrec.test.o led.o loopes.o loopse.o nvdump.o\
nvstorenum.o overnight.o pokevec.o showee.o sramtoee.o status.o\
stuffloop.o testalign.o
#
SRCS=  alldptest.c dptest.c eedump.c eetest.c eestorenum.c eetosram.c\
fifotst.c fifotst1.c ldsrec.test.c led.c loopes.c loopse.c nvdump.c\
nvstorenum.c overnight.c pokevec.c showee.c sramtoee.c status.c\
stuffloop.c testalign.c
#    
HEADERS=
#
# Include files go here.
INC=
#
# Binaries go here.
BIN= /ELDORA/vx/hskp/bin

.KEEP_STATE:

compile: $(OBJS)
#	 install -m 774 $(OBJS) $(BIN)

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
	for i in $(SRCS); \
		do (echo "co $$i";co $$i); \
	done;

get_rcs_locked:
	for i in $(SRCS); \
		do (echo "co -l $$i";co -l $$i); \
	done;

put_rcs:
	for i in $(SRCS); \
		do (echo "ci $$i";ci $$i); \
	done;

# Modify this to include an access list if desired.
set_rcs:
	for i in $(SRCS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

