%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "compiler.h"

extern int yylex();

ASTNode *program_root = NULL;

// Enhanced error reporting
extern int yylineno;
void yyerror(const char *msg);
const char *token_name(int token);

// For error reporting
extern int yychar;
static const char *last_expected = NULL;

%}

%define parse.error verbose
%define parse.lac full
%token-table


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
    | IF error %prec LBRACE {  // Add precedence
        last_expected = "expected '(' after 'if'";
        yyerrok;  // Add error recovery
        YYERROR;
    }
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