#
#	Makefile for displays
#	Author: R. E. K. Neitzel 
#	Date:1 March 1991
#
DIRS= wdw converter Display Dual Horiz Radial Vert clock ddp ge
VERSION= 2.0

obj:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE)); \
	done;

update:
	for i in ${DIRS}; \
		do (echo "making in $$i"; cd $$i; $(MAKE) update); \
	done;

depend:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) depend); \
	done;

asm:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) asm); \
	done;

get_rcs:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) get_rcs); \
	done;

get_rcs_locked:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) get_rcs_locked); \
	done;

put_rcs:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) put_rcs); \
	done;

put_rcs_new:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) VERSION=$(VERSION) put_rcs_new); \
	done;

set_rcs:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) set_rcs); \
	done;

clean:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) clean); \
	done;

real_clean:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) real_clean); \
	done;
