final:
	flex scanner.l
	gcc -o compilador generadorLaTeX.c lex.yy.c main.c scanner.c preprocesador.c -lfl
all:

	flex scanner.l
	gcc lex.yy.c main.c scanner.c -o lexer -lfl



