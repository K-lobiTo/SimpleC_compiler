// ast.c
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "compiler.h"


const char* node_type_to_str(NodeType type);

ASTNode *ast_new_program(ScopeStack *symbol_table) {
    ASTNode *node = calloc(1, sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_PROGRAM;
    node->symbol_table = symbol_table;
    node->children = NULL;
    node->children_count = 0;
    node->line_number = 0;
    return node;
}

ASTNode *ast_new_declaration(char *name, int type, ASTNode *init_value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_DECLARATION;
    node->line_number = line;
    node->var_name = strdup(name);
    node->var_type = type;
    node->is_constant = false;
    node->init_value = init_value;
    return node;
}

ASTNode *ast_new_identifier(char *name, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_IDENTIFIER;
    node->line_number = line;
    node->string_value = strdup(name);
    return node;
}

ASTNode *ast_new_integer(long long value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_INTEGER;
    node->line_number = line;
    node->int_value = value;
    node->left = NULL;
    node->right = NULL;
    return node;
}

ASTNode *ast_new_float(double value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_FLOAT;
    node->line_number = line;
    node->float_value = value;
    return node;
}

ASTNode *ast_new_string(char *value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_STRING;
    node->line_number = line;
    node->string_value = strdup(value);
    return node;
}

ASTNode *ast_new_char(char value, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_CHAR;
    node->line_number = line;
    node->char_value = value;
    return node;
}

ASTNode *ast_new_binary_op(OperatorType op, ASTNode *left, ASTNode *right, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_BINARY_OP;
    node->line_number = line;
    node->op = op;
    node->left = left;
    node->right = right;
    return node;
}

ASTNode *ast_new_unary_op(OperatorType op, ASTNode *expr, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_UNARY_OP;
    node->line_number = line;
    node->op = op;
    node->left = expr;
    node->right = NULL;
    return node;
}

ASTNode *ast_new_if(ASTNode *cond, ASTNode *then_part, ASTNode *else_part, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_IF;
    node->line_number = line;
    node->cond = cond;
    node->then_part = then_part;
    node->else_part = else_part;
    return node;
}

ASTNode *ast_new_while(ASTNode *cond, ASTNode *body, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

    node->type = NODE_WHILE;
    node->line_number = line;
    node->cond = cond;
    node->then_part = body;  // Reusing then_part as body
    return node;
}

ASTNode *ast_new_for(ASTNode *init, ASTNode *cond, ASTNode *step, ASTNode *body, int line) {
    ASTNode *node = malloc(sizeof(ASTNode));
    if(!node) return NULL;

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
    if(!node) return NULL;

    node->type = NODE_COMPOUND;
    node->line_number = line;
    node->children = NULL;
    node->children_count = 0;
    return node;
}

ASTNode *ast_add_statement(ASTNode *compound, ASTNode *stmt) {
    // Validate inputs
    if (!compound || !stmt) {
        compiler_error("Null pointer passed to ast_add_statement");
        return NULL;
    }

    if (compound->type != NODE_COMPOUND && compound->type != NODE_PROGRAM) {
        compiler_error("Cannot add statement to non-compound node");
        return NULL;
    }

    // Calculate new size
    size_t new_size = compound->children_count + 1;
    
    // Reallocate memory
    ASTNode **new_children = realloc(compound->children, new_size * sizeof(ASTNode *));
    if (!new_children) {
        compiler_error("Memory allocation failed in ast_add_statement");
        return NULL;
    }

    // Update structure
    compound->children = new_children;
    compound->children[compound->children_count] = stmt;  // Note: no -1 here
    compound->children_count = new_size;
    
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
            if (node->string_value) free(node->string_value);
            break;
            
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            ast_free(node->left);
            ast_free(node->right);
            break;
            
        case NODE_IF:
            ast_free(node->cond);
            ast_free(node->then_part);
            ast_free(node->else_part);
            break;
            
        case NODE_WHILE:
            ast_free(node->cond);
            ast_free(node->then_part);
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
            
        case NODE_RETURN:
            ast_free(node->left); // Return expression
            break;
            
        default:
            // For simple nodes like INTEGER, FLOAT, CHAR
            break;
    }
    
    free(node);
}


int scope_depth = -1;

void semantic_analyze(ASTNode *node, ScopeStack *symbol_table) {
    if (!node){
        // printf("No node in SemAnalyze\n");
        return;
    }
    assert(symbol_table);
    
    // Set symbol table reference for all nodes
    node->symbol_table = symbol_table;
    
    if(node->type<NODE_PROGRAM || node->type>NODE_CONTINUE){
        // printf("No node type in SemAnalyze \n");
        return;
    }
    // printf("NodeType : %s, NodeLineNo: %d\n", node_type_to_str(node->type), node->line_number);
    if (node->type < NODE_PROGRAM || node->type > NODE_CONTINUE) {
        fprintf(stderr, "Error: Invalid node type %d at line %d\n", node->type, node->line_number);
        abort();
    }
    switch (node->type) {
        case NODE_DECLARATION: {
            // Check if variable is already declared in current scope
            if (search_in_current_scope(symbol_table, node->var_name)) {
                char error_msg[100];
                snprintf(error_msg, sizeof(error_msg), 
                        "Redeclaration of variable '%s' at line %d", 
                        node->var_name, node->line_number);
                compiler_error(error_msg);
            } else {
                // Insert into symbol table
                // printf("Declaring %s as %s (type: %d, line: %d)\n",
                //     node->var_name,
                //     node->is_constant ? "const" : "variable",
                //     node->var_type,
                //     node->line_number);
                insert_in_current_scope(symbol_table, node->var_name, 
                                      node->is_constant, node->var_type, 
                                      node->line_number);
                // printf("Inserted variable: %s (scope depth: %d)\n", 
                //       node->var_name, symbol_table->top + 1);
            }
            if(node->init_value)semantic_analyze(node->init_value, symbol_table);
            break;
        }
            
        case NODE_IDENTIFIER: {
            if (!search_in_all_scopes(symbol_table, node->string_value)) {
                char error_msg[100];
                snprintf(error_msg, sizeof(error_msg),
                        "Undeclared variable '%s' at line %d",
                        node->string_value, node->line_number);
                compiler_error(error_msg);
            }
            break;
        }
            
        case NODE_COMPOUND: {
            push_scope(symbol_table);
            printf("Entering new scope (depth: %d)\n", ++scope_depth);
            
            for (size_t i = 0; i < node->children_count; i++) {
                semantic_analyze(node->children[i], symbol_table);
            }
            if(symbol_table->top==0)break; // keeping global until the end of the program
            printf("Leaving scope (depth: %d)\n", scope_depth--);
            print_trie(symbol_table->scopes[symbol_table->top]); // Print before popping
            pop_scope(symbol_table);
            break;
        }
        case NODE_PROGRAM: {
            printf("Entering program Initialization (depth: %d)\n", 0);
            
            for (size_t i = 0; i < node->children_count; i++) {
                semantic_analyze(node->children[i], symbol_table);
            }
            if(symbol_table->top==0){
                printf("Leaving global scope (depth: %d)\n", scope_depth--);
                print_trie(symbol_table->scopes[symbol_table->top]); // printing global scope
                pop_scope(symbol_table); //poping global scope
            }
            
            break;
        }
            
        case NODE_BINARY_OP:
        case NODE_UNARY_OP:
            semantic_analyze(node->left, symbol_table);
            semantic_analyze(node->right, symbol_table);
            break;
            
        case NODE_IF:
            semantic_analyze(node->cond, symbol_table);
            semantic_analyze(node->then_part, symbol_table);
            semantic_analyze(node->else_part, symbol_table);
            break;
            
        case NODE_WHILE:
            semantic_analyze(node->cond, symbol_table);
            semantic_analyze(node->then_part, symbol_table);
            break;
            
        case NODE_FOR:
            push_scope(symbol_table);
            // printf("Entering FOR scope (depth: %d)\n", symbol_table->top + 1);
            semantic_analyze(node->init, symbol_table);
            semantic_analyze(node->cond, symbol_table);
            semantic_analyze(node->step, symbol_table);
            semantic_analyze(node->then_part, symbol_table);
            // printf("Leaving FOR (depth: %d)\n", symbol_table->top + 1);
            // print_trie(symbol_table->scopes[symbol_table->top]);
            pop_scope(symbol_table);
            break;
            
            
        default:
            // Recursively analyze children
            if(node->left)semantic_analyze(node->left, symbol_table);
            if(node->left)semantic_analyze(node->right, symbol_table);
            break;
    }
}




// PRINTING PRINTING PRINTING PRINTING PRINTING
const char* var_type_to_str(VarType type) {
    static const char* names[] = {
        [TYPE_INT] = "int",
        [TYPE_LONG] = "long",
        [TYPE_LONGLONG] = "long long",
        [TYPE_FLOAT] = "float",
        [TYPE_DOUBLE] = "double",
        [TYPE_LONGDOUBLE] = "long double",
        [TYPE_CHAR] = "char",
        [TYPE_STR] = "char*"
    };

    // Handle invalid type values gracefully
    if (type >= 0 && type < sizeof(names)/sizeof(names[0])) {
        return names[type];
    }
    return "unknown_type";
}

const char* node_type_to_str(NodeType type){
    static const char* names[] = {
        "NODE_PROGRAM",
        "NODE_DECLARATION",
        "NODE_ASSIGNMENT",
        "NODE_IDENTIFIER",
        "NODE_INTEGER",
        "NODE_FLOAT",
        "NODE_STRING",
        "NODE_CHAR",
        "NODE_BINARY_OP",
        "NODE_UNARY_OP",
        "NODE_IF",
        "NODE_WHILE",
        "NODE_FOR",
        "NODE_BREAK",
        "NODE_RETURN",
        "NODE_COMPOUND",
        "NODE_FUNCTION",
        "NODE_CALL", 
        "NODE_CONTINUE"
    };

    if(type >= 0 && type < sizeof(names)/sizeof(names[0])){
        return names[type];
    }
    return "NODE_INVALID";
}
const char* operator_to_string(OperatorType op) {
    static const char* names[] = {
        [OP_ADD] = "+", 
        [OP_SUB] = "-", 
        [OP_MUL] = "*", 
        [OP_DIV] = "/",
        [OP_MOD] = "%",
        [OP_ASSIGN] = "=", 
        [OP_EQ] = "==", 
        [OP_NE] = "!=",
        [OP_LT] = "<", 
        [OP_LE] = "<=",
        [OP_GT] = ">", 
        [OP_GE] = ">=",
        [OP_AND] = "&&",
        [OP_OR] = "||",
        [OP_NOT] = "!",
        [OP_NEG] = "- (unary)",
        [OP_PRE_INC] = "++ (prefix)",
        [OP_PRE_DEC] = "-- (prefix)",
        [OP_POST_INC] = "++ (postfix)",
        [OP_POST_DEC] = "-- (postfix)",
        [OP_PE] = "+=",
        [OP_ME] = "-=",
        [OP_MULE] = "*=",
        [OP_DIVE] = "/=",
        [OP_MODE] = "%="
    };
    return (op >= 0 && op < sizeof(names)/sizeof(names[0])) ? names[op] : "?";
}

void ast_print(ASTNode *node, int indent) {
    // Safety check for infinite recursion
    if (indent > 20) {
        printf("%*sMAX DEPTH REACHED\n", indent*4, "");
        return;
    }

    if (!node) {
        printf("%*s(null)\n", indent*4, "");
        return;
    }

    // Print node header
    printf("%*s%s @%p (line %d)", indent*4, "", 
           node_type_to_str(node->type), node, node->line_number);

    // Print node-specific details
    switch (node->type) {
        case NODE_IDENTIFIER:
            printf(" '%s'", node->string_value);
            break;
        case NODE_INTEGER:
            printf(" %lld", node->int_value);
            break;
        case NODE_FLOAT:
            printf(" %f", node->float_value);
            break;
        case NODE_STRING:
            printf(" \"%s\"", node->string_value);
            break;
        case NODE_CHAR:
            printf(" '%c'", node->char_value);
            break;
        case NODE_DECLARATION:
            printf(" %s: type=%d, const=%d", 
                   node->var_name, node->var_type, node->is_constant);
            break;
        case NODE_BINARY_OP:
            printf(" %s", operator_to_string(node->op));
            break;
        case NODE_UNARY_OP:
            printf(" %s", operator_to_string(node->op));
            break;
        case NODE_COMPOUND:
            printf(" [%zu statements]", node->children_count);
            break;
        case NODE_IF:
            printf(" if");
            break;
        case NODE_WHILE:
            printf(" while");
            break;
        case NODE_FOR:
            printf(" for");
            break;
        case NODE_RETURN:
            printf(" return");
            break;
        case NODE_PROGRAM:
            printf(" program");
            break;
        case NODE_CONTINUE:
            printf(" continue");
            break;
        case NODE_BREAK:
            printf(" break");
            break;
        default:
            printf(" [unknown node type %d]", node->type);
            break;
    }
    printf("\n");

    // Print children with clear relationship indicators
    switch (node->type) {
        case NODE_BINARY_OP:
            printf("%*sleft:\n", (indent+1)*4, "");
            ast_print(node->left, indent+2);
            printf("%*sright:\n", (indent+1)*4, "");
            ast_print(node->right, indent+2);
            break;

        case NODE_UNARY_OP:
            printf("%*soperand:\n", (indent+1)*4, "");
            ast_print(node->left, indent+2);
            break;

        case NODE_IF:
            printf("%*scondition:\n", (indent+1)*4, "");
            ast_print(node->cond, indent+2);
            printf("%*sthen:\n", (indent+1)*4, "");
            ast_print(node->then_part, indent+2);
            if (node->else_part) {
                printf("%*selse:\n", (indent+1)*4, "");
                ast_print(node->else_part, indent+2);
            }
            break;

        case NODE_WHILE:
            printf("%*scondition:\n", (indent+1)*4, "");
            ast_print(node->cond, indent+2);
            printf("%*sbody:\n", (indent+1)*4, "");
            ast_print(node->then_part, indent+2);
            break;

        case NODE_FOR:
            printf("%*sinit:\n", (indent+1)*4, "");
            ast_print(node->init, indent+2);
            printf("%*scondition:\n", (indent+1)*4, "");
            ast_print(node->cond, indent+2);
            printf("%*sstep:\n", (indent+1)*4, "");
            ast_print(node->step, indent+2);
            printf("%*sbody:\n", (indent+1)*4, "");
            ast_print(node->then_part, indent+2);
            break;

        case NODE_RETURN:
            if (node->left) {
                printf("%*sexpression:\n", (indent+1)*4, "");
                ast_print(node->left, indent+2);
            }
            break;

        case NODE_COMPOUND:
            for (size_t i = 0; i < node->children_count; i++) {
                printf("%*s[%zu]:\n", (indent+1)*4, "", i);
                ast_print(node->children[i], indent+2);
            }
            break;

        case NODE_DECLARATION:
            if (node->init_value) {
                printf("%*sinitializer:\n", (indent+1)*4, "");
                ast_print(node->init_value, indent+2);
            }
            break;

        case NODE_PROGRAM:
            for (size_t i = 0; i < node->children_count; i++) {
                printf("%*s[%zu]:\n", (indent+1)*4, "", i);
                ast_print(node->children[i], indent+2);
            }
            break;

        default:
            // No children to print for other node types
            break;
    }
}




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