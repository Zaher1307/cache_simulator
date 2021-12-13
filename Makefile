#
# Student makefile for Cache Lab
# Note: requires a 64-bit x86-64 system 
#
CC = gcc
CFLAGS = -g -Wall -Werror -std=c99 -m64 

all: csim.c cachelab.c cachelab.h simulate.c simulate.h init.c init.h
	$(CC) $(CFLAGS) -o csim csim.c cachelab.c simulate.c init.c -lm 

#
# Clean the src dirctory
#
clean:
	rm -rf *.o
