#
#	Makefile for Directory dspq
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
INIT_OBJS= ppp_init.o dp_init.o
#
PPP_OBJS= ppp_init.o ppp_main.o
#
DP_OBJS= dp_init.o dp_main.o
#
INIT_SRCS= ppp_init.s dp_init.s
#
PPP_SRCS= ppp_init.s ppp_main.s
#
DP_SRCS= dp_init.s dp_main.s
#
SRCS= ppp_init.s ppp_main.s dp_init.s dp_main.s
#
# Binaries go here.
BIN=/ELDORA/vx/mhen/bin

.s.o:
	$(AS) $(ASFLAGS) $<

.KEEP_STATE:

compile: $(INIT_OBJS) 

fore_ppp:
	$(AS) -DFORE_INTERRUPT $(ASFLAGS) ppp_main.s
	d3ld -m -o ppp.o ppp_ifile $(PPP_OBJS)
	rm -f ppp_fore.dmp
	d3dump -sp ppp.o > ppp_fore.dmp
	install -m 774 ppp_fore.dmp $(BIN)
aft_ppp:
	$(AS) -DAFT_INTERRUPT $(ASFLAGS) ppp_main.s
	d3ld -m -o ppp.o ppp_ifile $(PPP_OBJS)
	rm -f ppp_aft.dmp
	d3dump -sp ppp.o > ppp_aft.dmp
	install -m 774 ppp_aft.dmp $(BIN)
master_dp:
	$(AS) -DMASTER $(ASFLAGS) dp_main.s
	d3ld -m -o dp.o dp_ifile $(DP_OBJS)
	rm -f dp_master.dmp
	d3dump -sp dp.o > dp_master.dmp
	install -m 774 dp_master.dmp $(BIN)
slave_dp:
	$(AS) -DSLAVE $(ASFLAGS) dp_main.s
	d3ld -m -o dp.o dp_ifile $(DP_OBJS)
	rm -f dp_slave.dmp
	d3dump -sp dp.o > dp_slave.dmp
	install -m 774 dp_slave.dmp $(BIN)

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






