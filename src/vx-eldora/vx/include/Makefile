#
#	Makefile for vx/include
#	Author: R. Neitzel
#	Date: 2 Jan 1991
#
HEADERS= AngleConvert.hh

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

set_rcs:
	for i in $(HEADERS); \
		do (echo "rcs -c\" * \" -L $$i";rcs -c" * " -L $$i); \
	done;

clean:
	rm -f *~

real_clean: clean
	rm -f *.h






