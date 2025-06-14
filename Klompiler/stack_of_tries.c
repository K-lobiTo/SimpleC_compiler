#include "stack_of_tries.h"
#include <stdlib.h>
#include <assert.h>

ScopeStack* create_scope_stack(int initial_capacity) {
    ScopeStack *stack = (ScopeStack*)malloc(sizeof(ScopeStack));
    stack->scopes = (TrieNode**)malloc(initial_capacity * sizeof(TrieNode*));
    stack->top = -1;
    stack->capacity = initial_capacity;
    return stack;
}

void push_scope(ScopeStack *stack) {
    if (stack->top + 1 >= stack->capacity) {
        stack->capacity *= 2;
        stack->scopes = (TrieNode**)realloc(stack->scopes, stack->capacity * sizeof(TrieNode*));
    }
    stack->scopes[++stack->top] = trie_create_node();  // New empty scope
}

void pop_scope(ScopeStack *stack) {
    assert(stack->top >= 0);
    trie_free(stack->scopes[stack->top--]);  // Free the topmost scope
}

void free_scope_stack(ScopeStack *stack) {
    while (stack->top >= 0) {
        pop_scope(stack);
    }
    free(stack->scopes);
    free(stack);
}

bool insert_in_current_scope(ScopeStack *stack, const char *symbol, const bool is_const) {
    if (stack->top < 0) return false;
    if (trie_search(stack->scopes[stack->top], symbol)) {
        return false;  // Already exists in current scope (redeclaration error)
    }
    trie_insert(stack->scopes[stack->top], symbol, is_const);
    return true;
}

bool search_in_all_scopes(ScopeStack *stack, const char *symbol) {
    for (int i = stack->top; i >= 0; i--) {
        if (trie_search(stack->scopes[i], symbol)) {
            return true;  // Found in some scope
        }
    }
    return false;  // Not found
}

bool search_in_current_scope(ScopeStack *stack, const char *symbol) {
    return (stack->top >= 0) && trie_search(stack->scopes[stack->top], symbol);
}