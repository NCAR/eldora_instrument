CC = gcc68k

OBJS= getInt.o sendInt.o 

CFLAGS = -O2 -I/vx/h -I/net/eldora/eldora/vx/mhen/include -I/net/eldora/eldora/vx/hskp/include -I/net/eldora/eldora/rpc/include -I.



all: ${OBJS}

test: ${OBJS} ctest.o stest.o
	ld68k -Ur -X -o mhen.o getInt.o ctest.o
	ld68k -Ur -X -o hskp.o sendInt.o stest.o
