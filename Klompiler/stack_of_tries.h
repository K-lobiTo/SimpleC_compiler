#ifndef STACK_OF_TRIES_H
#define STACK_OF_TRIES_H

#include "trie.h" 

typedef struct ScopeStack {
    TrieNode **scopes;
    int top;
    int capacity;
} ScopeStack;

// Stack operations
ScopeStack* create_scope_stack(int initial_capacity);
void push_scope(ScopeStack *stack);
void pop_scope(ScopeStack *stack);
void free_scope_stack(ScopeStack **stack);

// Symbol table operations
bool insert_in_current_scope(ScopeStack *stack, const char *symbol, const bool is_const, const int type, const int line);
bool search_in_all_scopes(const ScopeStack *stack, const char *symbol);
bool search_in_current_scope(const ScopeStack *stack, const char *symbol);




#endif