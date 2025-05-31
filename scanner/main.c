#include "tokens.h"
#include "preprocesador.h"
#include "parser.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_por_preprocesar>\n", argv[0]);
        return 1;
    }

    make_preprocess(argv[1], "preprocesado.c");
    init_scanner("preprocesado.c"); // Inicializa el scanner con el archivo preprocesado

    ASTNode *ast = parse_program();

    if(ast != NULL){
        printf("\nAbstract Syntax Tree:\n");
        print_ast(ast, 0);
        printf("\nAST pointers before free_ast:\n");
        print_ast_pointers(ast, 0);
        free_ast(ast);
        ast = NULL;
        printf("\nAST pointers after free_ast:\n");
        print_ast_pointers(ast, 0);
    }


    return parser_had_error() ? 1 : 0;
}