%{
#include <stdio.h>
#include <ctype.h>
#include "parser.tab.h"
#include "compiler.h"

void yyerror(const char *s);
extern int yychar;

%}

%option noyywrap
%option yylineno

DIGIT    [0-9]
INTEGER  [-+]?{DIGIT}+
FLOATNUM    [-+]?([0-9]*\.[0-9]+|[0-9]+\.)([eE][-+]?[0-9]+)?  
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
STRING   \"([^"\\]|\\.)*\"
WS       [ \t\n]

%%

"break"       { return BREAK; }
"continue"    { return CONTINUE; }
"if"          { return IF; }
"else"        { return ELSE; }
"while"       { return WHILE; }
"for"         { return FOR; }
"return"      { return RETURN; }
"const"       { return CONST; }
"unsigned"       { return UNSIGNED; }
"int"{WS}+"main" { 
                yylval.string = strdup("int main");
                return MAINFUN;}
'(\\.|[^'\\])'    {  // chars
                    yylval.char_val = process_char(yytext); 
                    return CHAR_LITERAL; 
                  }
{STRING}      { yylval.string = strdup(yytext);
                return STRING_LITERAL;
              }
"char"{WS}*"*" { 
                yylval.string = strdup("char*");
                return STR;}
"char"      { return CHAR; }
"float"      { return FLOAT; }
"long"      { return LONG; }
"long"{WS}+"long"      { return LONGLONG; }
"double"      { return DOUBLE; }
"long"{WS}+"double"    { return LONGDOUBLE; }
"int"         { return INT; }
"main"        { return MAIN; }

"++"          { return INC; }
"--"          { return DEC; }
"+="          { return PE; }
"-="          { return ME; }
"*="          { return MULE; }
"%="          { return MODE; }
"/="          { return DIVE; }
"+"           { return ADD; }
"-"           { return SUB; }
"*"           { return MUL; }
"/"           { return DIV; }
"%"           { return MOD; }
"="           { return ASSIGN; }
";"           { return SEMICOLON; }
","           { return COMMA; }
"("           { return LPAREN; }
")"           { return RPAREN; }
"{"           { return LBRACE; }
"}"           { return RBRACE; }

"=="          { return EQ; }
"!="          { return NE; }
"<"           { return LT; }
"<="          { return LE; }
">"           { return GT; }
">="          { return GE; }
"!"           { return NOT; }
"&&"          { return AND; }
"||"          { return OR; }

{ID}          { yylval.string = strdup(yytext); return IDENTIFIER; }
{INTEGER}     { yylval.number = atoll(yytext); return INTEGER; }
{FLOATNUM}       { yylval.float_val = atof(yytext); return FLOAT_LITERAL; }
{WS}       ;
.       {
    char error_msg[50];
    if (isprint(*yytext)) {
        snprintf(error_msg, sizeof(error_msg), "Invalid character '%s'", yytext);
    } else {
        snprintf(error_msg, sizeof(error_msg), "Invalid character (ASCII %d)", *yytext);
    }
    compiler_error(error_msg);
} 

%%