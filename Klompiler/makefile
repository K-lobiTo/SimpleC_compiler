CC = gcc
CFLAGS = -g -Wall

all: compiler

compiler: lexer.yy.c parser.tab.c compiler.o main.o trie.o stack_of_tries.o ast.o preprocesador.o
	$(CC) $(CFLAGS) -o $@ $^

lexer.yy.c: lexer.l parser.tab.h
	flex -o $@ $<

parser.tab.c parser.tab.h: parser.y
	bison -d $<

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o compiler lexer.yy.c lex.yy.c parser.tab.c parser.tab.h output.asm preprocessed.c

.PHONY: all clean