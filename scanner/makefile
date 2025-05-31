CC = gcc
CFLAGS = -Wall -Wextra -Wno-psabi -g
LDFLAGS = -lfl
TARGET = compilador
SOURCES = main.c scanner.c preprocesador.c parser.c lex.yy.c
HEADERS = tokens.h preprocesador.h parser.h

.PHONY: all clean

all: $(TARGET)

# Explicit rule to prevent scanner.c from being regenerated
scanner.c:
	@echo "Using existing scanner.c"

lex.yy.c: scanner.l
	flex $<

$(TARGET): $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o $@ $(SOURCES) $(LDFLAGS)

clean:
	rm -f $(TARGET) preprocesado.c

byhand:
	flex scanner.l
	gcc -o compilador lex.yy.c main.c scanner.c preprocesador.c parser.c -lfl



