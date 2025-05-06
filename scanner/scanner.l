%{
#include "tokens.h"
#include <stdlib.h>
#include <string.h>

int current_line = 1;
char* string_buf;        // Buffer para cadenas
unsigned int string_buf_size;

// Prototipos
void lexical_error(const char* msg);
void handle_comment(void);
%}

%option noyywrap nounput noinput
%x COMMENT STRING

DIGIT    [0-9]
HEX      [0-9a-fA-F]
OCTAL    [0-7]
ID       [a-zA-Z_][a-zA-Z0-9_]*
EXP      [Ee][+-]?{DIGIT}+
SUFFIX     [uUlL]{1,3}  // Sufijos para enteros: U, L, UL, etc.

%%

 /* ========== PALABRAS CLAVE ========== */
"auto"      { return TOK_AUTO; }
"break"     { return TOK_BREAK; }
"case"      { return TOK_CASE; }
"char"      { return TOK_CHAR; }
"const"     { return TOK_CONST; }
"continue"  { return TOK_CONTINUE; }
"default"   { return TOK_DEFAULT; }
"do"        { return TOK_DO; }
"double"    { return TOK_DOUBLE; }
"else"      { return TOK_ELSE; }
"enum"      { return TOK_ENUM; }
"extern"    { return TOK_EXTERN; }
"float"     { return TOK_FLOAT; }
"for"       { return TOK_FOR; }
"goto"      { return TOK_GOTO; }
"if"        { return TOK_IF; }
"int"       { return TOK_INT; }
"long"      { return TOK_LONG; }
"register"  { return TOK_REGISTER; }
"return"    { return TOK_RETURN; }
"short"     { return TOK_SHORT; }
"signed"    { return TOK_SIGNED; }
"sizeof"    { return TOK_SIZEOF; }
"static"    { return TOK_STATIC; }
"struct"    { return TOK_STRUCT; }
"switch"    { return TOK_SWITCH; }
"typedef"   { return TOK_TYPEDEF; }
"union"     { return TOK_UNION; }
"unsigned"  { return TOK_UNSIGNED; }
"void"      { return TOK_VOID; }
"volatile"  { return TOK_VOLATILE; }
"while"     { return TOK_WHILE; }
/* Palabras clave añadidas */
"bool"          { return TOK_BOOL; }
"true"          { yylval.bool_val = true; return TOK_BOOL_LITERAL; }
"false"         { yylval.bool_val = false; return TOK_BOOL_LITERAL; }
"restrict"      { return TOK_RESTRICT; }
"inline"        { return TOK_INLINE; }
"_Complex"      { return TOK_COMPLEX; }
"_Imaginary"    { return TOK_IMAGINARY; }
"_Thread_local" { return TOK_THREAD_LOCAL; }
"_Atomic"       { return TOK_ATOMIC; }
"_Noreturn"     { return TOK_NORETURN; }

 /* ========== OPERADORES Y SÍMBOLOS ========== */
"+"         { return TOK_PLUS; }
"-"         { return TOK_MINUS; }
"*"         { return TOK_MULT; }
"/"         { return TOK_DIV; }
"="         { return TOK_ASSIGN; }
"=="        { return TOK_EQ; }
"!="        { return TOK_NEQ; }
"<"         { return TOK_LT; }
">"         { return TOK_GT; }
"<="        { return TOK_LEQ; }
">="        { return TOK_GEQ; }
"&&"        { return TOK_AND; }
"||"        { return TOK_OR; }
"!"         { return TOK_NOT; }
"&"         { return TOK_BITAND; }
"|"         { return TOK_BITOR; }
"^"         { return TOK_XOR; }
"~"         { return TOK_COMPLEMENT; }
"<<"        { return TOK_LSHIFT; }
">>"        { return TOK_RSHIFT; }
"++"        { return TOK_INCREMENT; }
"--"        { return TOK_DECREMENT; }
"."         { return TOK_DOT; }
"->"        { return TOK_ARROW; }
"("         { return TOK_LPAREN; }
")"         { return TOK_RPAREN; }
"{"         { return TOK_LBRACE; }
"}"         { return TOK_RBRACE; }
"["         { return TOK_LBRACKET; }
"]"         { return TOK_RBRACKET; }
";"         { return TOK_SEMICOLON; }
":"         { return TOK_COLON; }
","         { return TOK_COMMA; }
"..."       { return TOK_ELLIPSIS; }

 /* ========== LITERALES ENTEROS CON SUFIJOS ========== */
0[xX]{HEX}+{SUFFIX}? {
    char *end;
    yylval.ll_val = strtoll(yytext, &end, 16);
    yylval.suffix = strdup(end);  // Almacena el sufijo (ej. "ULL")
    return TOK_INT_LITERAL;
}

0{OCTAL}+{SUFFIX}? {
    char *end;
    yylval.ll_val = strtoll(yytext, &end, 8);
    yylval.suffix = strdup(end);
    return TOK_INT_LITERAL;
}

{DIGIT}+{SUFFIX}? {
    char *end;
    yylval.ll_val = strtoll(yytext, &end, 10);
    yylval.suffix = strdup(end);
    return TOK_INT_LITERAL;
}

 /* ========== LITERALES FLOTANTES (incluye long double) ========== */
{DIGIT}+"."{DIGIT}*{EXP}?[fFlL]? {
    if (strchr(yytext, 'L') || strchr(yytext, 'l')) {
        yylval.long_double_val = strtold(yytext, NULL); // Para long double
    } else {
        yylval.double_val = strtod(yytext, NULL);       // Para double
    }
    return TOK_FLOAT_LITERAL;
}

 /* ========== LITERALES Y IDENTIFICADORES ========== */
{ID}        {
    yylval.lexeme = strdup(yytext);
    return TOK_IDENTIFIER;
}

 /* Enteros (decimal, octal, hexadecimal) */
0[xX]{HEX}+ {
    yylval.int_val = strtol(yytext, NULL, 16);
    return TOK_INT_LITERAL;
}

0{OCTAL}+   {
    yylval.int_val = strtol(yytext, NULL, 8);
    return TOK_INT_LITERAL;
}

 /* Literales enteros */
{DIGIT}+ {
    yylval.int_val = atoi(yytext); // Acceso correcto
    return TOK_INT_LITERAL;
}

 /* Flotantes */
 /* Literales flotantes */
{DIGIT}+"."{DIGIT}*{EXP}? {
    yylval.float_val = atof(yytext); // Acceso correcto
    return TOK_FLOAT_LITERAL;
}

 /* Cadenas con caracteres escapados */
\" {
    string_buf = (char*)malloc(256);
    string_buf_size = 0;
    BEGIN(STRING);
}

<STRING>{
    \" {
        string_buf[string_buf_size] = '\0';
        yylval.lexeme = string_buf;
        BEGIN(INITIAL);
        return TOK_STRING_LITERAL;
    }
    \\n     { strcat(string_buf, "\n"); string_buf_size++; }
    \\t     { strcat(string_buf, "\t"); string_buf_size++; }
    \\r     { strcat(string_buf, "\r"); string_buf_size++; }  // Nuevas secuencias
    \\b     { strcat(string_buf, "\b"); string_buf_size++; }
    \\f     { strcat(string_buf, "\f"); string_buf_size++; }
    \\\"    { strcat(string_buf, "\""); string_buf_size++; }
    \\\\    { strcat(string_buf, "\\"); string_buf_size++; }
    \\.     { lexical_error("Secuencia de escape inválida"); }
    .       {
        if (string_buf_size >= 255) {
            lexical_error("Cadena demasiado larga");
        }
        string_buf[string_buf_size++] = *yytext;
    }
    <<EOF>> {
        lexical_error("Cadena no cerrada");
        BEGIN(INITIAL);
        return TOK_ERROR;
    }
}

 /* ========== COMENTARIOS Y ESPACIOS ========== */
"//".*      { /* Ignorar comentarios de línea */ }
"/*"        { BEGIN(COMMENT); }

<COMMENT>{
    "*/"    { BEGIN(INITIAL); }
    <<EOF>> {
        lexical_error("Comentario no cerrado");
        BEGIN(INITIAL);
        return TOK_ERROR;
    }
    .|\n    { /* Ignorar contenido */ }
}

[ \t\r]+    { /* Ignorar espacios y tabs */ }
\n          { current_line++; }

 /* ========== MANEJO DE ERRORES ========== */
.           {
    lexical_error("Carácter no reconocido");
    return TOK_ERROR;
}

<<EOF>> {return TOK_EOF;}

%%

void lexical_error(const char* msg) {
    fprintf(stderr, "Error léxico (línea %d): %s: '%s'\n", current_line, msg, yytext);
}
