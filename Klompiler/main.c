#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

extern FILE *yyin;
extern int yyparse();
extern int yylineno;
extern ASTNode *program_root;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    yyin = input;
    // Parse the input file
    // int error_line = 0;
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed at line %d\n", yylineno);
        fclose(yyin);
        return 1;
    }

    printf("Parsing completed successfully!\n");
    
    // Future AST processing functions here
    
    // Cleanup
    fclose(yyin);
    free_ast(program_root);
    
    return 0;
}