#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
#include "preprocesador.h"
#include "ast.h"

extern FILE *yyin;
extern int yyparse();
extern int yylineno;
extern ASTNode *program_root;
extern ScopeStack *symbol_table;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <input_file_to_preprocess>\n", argv[0]);
        return 1;
    }

    make_preprocess(argv[1], "preprocessed.c");
    FILE* input = fopen("preprocessed.c", "r");
    
    // if (argc != 2) {
    //     fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
    //     return 1;
    // }

    // FILE* input = fopen(argv[1], "r");

    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    yyin = input;

    // Parse the input file
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed at line %d\n", yylineno);
        fclose(yyin);
        return 1;
    }

    printf("Parsing completed successfully!\n\n");

    // AST
    // printf("AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST\n");
    // ast_print(program_root, 0);
    // printf("AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST ~ AST\n\n");


    // Semantic analysis
    printf("Semantic Analysis\n");
    semantic_analyze(program_root, symbol_table);
    printf("End semantic Analysis\n");

    // Assembly gen
    FILE* output = fopen("output.asm", "w");
    generate_assembly(program_root, output);
    
    
    // Cleanup
    fclose(yyin);
    ast_free(program_root);
    free_scope_stack(&symbol_table);
    
    return 0;
}