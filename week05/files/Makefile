# COMP2521 19T3 ... lab 5 code

CC	= gcc -Wall -Werror -std=c11 -g

.PHONY: all
all:	tlab

tlab:	tlab.o Tree.o
tlab.o:	tlab.c Tree.h
Tree.o:	Tree.c Tree.h

.PHONY: clean
clean:
	-rm -f tlab tlab.o Tree.o

