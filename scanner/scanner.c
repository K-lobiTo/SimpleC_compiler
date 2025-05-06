#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"
//#include "scanner.h"  // Si es necesario para funciones auxiliares

YYSTYPE yylval;

extern int yylex(void);
extern FILE* yyin;

Token Get_Token(void) {
    Token token;
    int token_type = yylex();
    
    token.type = token_type;
    token.line = current_line;

    switch (token_type) {
        case TOK_IDENTIFIER:
        case TOK_STRING_LITERAL:
            token.lexeme = strdup(yylval.lexeme);
            break;
        case TOK_FLOAT_LITERAL:
            token.lexeme = NULL;
            token.value.float_val = yylval.float_val;
            break;
        case TOK_INT_LITERAL:
            token.lexeme = NULL;
            token.value.int_val = yylval.int_val;
            break;
        case TOK_EOF:
            fclose(yyin); // Cierra el archivo
            token.lexeme = NULL;
            break;
        default:
            token.lexeme = NULL;
    }
    
    return token;
}

// Función para inicializar el scanner con el archivo preprocesado
void init_scanner(const char* filename) {
    yyin = fopen(filename, "r");
    if (!yyin) {
        perror("Error al abrir el archivo preprocesado");
        exit(EXIT_FAILURE);
    }
}