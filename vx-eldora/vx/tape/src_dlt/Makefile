#
#	Makefile for DLT tape handling code 
#	Author: Craig Walther
#	Date: 06/18/96
#
# GNU C 
CC = gcc68k
CCC = $(CC)
LD=ld68k
#
# Totally agressive optimization.
OPT= -O -fvolatile -fno-builtin -nostdinc -m68020
#
DEFINES= 
#
# Drag in includes from here.
INCLUDES = -I/vx/h -I. -I/ELDORA/rpc/include -I/ELDORA/rpc/header\
	 -I/ELDORA/vx/c++/include -I/ELDORA/vx/tape/inc_dlt\
	  -I/ELDORA/vx/tape/inc_rec -I/local/include/vx++
#
CFLAGS=	$(DEFINES) $(INCLUDES) $(OPT)
#
LIBS= /ELDORA/rpc/lib/libVxHeader.a /ELDORA/vx/c++/bin/Task.o\
	/ELDORA/vx/c++/bin/Flags.o /ELDORA/rpc/lib/libVxTapeRpc.a\
	/ELDORA/rpc/lib/libVxLogger.a -L/local/lib/gcc-lib/68k/current\
	-lgcc
#
OBJS= cip_cmds.o dlt_cmds.o chan_attn.o tst_unt_rdy.o cip_init.o\
md_sel_init.o md_sns_init.o parm_blk_init.o print_md_sns.o print_rqst_sns.o\
print_stat.o sg_init.o drive_init.o initialize_tape.o tape_remain.o\
write_tape.o fill_sg.o print_results.o read_tape.o tape_cntrl_len.o\
space_tape.o tape_menu.o print_log_sns.o\
rqst_sns_init.o
#

SRCS= cip_cmds.c dlt_cmds.c chan_attn.c tst_unt_rdy.c cip_init.c\
md_sel_init.c md_sns_init.c parm_blk_init.c print_md_sns.c print_rqst_sns.c\
print_stat.c sg_init.c drive_init.c initialize_tape.c tape_remain.c\
write_tape.c fill_sg.c print_results.c read_tape.c tape_cntrl_len.c\
space_tape.c tape_menu.c print_log_sns.c\
rqst_sns_init.c
#
HEADERS=
#
# Include files go here.
CXX = ${CCC}
CXXFLAGS = ${CFLAGS}
#
# Binaries go here.
BIN=

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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,rich,eric -c" * " -L $$i); \
	done;

copyem:
	cp *.o /ELDORA/vx/tape/bin/.

clean:
	rm -f *.o *~ core

real_clean: clean
	rm -f *.c *.h





