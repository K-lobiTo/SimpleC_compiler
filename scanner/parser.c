#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode* parse_compound_statement(void);
static ASTNode* parse_statement(void);
static ASTNode* parse_return_statement(void);
static ASTNode* parse_if_statement(void);
static ASTNode* parse_if_statement(void);
static ASTNode* parse_while_statement(void);
static ASTNode* parse_for_statement(void);
static ASTNode* parse_declaration(void);
static ASTNode* parse_expression_statement(void);
static ASTNode* expression(void);
static ASTNode* equality(void);
static ASTNode* comparison(void);
static ASTNode* term(void);
static ASTNode* factor(void);
static ASTNode* unary(void);
static ASTNode* primary(void);

static Token current_token;
static int had_error = 0;

int parser_had_error(){
    return had_error;
}

// Helper functions
static void advance() {
    current_token = Get_Token();
}

static void consume(TokenType expected) {
    if (current_token.type == expected) {
        advance();
    } else {
        char error_msg[100];
        snprintf(error_msg, sizeof(error_msg), 
                "Expected %s but found %s",
                tTypeToStr(expected),
                tTypeToStr(current_token.type));
        parser_error(error_msg);
    }
}

static ASTNode* new_node(NodeType type, Token token) {
    ASTNode* node = malloc(sizeof(ASTNode));
    node->type = type;
    node->token = token;
    node->left = node->right = node->extra = node->next = NULL;
    return node;
}

void parser_error(const char* message) {
    fprintf(stderr, "Parser error at line %d: %s\n", current_token.line, message);
    had_error = 1;
}

// Expression parsing
static ASTNode* expression();
static ASTNode* equality();
static ASTNode* comparison();
static ASTNode* term();
static ASTNode* factor();
static ASTNode* unary();
static ASTNode* primary();

ASTNode* parse_program() {
    advance(); // Get first token
    
    ASTNode* program = new_node(NODE_PROGRAM, current_token);
    
    // Expect 'int main()'
    if (current_token.type != TOK_INT) {
        parser_error("Expected 'int' at start of program");
        return NULL;
    }
    advance();
    
    if (current_token.type != TOK_IDENTIFIER || strcmp(current_token.lexeme, "main") != 0) {
        parser_error("Expected 'main' function");
        return NULL;
    }
    advance();
    
    if (current_token.type != TOK_LPAREN) {
        parser_error("Expected '(' after 'main'");
        return NULL;
    }
    advance();
    
    if (current_token.type != TOK_RPAREN) {
        parser_error("Expected ')' after 'main('");
        return NULL;
    }
    advance();
    
    // Parse compound statement
    program->left = parse_compound_statement();
    
    if (current_token.type != TOK_EOF) {
        parser_error("Unexpected tokens after main function");
    }
    
    return program;
}

static ASTNode* parse_compound_statement() {
    if (current_token.type != TOK_LBRACE) {
        parser_error("Expected '{' at start of compound statement");
        return NULL;
    }
    consume(TOK_LBRACE);
    // advance();
    
    ASTNode* compound = new_node(NODE_COMPOUND_STMT, current_token);
    ASTNode* current = NULL;
    
    while (current_token.type != TOK_RBRACE && current_token.type != TOK_EOF) {
        ASTNode* stmt = parse_statement();
        
        if (!compound->left) {
            compound->left = stmt;
            current = stmt;
        } else {
            current->next = stmt;
            current = stmt;
        }
        // if (current == NULL) {
        //     compound->left = stmt;
        //     current = stmt;
        // } else {
        //     current->next = stmt;
        //     current = stmt;
        // }
    }
    
    if (current_token.type != TOK_RBRACE) {
        parser_error("Expected '}' at end of compound statement");
    }
    consume(TOK_RBRACE);
    
    return compound;
}

static ASTNode* parse_statement() {
    switch (current_token.type) {
        case TOK_RETURN:
            return parse_return_statement();
        case TOK_IF:
            return parse_if_statement();
        case TOK_WHILE:
            return parse_while_statement();
        case TOK_FOR:
            return parse_for_statement();
        case TOK_LBRACE:
            return parse_compound_statement();
        case TOK_INT:
            return parse_declaration();
        default:
            return parse_expression_statement();
    }
}

static ASTNode* parse_return_statement(){
    Token return_token = current_token;
    advance();  // Consume 'return'
    
    ASTNode* return_node = new_node(NODE_RETURN, return_token);
    
    // Parse optional expression
    if (current_token.type != TOK_SEMICOLON) {
        return_node->left = expression();
    }
    
    consume(TOK_SEMICOLON);
    return return_node;
}

static ASTNode* parse_if_statement() {
    Token if_token = current_token;
    consume(TOK_IF);
    consume(TOK_LPAREN);
    
    ASTNode* condition = expression();
    consume(TOK_RPAREN);
    
    ASTNode* if_branch = parse_statement();
    ASTNode* else_branch = NULL;
    
    if (current_token.type == TOK_ELSE) {
        advance();
        else_branch = parse_statement();
    }
    
    ASTNode* if_node = new_node(NODE_IF, if_token);
    if_node->left = condition;
    if_node->right = if_branch;
    if_node->extra = else_branch;
    
    return if_node;
}

static ASTNode* parse_while_statement() {
    Token while_token = current_token;
    consume(TOK_WHILE);
    consume(TOK_LPAREN);
    
    ASTNode* condition = expression();
    consume(TOK_RPAREN);
    
    ASTNode* body = parse_statement();
    
    ASTNode* while_node = new_node(NODE_WHILE, while_token);
    while_node->left = condition;
    while_node->right = body;
    
    return while_node;
}

static ASTNode* parse_for_statement() {
    Token for_token = current_token;
    consume(TOK_FOR);
    consume(TOK_LPAREN);
    
    // Initializer (can be empty)
    ASTNode* initializer = NULL;
    if (current_token.type != TOK_SEMICOLON) {
        initializer = parse_expression_statement();
    } else {
        advance(); // Skip semicolon
    }
    
    // Condition (can be empty)
    ASTNode* condition = NULL;
    if (current_token.type != TOK_SEMICOLON) {
        condition = expression();
    }
    consume(TOK_SEMICOLON);
    
    // Increment (can be empty)
    ASTNode* increment = NULL;
    if (current_token.type != TOK_RPAREN) {
        increment = expression();
    }
    consume(TOK_RPAREN);
    
    ASTNode* body = parse_statement();
    
    ASTNode* for_node = new_node(NODE_FOR, for_token);
    for_node->left = initializer;
    for_node->right = condition;
    for_node->extra = increment;
    // Body goes in the "next" field
    ASTNode* wrapper = new_node(NODE_COMPOUND_STMT, for_token);
    wrapper->left = body;
    if (increment) {
        wrapper->next = increment;
    }
    for_node->next = wrapper;
    
    return for_node;
}

static ASTNode* parse_declaration() {
    Token type_token = current_token;
    
    if (current_token.type != TOK_INT && 
        current_token.type != TOK_DOUBLE && 
        current_token.type != TOK_LONG) {
        parser_error("Expected type specifier");
        return NULL;
    }
    advance();
    
    if (current_token.type != TOK_IDENTIFIER) {
        parser_error("Expected variable name");
        return NULL;
    }
    
    Token var_token = current_token;
    advance();
    
    ASTNode* decl = new_node(NODE_VARIABLE, var_token);
    decl->extra = new_node(NODE_TYPE, type_token);
    
    if (current_token.type == TOK_ASSIGN) {
        advance();
        ASTNode* assign = new_node(NODE_ASSIGNMENT, var_token);
        assign->left = decl;
        assign->right = expression();
        decl = assign;
    }
    
    consume(TOK_SEMICOLON);
    return decl;
}

static ASTNode* parse_expression_statement() {
    ASTNode* expr = expression();
    consume(TOK_SEMICOLON);
    return expr;
}

// Expression parsing functions
static ASTNode* expression() {
    return equality();
}

static ASTNode* equality() {
    ASTNode* node = comparison();
    
    while (current_token.type == TOK_EQ || current_token.type == TOK_NEQ) {
        Token op = current_token;
        advance();
        ASTNode* right = comparison();
        ASTNode* binary = new_node(NODE_BINARY_OP, op);
        binary->left = node;
        binary->right = right;
        node = binary;
    }
    
    return node;
}

static ASTNode* comparison() {
    ASTNode* node = term();
    
    while (current_token.type == TOK_LT || current_token.type == TOK_GT || 
           current_token.type == TOK_LEQ || current_token.type == TOK_GEQ) {
        Token op = current_token;
        advance();
        ASTNode* right = term();
        ASTNode* binary = new_node(NODE_BINARY_OP, op);
        binary->left = node;
        binary->right = right;
        node = binary;
    }
    
    return node;
}

static ASTNode* term() {
    ASTNode* node = factor();
    
    while (current_token.type == TOK_PLUS || current_token.type == TOK_MINUS) {
        Token op = current_token;
        advance();
        ASTNode* right = factor();
        ASTNode* binary = new_node(NODE_BINARY_OP, op);
        binary->left = node;
        binary->right = right;
        node = binary;
    }
    
    return node;
}

static ASTNode* factor() {
    ASTNode* node = unary();
    
    while (current_token.type == TOK_MULT || current_token.type == TOK_DIV) {
        Token op = current_token;
        advance();
        ASTNode* right = unary();
        ASTNode* binary = new_node(NODE_BINARY_OP, op);
        binary->left = node;
        binary->right = right;
        node = binary;
    }
    
    return node;
}

static ASTNode* unary() {
    if (current_token.type == TOK_MINUS || current_token.type == TOK_NOT) {
        Token op = current_token;
        advance();
        ASTNode* node = new_node(NODE_UNARY_OP, op);
        node->left = unary();
        return node;
    }
    return primary();
}

static ASTNode* primary() {
    switch (current_token.type) {
        case TOK_INT_LITERAL: {
            ASTNode* node = new_node(NODE_INT_LITERAL, current_token);
            advance();
            return node;
        }
        case TOK_IDENTIFIER: {
            ASTNode* node = new_node(NODE_VARIABLE, current_token);
            advance();
            return node;
        }
        case TOK_LPAREN: {
            advance();
            ASTNode* expr = expression();
            consume(TOK_RPAREN);
            return expr;
        }
        default:
            parser_error("Expected expression");
            return NULL;
    }
}

void free_ast(ASTNode* node) {
    if (!node) return;

     if (node->left && node->left != node->right && node->left != node->extra) {
        free_ast(node->left);
    }
    node->left = NULL;

    if (node->right && node->right != node->extra) {
        free_ast(node->right);
    }
    node->right = NULL;

    if (node->extra) {
        free_ast(node->extra);
    }
    node->extra = NULL;

    if (node->next) {
        ASTNode* next = node->next;
        node->next = NULL;
        free_ast(next);
    }

    if (node->token.lexeme) {
        free(node->token.lexeme);
        node->token.lexeme = NULL;
    }

    free(node);
}

void print_ast(ASTNode* node, int indent) {
    if (node == NULL) return;
    
    for (int i = 0; i < indent; i++) printf("  ");
    
    switch (node->type) {
        case NODE_PROGRAM: printf("Program\n"); break;
        case NODE_STATEMENT_LIST: printf("Statement List\n"); break;
        case NODE_EXPRESSION: printf("Expression\n"); break;
        case NODE_ASSIGNMENT: printf("Assignment\n"); break;
        case NODE_RETURN: printf("Return\n"); break;
        case NODE_IF: printf("If\n"); break;
        case NODE_WHILE: printf("While\n"); break;
        case NODE_FOR: printf("For\n"); break;
        case NODE_BINARY_OP: 
            printf("Binary Op: %s\n", tTypeToStr(node->token.type));
            break;
        case NODE_UNARY_OP:
            printf("Unary Op: %s\n", tTypeToStr(node->token.type));
            break;
        case NODE_INT_LITERAL:
            printf("Int Literal: %lld\n", node->token.value.int_val);
            break;
        case NODE_STRING_LITERAL:
            printf("String Literal: %s\n", node->token.lexeme);
            break;
        case NODE_VARIABLE:
            printf("Variable: %s\n", node->token.lexeme);
            break;
        case NODE_TYPE:
            printf("Node Type: \n");
            break;
        case NODE_DOUBLE_LITERAL:
            printf("Int Literal: %Lf\n", node->token.value.float_val);
            break;
        case NODE_COMPOUND_STMT:
            printf("Compound Statement\n");
            break;
    }
    
    print_ast(node->left, indent + 1);
    print_ast(node->right, indent + 1);
    print_ast(node->extra, indent + 1);

    print_ast(node->next, indent);
}

void print_ast_pointers(ASTNode* node, int indent) {
    if (!node) {
        printf("%*s[NULL]\n", indent, "");
        return;
    }
    
    printf("%*sNode %p (type %d)\n", indent, "", node, node->type);
    print_ast_pointers(node->left, indent + 2);
    print_ast_pointers(node->right, indent + 2);
    print_ast_pointers(node->extra, indent + 2);
    print_ast_pointers(node->next, indent + 2);
}
