#
#	Makefile for the housekeeping processor
#	Author: C Walther
#	Date:August 1992
#
DIRS= arinc429 clk gps motor vmevme power main ecb tp41 minirims

.MAIN: obj

obj: 
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE)); \
	done;

test: obj
	(cd test; make)

depend:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) depend); \
	done;

asm:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) asm); \
	done;

print:
	for i in $(DIRS); \
		do (echo "Printing in $$i"; cd $$i; enscript *.c); \
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

set_rcs:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) set_rcs); \
	done;

clean:
	\rm /ELDORA/vx/hskp/bin/*.o

	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) clean); \
	done;

copyem:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) copyem); \
	done;

real_clean:
	for i in $(DIRS); \
		do (echo "Making in $$i"; cd $$i; $(MAKE) real_clean); \
	done;
