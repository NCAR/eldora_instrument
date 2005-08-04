#**********************************************************************
# 			     MCPL Library Makefile
#                         A portion of the MCPL library
#                  Copyright 1989 Kevin J. Lynch, Ironics Inc.
#*********************************************************************/
# 2.0 Production Release
#
#
CC=gcc
#ANSI=-UANSI
ANSI= -DANSI
CFLAGS= $(ANSI) -O -DDEBUG -fforce-mem -fstrength-reduce -fomit-frame-pointer \
	-I../include
MCPLOBJS=mcplglobal.o mcpltpb.o mcplexe.o mcplreg.o mcpldump.o

.KEEP_STATE:

mcpllib.o: $(MCPLOBJS)
	ld -r -X -o $@ $(MCPLOBJS)
