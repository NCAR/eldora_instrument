#
#	Makefile for 
#	Author:
#	Date:
#
# GNU C 
CC= gcc68k
CCC= $(CC)
LD=ld68k
#
# Totally agressive optimization.
OPT= -O -fvolatile -fno-builtin -nostdinc
#
DEFINES= -DUSE_ANSI
#
# Drag in includes from here.
INCLUDES= -I/vx/h -I. 
#
CFLAGS=	$(DEFINES) $(INCLUDES)
#
LIBS=
#
OBJS= cpcmds.o exbcmds.o chnattn.o tstrdy.o write_tape.o read_tape.o cipinit.o mdselinit.o mdsnsinit.o pmblkinit.o prtmdsns.o prtrqst.o prtstat.o rqstsns.o sginit.o tape_menu.o sctr_gthr.o tp_cpy.o fill_sg.o space_tape.o send_diags.o print_results.o
#
SRCS= cpcmds.c exbcmds.c chnattn.c tstrdy.c write_tape.c read_tape.c cipinit.c mdselinit.c mdsnsinit.c pmblkinit.c prtmdsns.c prtrqst.c prtstat.c rqstsns.c sginit.c tape_menu.c sctr_gthr.c tp_cpy.c fill_sg.c space_tape.c send_diags.c print_results.c
#
HEADERS= cipincl.h
#
# Include files go here.
INC=
#
# Binaries go here.
BIN=

.KEEP_STATE:
tape_diag.o:	$(OBJS)
	$(LD) -r -X -o $@ $(OBJS) $(LIBS)
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

# Modify this to include an access list if desired.
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
startRPC.o:
	$(CCC) $(CFLAGS) -c startRPC.cc
ctrlHandler.o:
	$(CCC) $(CFLAGS) -c ctrlHandler.cc
headerHandler.o:
	$(CCC) $(CFLAGS) -c headerHandler.cc






