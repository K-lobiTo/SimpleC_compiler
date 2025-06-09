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
LETTER   [a-zA-Z_]
ID       {LETTER}({LETTER}|{DIGIT})*
WS       [ \t\n]

%%

"break"       { return BREAK; }
"if"          { return IF; }
"else"        { return ELSE; }
"while"       { return WHILE; }
"for"         { return FOR; }
"return"      { return RETURN; }
"int"{WS}+"main" { 
                yylval.string = strdup("int main");
                return MAINFUN;}

"int"         { return INT; }
"main"        { return MAIN; }

"++"          { return INC; }
"--"          { return DEC; }
"+="          { return PE; }
"-="          { return ME; }
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
{DIGIT}+      { yylval.number = atoi(yytext); return INTEGER; }
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