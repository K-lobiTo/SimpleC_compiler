CC = gcc
CFLAGS = -g -Wall

all: compiler

compiler: lexer.yy.c parser.tab.c compiler.c main.c trie.c
	$(CC) $(CFLAGS) -o $@ $^

lexer.yy.c: lexer.l parser.tab.h
	flex -o $@ $<

parser.tab.c parser.tab.h: parser.y
	bison -d $<

clean:
	rm -f compiler lexer.yy.c parser.tab.c parser.tab.h

.PHONY: all clean