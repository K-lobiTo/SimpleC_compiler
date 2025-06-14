// ast.c
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compiler.h"

ASTNode *ast_new_program(ScopeStack *symbol_table) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_PROGRAM;
    node->symbol_table = symbol_table;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

ASTNode *ast_new_declaration(char *name, int type, ASTNode *init_value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_DECLARATION;
    node->line_number = line;
    node->var_name = strdup(name);
    node->var_type = type;
    node->init_value = init_value;
    return node;
}

ASTNode *ast_new_identifier(char *name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IDENTIFIER;
    node->line_number = line;
    node->string_value = strdup(name);
    return node;
}

ASTNode *ast_new_integer(long long value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_INTEGER;
    node->line_number = line;
    node->int_value = value;
    return node;
}

ASTNode *ast_new_float(double value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FLOAT;
    node->line_number = line;
    node->float_value = value;
    return node;
}

ASTNode *ast_new_string(char *value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_STRING;
    node->line_number = line;
    node->string_value = strdup(value);
    return node;
}

ASTNode *ast_new_char(char value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_CHAR;
    node->line_number = line;
    node->char_value = value;
    return node;
}

ASTNode *ast_new_binary_op(OperatorType op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_BINARY_OP;
    node->line_number = line;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *ast_new_unary_op(OperatorType op, ASTNode *expr, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_UNARY_OP;
    node->line_number = line;
    node->op = op;
    node->left = expr;
    node->right = NULL;
    return node;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_part, ASTNode *else_part, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_IF;
    node->line_number = line;
    node->cond = cond;
    node->then_part = then_part;
    node->else_part = else_part;
    return node;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_WHILE;
    node->line_number = line;
    node->cond = cond;
    node->then_part = body;  // Reusing then_part as body
    return node;
}

ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *step, ASTNode *body, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_FOR;
    node->line_number = line;
    node->init = init;
    node->cond = cond;
    node->step = step;
    node->then_part = body;  // Reusing then_part as body
    return node;
}

ASTNode *ast_new_compound_statement(int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    node->type = NODE_COMPOUND;
    node->line_number = line;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

ASTNode *ast_add_statement(ASTNode *compound, ASTNode *stmt) {
    if (compound->type != NODE_COMPOUND) {
        return NULL;
    }
    
    compound->children_count++;
    compound->children = realloc(compound->children, 
                                compound->children_count * sizeof(ASTNode *));
    compound->children[compound->children_count - 1] = stmt;
    return compound;
}

ASTNode *ast_new_continue(int line) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        compiler_error("Memory allocation failed for continue node");
        return NULL;
    }

    node->type = NODE_CONTINUE;
    node->line_number = line;
    node->symbol_table = NULL;  // Continue doesn't need symbol table
    
    return node;
}

ASTNode *ast_new_break(int line) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        compiler_error("Memory allocation failed for break node");
        return NULL;
    }

    node->type = NODE_BREAK;
    node->line_number = line;
    node->symbol_table = NULL;  // Break doesn't need symbol table context
    
    return node;
}

ASTNode *ast_new_return(ASTNode *expr, int line) {
    ASTNode *node = (ASTNode *)malloc(sizeof(ASTNode));
    if (!node) {
        compiler_error("Memory allocation failed for return node");
        return NULL;
    }

    node->type = NODE_RETURN;
    node->line_number = line;
    node->symbol_table = NULL;  // Return doesn't need its own symbol table
    
    // Store the return expression (can be NULL for empty returns)
    node->left = expr;  // Using left child for the return expression
    
    return node;
}

void ast_free(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_IDENTIFIER:
        case NODE_STRING:
        case NODE_DECLARATION:
            free(node->string_value);
            break;
            
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            ast_free(node->left);
            ast_free(node->right);
            break;
            
        case NODE_IF:
        case NODE_CONTINUE:
            break;
        case NODE_WHILE:
            ast_free(node->cond);
            ast_free(node->then_part);
            ast_free(node->else_part);
            break;
            
        case NODE_FOR:
            ast_free(node->init);
            ast_free(node->cond);
            ast_free(node->step);
            ast_free(node->then_part);
            break;
            
        case NODE_COMPOUND:
            for (size_t i = 0; i < node->children_count; i++) {
                ast_free(node->children[i]);
            }
            free(node->children);
            break;
            
        default:
            break;
    }
    
    free(node);
}


void semantic_analyze(ASTNode *node) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_DECLARATION: {
            // Check if variable is already declared in current scope
            if (search_in_current_scope(node->symbol_table, node->var_name)) {
                char error_msg[100];
                snprintf(error_msg, sizeof(error_msg), 
                         "Redeclaration of variable '%s' in the same scope", node->var_name);
                compiler_error(error_msg);
            } else {
                // Insert into symbol table (type doesn't matter for your requirements)
                insert_in_current_scope(node->symbol_table, node->var_name, false, 0, node->line_number);
            }
            // Analyze initialization expression if present
            semantic_analyze(node->init_value);
            break;
        }
            
        case NODE_IDENTIFIER: {
            // Check if variable is declared
            if (!search_in_all_scopes(node->symbol_table, node->string_value)) {
                char error_msg[100];
                snprintf(error_msg, sizeof(error_msg), 
                         "Use of undeclared variable '%s'", node->string_value);
                compiler_error(error_msg);
            }
            break;
        }
            
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            semantic_analyze(node->left);
            semantic_analyze(node->right);
            break;
            
        case NODE_IF:
            semantic_analyze(node->cond);
            semantic_analyze(node->then_part);
            semantic_analyze(node->else_part);
            break;
            
        case NODE_WHILE:
            semantic_analyze(node->cond);
            semantic_analyze(node->then_part);
            break;
            
        case NODE_FOR:
            semantic_analyze(node->init);
            semantic_analyze(node->cond);
            semantic_analyze(node->step);
            semantic_analyze(node->then_part);
            break;
            
        case NODE_COMPOUND:
            for (size_t i = 0; i < node->children_count; i++) {
                semantic_analyze(node->children[i]);
            }
            break;
            
        default:
            break;
    }
}

// In ast.c

static int label_counter = 0;

void generate_assembly(ASTNode *node, FILE *out) {
    if (!node) return;
    
    switch (node->type) {
        case NODE_PROGRAM:
            fprintf(out, ".globl main\n");
            fprintf(out, "main:\n");
            fprintf(out, "  push %%ebp\n");
            fprintf(out, "  mov %%esp, %%ebp\n");
            
            // Generate code for all statements
            for (size_t i = 0; i < node->children_count; i++) {
                generate_assembly(node->children[i], out);
            }
            
            fprintf(out, "  mov $0, %%eax\n");  // Return 0
            fprintf(out, "  leave\n");
            fprintf(out, "  ret\n");
            break;
            
        case NODE_DECLARATION:
            // Allocate space on stack
            fprintf(out, "  sub $8, %%esp  # Allocate space for %s\n", node->var_name);
            
            // If there's an initializer
            if (node->init_value) {
                generate_assembly(node->init_value, out);
                fprintf(out, "  mov %%eax, -%d(%%ebp)  # Store %s\n", 
                        (node->symbol_table->top + 1) * 8, node->var_name);
            }
            break;
            
        case NODE_IDENTIFIER:
            // Load variable from stack
            fprintf(out, "  mov -%d(%%ebp), %%eax  # Load %s\n", 
                    (node->symbol_table->top + 1) * 8, node->string_value);
            break;
            
        case NODE_INTEGER:
            fprintf(out, "  mov $%lld, %%eax\n", node->int_value);
            break;
            
        case NODE_FLOAT:
            // For simplicity, we'll treat it as integer here
            fprintf(out, "  mov $%d, %%eax\n", (int)node->float_value);
            break;
            
        case NODE_BINARY_OP: {
            generate_assembly(node->right, out);
            fprintf(out, "  push %%eax\n");
            generate_assembly(node->left, out);
            fprintf(out, "  pop %%ecx\n");
            
            switch (node->op) {
                case OP_ADD:
                    fprintf(out, "  add %%ecx, %%eax\n");
                    break;
                case OP_SUB:
                    fprintf(out, "  sub %%ecx, %%eax\n");
                    break;
                case OP_MUL:
                    fprintf(out, "  imul %%ecx, %%eax\n");
                    break;
                case OP_DIV:
                    fprintf(out, "  cdq\n");
                    fprintf(out, "  idiv %%ecx\n");
                    break;
                case OP_ASSIGN:
                    fprintf(out, "  mov %%eax, -%d(%%ebp)  # Store to %s\n", 
                            (node->left->symbol_table->top + 1) * 8, 
                            node->left->string_value);
                    break;
                // Add other operations as needed
                default:
                    break;
            }
            break;
        }
            
        case NODE_IF: {
            int else_label = label_counter++;
            int end_label = label_counter++;
            
            generate_assembly(node->cond, out);
            fprintf(out, "  cmp $0, %%eax\n");
            fprintf(out, "  je L%d\n", else_label);
            
            generate_assembly(node->then_part, out);
            fprintf(out, "  jmp L%d\n", end_label);
            
            fprintf(out, "L%d:\n", else_label);
            if (node->else_part) {
                generate_assembly(node->else_part, out);
            }
            
            fprintf(out, "L%d:\n", end_label);
            break;
        }
            
        case NODE_WHILE: {
            int start_label = label_counter++;
            int end_label = label_counter++;
            
            fprintf(out, "L%d:\n", start_label);
            generate_assembly(node->cond, out);
            fprintf(out, "  cmp $0, %%eax\n");
            fprintf(out, "  je L%d\n", end_label);
            
            generate_assembly(node->then_part, out);
            fprintf(out, "  jmp L%d\n", start_label);
            
            fprintf(out, "L%d:\n", end_label);
            break;
        }
            
        // Add other node types as needed
            
        default:
            break;
    }
}