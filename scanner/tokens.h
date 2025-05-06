#ifndef TOKENS_H
#define TOKENS_H

// Definir YYSTYPE para Flex
typedef union {
    char* lexeme;
    int int_val;
    float float_val;
} YYSTYPE;

extern YYSTYPE yylval;  // Declaración externa

extern int current_line;

typedef enum {
    // ========== PALABRAS CLAVE ==========
    TOK_AUTO,       TOK_BREAK,      TOK_CASE,       TOK_CHAR,
    TOK_CONST,      TOK_CONTINUE,   TOK_DEFAULT,    TOK_DO,
    TOK_DOUBLE,     TOK_ELSE,       TOK_ENUM,       TOK_EXTERN,
    TOK_FLOAT,      TOK_FOR,        TOK_GOTO,       TOK_IF,
    TOK_INT,        TOK_LONG,       TOK_REGISTER,   TOK_RETURN,
    TOK_SHORT,      TOK_SIGNED,     TOK_SIZEOF,     TOK_STATIC,
    TOK_STRUCT,     TOK_SWITCH,     TOK_TYPEDEF,    TOK_UNION,
    TOK_UNSIGNED,   TOK_VOID,       TOK_VOLATILE,   TOK_WHILE,

    // ========== OPERADORES Y SÍMBOLOS ==========
    TOK_PLUS,       TOK_MINUS,      TOK_MULT,       TOK_DIV,
    TOK_ASSIGN,     TOK_EQ,         TOK_NEQ,        TOK_LT,
    TOK_GT,         TOK_LEQ,        TOK_GEQ,        TOK_AND,
    TOK_OR,         TOK_NOT,        TOK_BITAND,     TOK_BITOR,
    TOK_XOR,        TOK_COMPLEMENT, TOK_LSHIFT,     TOK_RSHIFT,
    TOK_INCREMENT,  TOK_DECREMENT,  TOK_DOT,        TOK_ARROW,
    TOK_LPAREN,     TOK_RPAREN,     TOK_LBRACE,     TOK_RBRACE,
    TOK_LBRACKET,   TOK_RBRACKET,   TOK_SEMICOLON,  TOK_COLON,
    TOK_COMMA,      TOK_ELLIPSIS,

    // ========== LITERALES E IDENTIFICADORES ==========
    TOK_IDENTIFIER,     TOK_INT_LITERAL,
    TOK_FLOAT_LITERAL,  TOK_STRING_LITERAL,

    // ========== ERRORES Y FIN DE ARCHIVO ==========
    TOK_ERROR,          TOK_EOF

} TokenType;

typedef struct {
    TokenType type;      // Tipo del token
    char* lexeme;        // Lexema (cadena del token)
    int line;            // Número de línea
    union {
        int int_val;     // Valor numérico entero
        float float_val; // Valor numérico flotante
    } value;
} Token;

// Prototipos de funciones
Token Get_Token(void);
void init_scanner(const char* filename);

#endif