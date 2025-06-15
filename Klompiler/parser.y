%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"
#include "ast.h"


extern int yylex();

#include "ast.h"
extern int yylineno;
void yyerror(const char *msg);
const char *token_name(int token);

extern int yychar;
static const char *last_expected = NULL;

// Global AST root and symbol table
ASTNode *program_root = NULL;
ScopeStack *symbol_table = NULL;

ASTNode *current_compound = NULL;

%}

%define parse.error verbose
%define parse.lac full
%token-table

%union {
    long long number;
    char *string;
    char char_val;
    double float_val;
    struct ASTNode *node;
    int var_type;
}

%left UNARY_MINUS
%token <number> INTEGER
%token <string> IDENTIFIER STRING_LITERAL
%token <float_val> FLOAT_LITERAL
%token <char_val> CHAR_LITERAL
%token INT LONG LONGLONG FLOAT MAIN MAINFUN STR CONST UNSIGNED CHAR DOUBLE LONGDOUBLE
%token BREAK IF ELSE WHILE FOR RETURN CONTINUE
%token ADD SUB MUL DIV MOD
%token ASSIGN
%token SEMICOLON COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NE LT LE GT GE
%token NOT AND OR
%token INC DEC PE ME MULE MODE DIVE


%type <var_type> type_specifier
%type <node> program_start program global_declarations
%type <node> declaration expression statement declaration_prefix
%type <node> statement_list expression_statement
%type <node> selection_statement iteration_statement jump_statement for_init expression_opt 
%type <node> compound_statement 
%type <node> assignment_expression logical_or_expression logical_and_expression
%type <node> equality_expression relational_expression additive_expression
%type <node> multiplicative_expression postfix_expression unary_expression primary_expression

%%


program:
    program_start
    global_declarations MAINFUN LPAREN RPAREN compound_statement
    {
        ast_add_statement(program_root, $2);
        ast_add_statement(program_root, $6);
        $$ = program_root;
    }
    | program_start global_declarations 
    {
        ast_add_statement(program_root, $2);
        $$ = program_root;
    }
    ;

program_start:
    {   // Initialization of the symbol table here 
        symbol_table = create_scope_stack(10);
        program_root = ast_new_program(symbol_table);
        if (!program_root) {
            printf("Failed to create program node\n");
            YYABORT;
        }
        $$ = ast_new_compound_statement(yylineno);
    }

global_declarations:
    global_declarations declaration { $$ = ast_add_statement($1, $2); }
    | /* empty */ { $$ = ast_new_compound_statement(yylineno); }
    ;

declaration:
    declaration_prefix type_specifier IDENTIFIER SEMICOLON {
        $$ = ast_new_declaration($3, $2, NULL, yylineno);
        $$->is_constant = ($1 != NULL); 
        free($3);
    }
    | declaration_prefix type_specifier IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = ast_new_declaration($3, $2, $5, yylineno);
        $$->is_constant = ($1 != NULL); 
        free($3);
    }
    | declaration_prefix STR IDENTIFIER ASSIGN expression SEMICOLON {
        $$ = ast_new_declaration($3, TYPE_STR, $5, yylineno);
        $$->is_constant = ($1 != NULL); 
        free($3);
    }
    |  error SEMICOLON {
        yyerrok;
        fprintf(stderr, "Skipping invalid declaration at line %d\n", yylineno);
        $$ = NULL;
    }
    | declaration_prefix type_specifier error SEMICOLON {
        yyerrok;
        fprintf(stderr, "Skipping invalid declaration at line %d\n", yylineno);
        $$ = NULL;
    }
    | declaration_prefix type_specifier IDENTIFIER error {
        yyerrok;
        fprintf(stderr, "Skipping invalid declaration [semicolon missing] at line %d\n", yylineno);
        $$ = NULL;
    }
    ;

declaration_prefix:
    CONST { $$ = (ASTNode*)1; } 
    | UNSIGNED { $$ = NULL; }
    | UNSIGNED CONST { $$ = (ASTNode*)1; }
    | CONST UNSIGNED { $$ = (ASTNode*)1; }
    | { $$ = NULL; }
    ;

type_specifier:
    INT { $$ = TYPE_INT; }
    | LONG { $$ = TYPE_LONG; }
    | LONGLONG { $$ = TYPE_LONGLONG; }
    | FLOAT { $$ = TYPE_FLOAT; }
    | DOUBLE { $$ = TYPE_DOUBLE; }
    | LONGDOUBLE { $$ = TYPE_LONGDOUBLE; }
    | CHAR { $$ = TYPE_CHAR; }
    ;

statement:
    expression_statement { $$ = $1; }
    | declaration { $$ = $1; }
    | compound_statement { $$ = $1; }
    | selection_statement { $$ = $1; }
    | iteration_statement { $$ = $1; }
    | jump_statement { $$ = $1; }
    /* | error SEMICOLON {
        skip_to_semicolon();
        yyerrok;
        fprintf(stderr, "Error recovered at line %d\n", yylineno);
        $$ = NULL;
    } */
    ;

expression_statement:
    expression SEMICOLON { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    ;

compound_statement:
    LBRACE statement_list RBRACE {
        $$ = $2;
    }
    | LBRACE error RBRACE {
        yyerrok;
        fprintf(stderr, "Skipping invalid block at line %d\n", yylineno);
        $$ = ast_new_compound_statement(yylineno); //maybe is better set at NULL
    }
    ;

statement_list:
    statement_list statement {
        if (!$1) {
            // First statement - create new compound
            $$ = ast_new_compound_statement(yylineno);
        } else {
            $$ = $1;
        }
        ast_add_statement($$, $2);
    }
    | /* empty */ {
        $$ = NULL;  // Explicit empty list
    }
    ;

selection_statement:
    IF LPAREN expression RPAREN compound_statement {
        $$ = ast_new_if($3, $5, NULL, yylineno);
    }
    | IF LPAREN expression RPAREN compound_statement ELSE compound_statement {
        $$ = ast_new_if($3, $5, $7, yylineno);
    }
    ;

iteration_statement:
    WHILE LPAREN expression RPAREN compound_statement {
        $$ = ast_new_while($3, $5, yylineno);
    }
    | FOR LPAREN for_init expression_opt SEMICOLON expression_opt RPAREN compound_statement {
        ast_add_statement($8, $3);
        $$ = ast_new_for($3, $4, $6, $8, yylineno); 
    }
    | FOR LPAREN error RPAREN compound_statement {
        yyerrok;
        fprintf(stderr, "Skipping invalid for loop at line %d\n", yylineno);
        $$ = NULL;
    }
    ;

for_init:
    declaration { $$ = $1; }
    | expression_statement { $$ = $1; }
    ;

expression_opt:
    expression { $$ = $1; }
    | /* empty */ { $$ = NULL; }
    ;

jump_statement:
    BREAK SEMICOLON { $$ = ast_new_break(yylineno); }
    | CONTINUE SEMICOLON { $$ = ast_new_continue(yylineno); }
    | RETURN expression SEMICOLON { $$ = ast_new_return($2, yylineno); }
    ;

expression:
    assignment_expression { $$ = $1; }
    | error SEMICOLON {
        yyerrok;
        fprintf(stderr, "Skipping invalid expression at line %d\n", yylineno);
        $$ = NULL;
    }
    ;

assignment_expression:
    IDENTIFIER ASSIGN expression {
        $$ = ast_new_binary_op(OP_ASSIGN, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | IDENTIFIER PE expression {
        $$ = ast_new_binary_op(OP_PE, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | IDENTIFIER ME expression {
        $$ = ast_new_binary_op(OP_ME, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | IDENTIFIER MODE expression {
        $$ = ast_new_binary_op(OP_MODE, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | IDENTIFIER MULE expression {
        $$ = ast_new_binary_op(OP_MULE, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | IDENTIFIER DIVE expression {
        $$ = ast_new_binary_op(OP_DIVE, ast_new_identifier($1, yylineno), $3, yylineno);
        free($1);
    }
    | logical_or_expression { $$ = $1; }
    ;


logical_or_expression:
    logical_or_expression OR logical_and_expression { 
        $$ = ast_new_binary_op(OP_OR, $1, $3, yylineno);
    }
    | logical_and_expression { $$ = $1; }
    ;

logical_and_expression:
    logical_and_expression AND equality_expression { 
        $$ = ast_new_binary_op(OP_AND, $1, $3, yylineno);
    }
    | equality_expression { $$ = $1; }
    ;

equality_expression:
    equality_expression EQ relational_expression { 
        $$ = ast_new_binary_op(OP_EQ, $1, $3, yylineno);
    }
    | equality_expression NE relational_expression { 
        $$ = ast_new_binary_op(OP_NE, $1, $3, yylineno);
    }
    | relational_expression { $$ = $1; }
    ;

relational_expression:
    relational_expression LT additive_expression { 
        $$ = ast_new_binary_op(OP_LT, $1, $3, yylineno);
    }
    | relational_expression LE additive_expression { 
        $$ = ast_new_binary_op(OP_LE, $1, $3, yylineno);
    }
    | relational_expression GT additive_expression { 
        $$ = ast_new_binary_op(OP_GT, $1, $3, yylineno);
    }
    | relational_expression GE additive_expression { 
        $$ = ast_new_binary_op(OP_GE, $1, $3, yylineno);
    }
    | additive_expression { $$ = $1; }
    ;

additive_expression:
    additive_expression ADD multiplicative_expression { 
        $$ = ast_new_binary_op(OP_ADD, $1, $3, yylineno);
    }
    | additive_expression SUB multiplicative_expression { 
        $$ = ast_new_binary_op(OP_SUB, $1, $3, yylineno);
    }
    | multiplicative_expression { $$ = $1; }
    ;

multiplicative_expression:
    multiplicative_expression MUL unary_expression { 
        $$ = ast_new_binary_op(OP_MUL, $1, $3, yylineno);
    }
    | multiplicative_expression DIV unary_expression { 
        $$ = ast_new_binary_op(OP_DIV, $1, $3, yylineno);
    }
    | multiplicative_expression MOD unary_expression { 
        $$ = ast_new_binary_op(OP_MOD, $1, $3, yylineno);
    }
    | unary_expression { $$ = $1; }
    ;

unary_expression:
    postfix_expression { $$ = $1; }
    | INC IDENTIFIER { 
        $$ = ast_new_unary_op(OP_PRE_INC, ast_new_identifier($2, yylineno), yylineno);
        free($2);
    }
    | DEC IDENTIFIER { 
        $$ = ast_new_unary_op(OP_PRE_DEC, ast_new_identifier($2, yylineno), yylineno);
        free($2);
    }
    | NOT unary_expression { 
        $$ = ast_new_unary_op(OP_NOT, $2, yylineno);
    }
    | SUB unary_expression %prec UNARY_MINUS { 
        $$ = ast_new_unary_op(OP_NEG, $2, yylineno);
    }
    ;

postfix_expression:
    primary_expression { $$ = $1; }
    | IDENTIFIER INC { 
        $$ = ast_new_unary_op(OP_POST_INC, ast_new_identifier($1, yylineno), yylineno);
        free($1);
    }
    | IDENTIFIER DEC { 
        $$ = ast_new_unary_op(OP_POST_DEC, ast_new_identifier($1, yylineno), yylineno);
        free($1);
    }
    ;

primary_expression:
    IDENTIFIER {
        $$ = ast_new_identifier($1, yylineno);
        free($1);
    }
    | INTEGER { $$ = ast_new_integer($1, yylineno); }
    | FLOAT_LITERAL { $$ = ast_new_float($1, yylineno); }
    | STRING_LITERAL { $$ = ast_new_string($1, yylineno); free($1); }
    | CHAR_LITERAL { $$ = ast_new_char($1, yylineno); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%

const char *token_name(int token) {
    static char buf[32];
    
    // Handle single-character tokens first
    if (token < 256 && isprint(token)) {
        sprintf(buf, "'%c'", token);
        return buf;
    }
    
    switch (token) {
        /* Keywords */
        case INT: return "'int'";
        case MAIN: return "'main'";
        case IF: return "'if'";
        case ELSE: return "'else'";
        case WHILE: return "'while'";
        case FOR: return "'for'";
        case BREAK: return "'break'";
        case RETURN: return "'return'";
        
        /* Identifiers/Literals */
        case IDENTIFIER: return "identifier";
        case INTEGER: return "integer";
        
        /* Operators */
        case INC: return "'++'";
        case DEC: return "'--'";
        case PE: return "'+='";
        case ME: return "'-='";
        case ADD: return "'+'";
        case SUB: return "'-'";
        case MUL: return "'*'";
        case DIV: return "'/'";
        case MOD: return "'%'";
        case ASSIGN: return "'='";
        case EQ: return "'=='";
        case NE: return "'!='";
        case LT: return "'<'";
        case LE: return "'<='";
        case GT: return "'>'";
        case GE: return "'>='";
        case NOT: return "'!'";
        case AND: return "'&&'";
        case OR: return "'||'";
        
        /* Punctuation */
        case SEMICOLON: return "';'";
        case COMMA: return "','";
        case LPAREN: return "'('";
        case RPAREN: return "')'";
        case LBRACE: return "'{'";
        case RBRACE: return "'}'";
        
        /* Special cases */
        case 0: return "end of input";
        case YYerror: return "error";
        case YYUNDEF: return "invalid token";
        
        default: {
            sprintf(buf, "token %d", token);
            return buf;
        }
    }
}


void yyerror(const char *msg) {
    if (yychar != YYEMPTY && yychar != YYEOF) {
        fprintf(stderr, "Line %d: Syntax error at %s, unexpected %s",
                yylineno,
                last_expected ? last_expected : "input",
                token_name(yychar));
        
        if (last_expected) {
            fprintf(stderr, ", expected %s\n", last_expected);
        } else {
            fprintf(stderr, "\n");
        }
    } else {
        fprintf(stderr, "Line %d: Syntax error at %s\n", 
                yylineno,
                last_expected ? last_expected : "input");
    }
    last_expected = NULL;
}

