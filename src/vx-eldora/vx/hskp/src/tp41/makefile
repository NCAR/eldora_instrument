#
#	Makefile for Directory ELDORA/vx/hskp/src/tp41 
#	Author: Craig Walther
#	Date: 8/19/92
#
# GNU C 
CC= gcc68k
CCC = $(CC)
#
# Totally agressive optimization.
#OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
#	-fomit-frame-pointer
OPT= -O -fvolatile -fno-builtin -nostdinc -m68040
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. -I/ELDORA/rpc/header2 -I/ELDORA/vx/hskp/include\
		-I/ELDORA/vx/c++/include
#
CFLAGS= $(OPT) $(DEFINES) $(INCLUDES)
CCFLAGS= $(CFLAGS)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/builtin++.o\
	/ELDORA/vx/c++/ctor_dummy.o
#
OBJS= hskpIntInit.o
#
SRCS= hskpIntInit.c
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

#	 bsdinst -m 774 $(OBJS) $(BIN)

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
		do (echo "rcs -c\" * \" -L $$i";rcs -aericloew,granger,martinc -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/hskp/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h


