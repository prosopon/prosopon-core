SHELL=/bin/bash

#
#	define version of c compiler, linker, and lex
#
CC = gcc
LINK = gcc
LEX = flex
BISON = bison

#
#	define yacc lex and compiler flags
#
BISON_FLAGS = --debug -d
LEX_FLAGS = -dv
CFLAGS = -g

SRC = lex.yy.c gram.tab.c

all : $(SRC)
	g++ $(CFLAGS) $(SRC) -o talaria

lex.yy.c : scan.l
	$(LEX) $(LEX_FLAGS) scan.l

gram.tab.c : gram.y
	$(BISON) $(BISON_FLAGS) gram.y

clean :
	rm -f gram.tab.* lex.yy.c *.o talaria
	rm -rf talaria.dSYM



