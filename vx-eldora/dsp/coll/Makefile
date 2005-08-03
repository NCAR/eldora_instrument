#
#	Makefile for Directory coll
#	Author: Eric Loew
#	Date: 11/10/92
#
# DSP32C
AS= d3as
#
DEFINES= -I/ELDORA/dsp/include -I/ELDORA/vx/mhen/include
# Drag in includes from here.
ASFLAGS= $(DEFINES) -Q -l 
#
OBJS= coll_init.o coll_main.o coll_ts.o
#
SRCS= coll_init.s coll_main.s coll_ts.s
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.s.o:
	$(AS) $(ASFLAGS) $<

.KEEP_STATE:

compile: $(OBJS)

coll:
	d3ld -m -o coll.o coll_ifile $(OBJS)
	rm -f coll.dmp
	d3dump -sp coll.o > coll.dmp
	install -m 774 coll.dmp $(BIN)

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

real_clean: clean
	rm -f *.s *.h






