#include "tokens.h"
#include "generadorLaTeX.h"
#include "preprocesador.h"
#include <stdio.h>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_por_preprocesar>\n", argv[0]);
        return 1;
    }

    make_preprocess(argv[1], "preprocesado.c");


    init_scanner("preprocesado.c"); // Inicializa el scanner con el archivo preprocesado
    unsigned long long base = 1;
    unsigned long long errores = 0;
    int cantErrores = 0;
    Token token;
    do {
        token = Get_Token();
        printf("Línea %d: Token %s", token.line, tTypeToStr(token.type));
        if (token.lexeme != NULL) {
            printf(" - Lexema: %s", token.lexeme);
        }
        if (token.type == TOK_INT_LITERAL) {
            printf(" - Valor: %d", token.value.int_val);
        } else if (token.type == TOK_FLOAT_LITERAL) {
            printf(" - Valor: %Lf", token.value.float_val);
        }
        else if(token.type == TOK_CHAR_LITERAL){
            printf(" - Valor: %d", token.value.int_val);
        }
        else if(token.type == TOK_ERROR) {
            errores += base*token.line;
            cantErrores++;
            base *= 100000;
        }
        printf("\n");
    } while (token.type != TOK_EOF);
    // print_token_counts();
    TokenCount cantidades = getTokenCount();
    for (int i = 0; i < TOK_COUNT; i++) {
        if (cantidades.token_count[i] >= 0) {
            printf("%-20samount: %d  / index: %d\n", tTypeToStr(i), cantidades.token_count[i], i);
        }
    }

    // Convertir TokenCount a un arreglo de enteros (como en generadorLaTeX.c)
    int arreglo_cantidades[86]; // Ajusta el tamaño según TOK_COUNT
    for (int i = 0; i < TOK_COUNT; i++) {
        arreglo_cantidades[i] = cantidades.token_count[i];
    }
    int arreglo_errores[cantErrores];
    for (int i = 0; i < cantErrores; i++) {
        arreglo_errores[i] = errores%100000;
        errores /= 100000;
    }
    // Llamar a la función para generar el PDF
    generar_beamer(arreglo_cantidades, arreglo_errores);

    return 0;
}