#
#	Makefile for Directory include
#	Author: Eric Loew
#	Date: 12/2/91
#
# GNU C 
CC= gcc68
#
# Totally agressive optimization.
OPT= -O -fstrength-reduce -fforce-mem -finline-functions -fcombine-regs \
	-fomit-frame-pointer
#
DEFINES=
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I/ELDORA/vx/mhen/include -I/ELDORA/dsp/include
#
CFLAGS= $(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS=
#
SRCS=
#
HEADERS= rp7.h dspdefs.h dspaddr.h colquadsubs.h cband.col_dpramdefs.h \
         coll_dpramdefs.h dp_ramdefs.h
#
# Include files go here.
INC=
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.KEEP_STATE:

compile: $(OBJS)
	install -m 774 $(OBJS) $(BIN)

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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif -c" * " -L $$i); \
	done;

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h






