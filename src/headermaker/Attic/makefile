   

   EXEC = header
   CC = gcc
   #CFLAGS  =
   CFLAGS  =  $(DEFINES) -g -I../include
#   DEFINES = -DNEW -DLO_RT_SDLB -DNU_RX
   DEFINES = -DNEW -DNU_RX
   LIB = -lm
   OBJS = set_physical.o make_new.o waveform.o volume.o fieldradar.o\
	  radar.o cellspacing.o navigation.o insitudata.o parameter.o\
	  gen_field.o read_header.o verify.o engin.o big_little.o \
	  unpack_hd.o re_cal.o
   SRCS = gen_field.c read_header.c fieldradar.c cellspacing.c\
          waveform.c volume.c radar.c parameter.c engin.c\
	  navigation.c insitudata.c set_physical.c big_little.c\
	  mkrDef.h mkrGbl.h mkrFunc.h make_new.c verify.c unpack_hd.c\
	  re_cal.c
	
   $(EXEC): $(OBJS)
	gcc -v  ${CFLAGS} $(OBJS) $(LIB) -o $@
#
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
		do (echo "rcs -c\" * \" -L $$i";rcs -acraig,shawn,rich,eric,reif,sherrie -c" * " -L $$i); \
	done;








