#ifndef TOKENS_H
#define TOKENS_H

// Definir YYSTYPE para Flex
typedef union {
    char* lexeme;
    long long int_val;
    long double float_val;
    char* suffix;           // Sufijos como "ULL"
    _Bool bool_val;         // Literales booleanos
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
    // Palabras clave añadidas
    TOK_LONG_LONG,  TOK_LONG_DOUBLE, TOK_BOOL,      TOK_TRUE,
    TOK_FALSE,      TOK_RESTRICT,    TOK_INLINE,    TOK_COMPLEX,
    TOK_IMAGINARY,  TOK_THREAD_LOCAL, TOK_ATOMIC,   TOK_NORETURN,

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
    // Literales añadidos
    TOK_CHAR_LITERAL,   TOK_BOOL_LITERAL,

    // ========== ERRORES Y FIN DE ARCHIVO ==========
    TOK_ERROR,          TOK_EOF,

    TOK_COUNT // Añadir al final para obtener el total

} TokenType;

typedef struct {
    TokenType type;      // Tipo del token
    char* lexeme;        // Lexema (cadena del token)
    int line;            // Número de línea
    union {
        long long int_val;     // Valor numérico entero
        long double float_val; // Valor numérico flotante
    } value;
} Token;

// Prototipos de funciones
Token Get_Token(void);
void init_scanner(const char* filename);
void print_token_counts(void);
char* tTypeToStr(TokenType t); 


#endif
