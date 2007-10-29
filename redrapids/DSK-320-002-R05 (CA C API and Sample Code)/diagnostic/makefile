#Red River ChannelAdapter makefile

TARGET = diagnostic

CC=gcc
USER_BITS=32
OD = unknown
CA_APIREV=R05

TARGET_ARCH=$(shell uname -m)

CFLAGS = -DLINUX

ifeq ("$(TARGET_ARCH)", "PPC")
	USER_BITS = 32
	OD = linux_ppc_gcc3
endif

ifeq ("$(TARGET_ARCH)", "x86_64")
	USER_BITS = 64
	OD = linux_x86-64_gcc3
endif

ifeq ("$(TARGET_ARCH)", "i686")
	USER_BITS = 32
	OD = linux_x86-32
endif

CFLAGS += -m$(USER_BITS)
LFLAGS += -m$(USER_BITS)

#Code has to be thread aware
IPATH = ../channeladapterlib/$(CA_APIREV)
LPATH = ../channeladapterlib/$(CA_APIREV)/$(OD)
LFLAGS = -lpthread -lchanneladapter
CFLAGS += -I $(IPATH)

SRCS = main.c

ARCH_OBJS = $(addsuffix .o, $(addprefix $(OD)/, $(basename $(notdir $(SRCS)))))
OBJS      = $(addsuffix .o, $(basename $(notdir $(SRCS))))

all: $(OD) $(OBJS) ca_functions.o ca_diag_functions.o winfuncs.o $(TARGET)

clean:
	rm -f $(OD)/*.o $(OD)/$(TARGET)

$(OD):
	-@ mkdir $@ &
	
$(OBJS): %.o : %.c
	$(CC) -c $(CFLAGS) -o $(OD)/$@ $<

ca_functions.o : $(IPATH)/ca_functions.c
	$(CC) -c $(CFLAGS) -o $(OD)/$@ $<
	
ca_diag_functions.o : $(IPATH)/ca_diag_functions.c
	$(CC) -c $(CFLAGS) -o $(OD)/$@ $<
	
winfuncs.o : $(IPATH)/winfuncs.c
	$(CC) -c $(CFLAGS) -o $(OD)/$@ $<
	
$(TARGET) : $(ARCH_OBJS)
	$(CC) $(CFLAGS) -o $(OD)/$(TARGET) $(OD)/ca_functions.o $(OD)/ca_diag_functions.o $(OD)/winfuncs.o $(ARCH_OBJS) -L$(LPATH) $(LFLAGS)
	cp $(OD)/$(TARGET) bin/
