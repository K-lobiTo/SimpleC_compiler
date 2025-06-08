%{
#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"

extern int yylex();

ASTNode *program_root = NULL;

void yyerror(const char* s) { // override (error handling)
    compiler_error(s);
}

%}


%union {
    int number;
    char *string;
    struct ast_node *node;
}

%token <number> INTEGER
%token <string> IDENTIFIER
%token INT MAIN MAINFUN
%token BREAK IF ELSE WHILE FOR RETURN
%token ADD SUB MUL DIV MOD
%token ASSIGN
%token SEMICOLON COMMA
%token LPAREN RPAREN LBRACE RBRACE
%token EQ NE LT LE GT GE
%token NOT AND OR
%token INC DEC PE ME


%type <node> program global_declarations
%type <node> declaration expression statement
%type <node> statement_list expression_statement
%type <node> selection_statement iteration_statement jump_statement
%type <node> compound_statement
%type <node> assignment_expression logical_or_expression logical_and_expression
%type <node> equality_expression relational_expression additive_expression
%type <node> multiplicative_expression postfix_expression unary_expression primary_expression

%%

program:
    global_declarations MAINFUN LPAREN RPAREN compound_statement {
        $$ = new_main_function_node("main", NULL, $5);
        program_root = $$;
    }
    ;

global_declarations:
    global_declarations declaration { $$ = new_compound_node((ASTNode*[]){$1, $2}, 2); }
    | /* empty */ { $$ = NULL; }
    ;

declaration:
    INT IDENTIFIER SEMICOLON { 
        // if(lookup_symbol($2)){
        //     report_error(@2.first_line, @2.first_colum,
        //     "Redeclaration of variable", filename);
        // }
        // else{
        //     add_symbol($2);
        // }
        $$ = new_decl_node($2); 

        }
    | INT IDENTIFIER ASSIGN expression SEMICOLON { $$ = new_decl_init_node($2, $4); }
    ;

statement:
    expression_statement
    | declaration
    | compound_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

expression_statement:
    expression SEMICOLON { $$ = $1; }
    | SEMICOLON { $$ = NULL; }
    ;

compound_statement:
    LBRACE statement_list RBRACE { $$ = $2; }
    ;

statement_list:
    statement_list statement { $$ = new_compound_node((ASTNode*[]){$1, $2}, 2); }
    | /* empty */ { $$ = NULL; }
    ;

selection_statement:
    IF LPAREN expression RPAREN compound_statement { $$ = new_if_node($3, $5, NULL); }
    | IF LPAREN expression RPAREN compound_statement ELSE compound_statement { $$ = new_if_node($3, $5, $7); }
    ;

iteration_statement:
    WHILE LPAREN expression RPAREN compound_statement { $$ = new_while_node($3, $5); }
    | FOR LPAREN declaration expression_statement expression RPAREN compound_statement {
        $$ = new_for_node($3, $4, $5, $7);
      }
    ;

jump_statement:
    BREAK SEMICOLON { $$ = new_break_node(); }
    | RETURN expression SEMICOLON { $$ = new_return_node($2); }
    ;

expression:
    assignment_expression
    ;

assignment_expression:
    IDENTIFIER ASSIGN expression { $$ = new_assign_node(new_ident_node($1), $3); }
    | IDENTIFIER PE expression { $$ = new_compound_assign_node($1, OP_PE, $3); }
    | IDENTIFIER ME expression { $$ = new_compound_assign_node($1, OP_ME, $3); }
    | logical_or_expression
    ;

logical_or_expression:
    logical_or_expression OR logical_and_expression { $$ = new_binop_node(OP_OR, $1, $3); }
    | logical_and_expression
    ;

logical_and_expression:
    logical_and_expression AND equality_expression { $$ = new_binop_node(OP_AND, $1, $3); }
    | equality_expression
    ;

equality_expression:
    equality_expression EQ relational_expression { $$ = new_binop_node(OP_EQ, $1, $3); }
    | equality_expression NE relational_expression { $$ = new_binop_node(OP_NE, $1, $3); }
    | relational_expression
    ;

relational_expression:
    relational_expression LT additive_expression { $$ = new_binop_node(OP_LT, $1, $3); }
    | relational_expression LE additive_expression { $$ = new_binop_node(OP_LE, $1, $3); }
    | relational_expression GT additive_expression { $$ = new_binop_node(OP_GT, $1, $3); }
    | relational_expression GE additive_expression { $$ = new_binop_node(OP_GE, $1, $3); }
    | additive_expression
    ;

additive_expression:
    additive_expression ADD multiplicative_expression { $$ = new_binop_node(OP_ADD, $1, $3); }
    | additive_expression SUB multiplicative_expression { $$ = new_binop_node(OP_SUB, $1, $3); }
    | multiplicative_expression
    ;

multiplicative_expression:
    multiplicative_expression MUL unary_expression { $$ = new_binop_node(OP_MUL, $1, $3); }
    | multiplicative_expression DIV unary_expression { $$ = new_binop_node(OP_DIV, $1, $3); }
    | multiplicative_expression MOD unary_expression { $$ = new_binop_node(OP_MOD, $1, $3); }
    | unary_expression
    ;

unary_expression:
    postfix_expression
    | INC IDENTIFIER { $$ = new_prefix_op_node(OP_INC, $2); }
    | DEC IDENTIFIER { $$ = new_prefix_op_node(OP_DEC, $2); }
    | NOT unary_expression { $$ = new_not_node($2); }
    ;

postfix_expression:
    primary_expression
    | IDENTIFIER INC { $$ = new_postfix_op_node(OP_INC, $1); }
    | IDENTIFIER DEC { $$ = new_postfix_op_node(OP_DEC, $1); }
    ;

primary_expression:
    IDENTIFIER { $$ = new_ident_node($1); }
    | INTEGER { $$ = new_int_node($1); }
    | LPAREN expression RPAREN { $$ = $2; }
    ;

%%
