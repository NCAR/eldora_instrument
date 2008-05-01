   

   EXEC = header
   CC = gcc 
   #CFLAGS  =
   CFLAGS  =  -g 
   LIB = -lm
   OBJS = gen_field.o read_header.o forefieldradar.o  aftfieldradar.o\
	  forecellspacing.o aftcellspacing.o\
          waveform.o volume.o foreradar.o aftradar.o\
	  foreparameter.o aftparameter.o\
	  calculate_parameters.o navagation.o insitudata.o
   SRCS = gen_field.c read_header.c forefieldradar.c  aftfieldradar.c\
	  forecellspacing.c aftcellspacing.c\
          waveform.c volume.c foreradar.c aftradar.c\
	  foreparameter.c aftparameter.c\
	  calculate_parameters.c navagation.c insitudata.c\
	  Comment.h Correction.h Sweep.h field.h
	
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


