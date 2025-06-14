#include <stdio.h>
#include <stdlib.h>
#include "compiler.h"
// #include "trie.h"
#include "stack_of_tries.h"

extern FILE *yyin;
extern int yyparse();
extern int yylineno;
extern ASTNode *program_root;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file.c>\n", argv[0]);
        return 1;
    }

    // Open the input file
    FILE* input = fopen(argv[1], "r");
    if (!input) {
        perror("Error opening input file");
        return 1;
    }

    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS

    //     TrieNode *root = trie_create_node();
    
    // // Insert valid variable names
    // trie_insert(root, "myVar", 0);
    // trie_insert(root, "counter1", 1);
    // trie_insert(root, "_temp_value", 1);
    // trie_insert(root, "MAX_SIZE", 0);
    
    // // Test searches
    // printf("Search 'myVar': %s\n", trie_search(root, "myVar") ? "Found" : "Not found");
    // printf("isConst? 'myVar': %s\n", inTrieConst(trie_search_node(root, "myVar")) ? "isCOnst" : "Not const");
    // printf("Search 'unknown': %s\n", trie_search(root, "unknown") ? "Found" : "Not found");
    // printf("Search 'counter1': %s\n", trie_search(root, "counter1") ? "Found" : "Not found");
    // printf("isConst? 'counter1': %s\n", inTrieConst(trie_search_node(root, "counter1")) ? "isCOnst" : "Not const");
    // printf("Search '1counter': %s\n", trie_search(root, "1counter") ? "Found" : "Not found"); // Invalid in C
    
    // // Cleanup
    // trie_free(root);
    // return 0;

    ScopeStack *symtab = create_scope_stack(10);
    push_scope(symtab);  // Global scope
    insert_in_current_scope(symtab, "GlobalConst", 1, 0);

    // In a function:
    push_scope(symtab);  // New scope
    insert_in_current_scope(symtab, "local_var", 0, 0);
    if (!search_in_all_scopes(symtab, "undeclared_var")) {
        printf("GOOD: Undeclared variable!\n");
    }
    if (search_in_current_scope(symtab, "local_var")) {
        printf("GOOD: declared variable!\n");
    }
    if (search_in_all_scopes(symtab, "GlobalConst")) {
        printf("YES: DEclared variable!\n");
    }
    pop_scope(symtab);  // Exit function scope
    if (search_in_current_scope(symtab, "local_var")) {
        printf("BAD: declared variable local_var!\n");
    } else 
        printf("GOOD: undeclared variable local_var!\n");
    if (search_in_all_scopes(symtab, "GlobalConst")) {
        printf("YES: DEclared variable!\n");
    }
    free_scope_stack(&symtab);
    printf("after free scope\n");
    if (search_in_all_scopes(symtab, "GlobalConst")) {
        printf("YES: DEclared variable!\n");
    } else printf("YES: not found!\n");
    
    search_in_all_scopes(symtab, "GlobalConst");
    // free_scope_stack(NULL);
    return 0;
    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS
    // TRIE TESTS

    yyin = input;
    // Parse the input file
    // int error_line = 0;
    if (yyparse() != 0) {
        fprintf(stderr, "Parsing failed at line %d\n", yylineno);
        fclose(yyin);
        return 1;
    }

    printf("Parsing completed successfully!\n");
    
    // Future AST processing functions here
    
    // Cleanup
    fclose(yyin);
    free_ast(program_root);
    
    return 0;
}