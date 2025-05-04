#include "tokens.h"
#include <stdio.h>

char* tTypeToStr(TokenType t) {
    switch (t) {
        // ========== PALABRAS CLAVE ==========
        case TOK_AUTO:       return "TOK_AUTO";
        case TOK_BREAK:      return "TOK_BREAK";
        case TOK_CASE:       return "TOK_CASE";
        case TOK_CHAR:       return "TOK_CHAR";
        case TOK_CONST:      return "TOK_CONST";
        case TOK_CONTINUE:   return "TOK_CONTINUE";
        case TOK_DEFAULT:    return "TOK_DEFAULT";
        case TOK_DO:         return "TOK_DO";
        case TOK_DOUBLE:     return "TOK_DOUBLE";
        case TOK_ELSE:       return "TOK_ELSE";
        case TOK_ENUM:       return "TOK_ENUM";
        case TOK_EXTERN:     return "TOK_EXTERN";
        case TOK_FLOAT:      return "TOK_FLOAT";
        case TOK_FOR:        return "TOK_FOR";
        case TOK_GOTO:       return "TOK_GOTO";
        case TOK_IF:         return "TOK_IF";
        case TOK_INT:        return "TOK_INT";
        case TOK_LONG:       return "TOK_LONG";
        case TOK_REGISTER:   return "TOK_REGISTER";
        case TOK_RETURN:     return "TOK_RETURN";
        case TOK_SHORT:      return "TOK_SHORT";
        case TOK_SIGNED:     return "TOK_SIGNED";
        case TOK_SIZEOF:     return "TOK_SIZEOF";
        case TOK_STATIC:     return "TOK_STATIC";
        case TOK_STRUCT:     return "TOK_STRUCT";
        case TOK_SWITCH:     return "TOK_SWITCH";
        case TOK_TYPEDEF:    return "TOK_TYPEDEF";
        case TOK_UNION:      return "TOK_UNION";
        case TOK_UNSIGNED:   return "TOK_UNSIGNED";
        case TOK_VOID:       return "TOK_VOID";
        case TOK_VOLATILE:   return "TOK_VOLATILE";
        case TOK_WHILE:      return "TOK_WHILE";

        // ========== OPERADORES Y SÍMBOLOS ==========
        case TOK_PLUS:       return "TOK_PLUS";
        case TOK_MINUS:      return "TOK_MINUS";
        case TOK_MULT:       return "TOK_MULT";
        case TOK_DIV:        return "TOK_DIV";
        case TOK_ASSIGN:     return "TOK_ASSIGN";
        case TOK_EQ:         return "TOK_EQ";
        case TOK_NEQ:        return "TOK_NEQ";
        case TOK_LT:         return "TOK_LT";
        case TOK_GT:         return "TOK_GT";
        case TOK_LEQ:        return "TOK_LEQ";
        case TOK_GEQ:        return "TOK_GEQ";
        case TOK_AND:        return "TOK_AND";
        case TOK_OR:         return "TOK_OR";
        case TOK_NOT:        return "TOK_NOT";
        case TOK_BITAND:     return "TOK_BITAND";
        case TOK_BITOR:      return "TOK_BITOR";
        case TOK_XOR:        return "TOK_XOR";
        case TOK_COMPLEMENT: return "TOK_COMPLEMENT";
        case TOK_LSHIFT:     return "TOK_LSHIFT";
        case TOK_RSHIFT:     return "TOK_RSHIFT";
        case TOK_INCREMENT:  return "TOK_INCREMENT";
        case TOK_DECREMENT:  return "TOK_DECREMENT";
        case TOK_DOT:        return "TOK_DOT";
        case TOK_ARROW:      return "TOK_ARROW";
        case TOK_LPAREN:     return "TOK_LPAREN";
        case TOK_RPAREN:     return "TOK_RPAREN";
        case TOK_LBRACE:     return "TOK_LBRACE";
        case TOK_RBRACE:     return "TOK_RBRACE";
        case TOK_LBRACKET:   return "TOK_LBRACKET";
        case TOK_RBRACKET:   return "TOK_RBRACKET";
        case TOK_SEMICOLON:  return "TOK_SEMICOLON";
        case TOK_COLON:      return "TOK_COLON";
        case TOK_COMMA:      return "TOK_COMMA";
        case TOK_ELLIPSIS:   return "TOK_ELLIPSIS";

        // ========== LITERALES E IDENTIFICADORES ==========
        case TOK_IDENTIFIER:     return "TOK_IDENTIFIER";
        case TOK_INT_LITERAL:    return "TOK_INT_LITERAL";
        case TOK_FLOAT_LITERAL:  return "TOK_FLOAT_LITERAL";
        case TOK_STRING_LITERAL: return "TOK_STRING_LITERAL";

        // ========== ERRORES Y FIN DE ARCHIVO ==========
        case TOK_ERROR:          return "TOK_ERROR";
        case TOK_EOF:            return "TOK_EOF";

        default:
            return "UNKNOWN_TOKEN";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <archivo_preprocesado>\n", argv[0]);
        return 1;
    }

    init_scanner(argv[1]); // Inicializa el scanner con el archivo preprocesado

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
            printf(" - Valor: %f", token.value.float_val);
        }
        printf("\n");
    } while (token.type != TOK_EOF);

    return 0;
}