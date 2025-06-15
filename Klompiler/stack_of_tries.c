#include "stack_of_tries.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

ScopeStack* create_scope_stack(int initial_capacity) {
    ScopeStack *stack = (ScopeStack*)malloc(sizeof(ScopeStack));
    stack->scopes = (TrieNode**)malloc(initial_capacity * sizeof(TrieNode*));
    stack->top = -1;
    stack->capacity = initial_capacity;
    return stack;
}

void push_scope(ScopeStack *stack) {
    assert(stack);
    if (stack->top + 1 >= stack->capacity) {
        stack->capacity *= 2;
        stack->scopes = (TrieNode**)realloc(stack->scopes, stack->capacity * sizeof(TrieNode*));
    }
    stack->scopes[++stack->top] = trie_create_node();  // New empty scope
}

void pop_scope(ScopeStack *stack) {
    if (!stack || stack->top < 0) return;

    // printf("\nPopping scope (depth: %d):\n", stack->top + 1);
    // print_trie(stack->scopes[stack->top]);

    trie_free(stack->scopes[stack->top--]);  // Free the topmost scope

    // printf("stack->top :%d\n",stack->top);
}

void free_scope_stack(ScopeStack **stack_ptr) {
    if(!stack_ptr || !*stack_ptr)return;
    ScopeStack *stack = *stack_ptr;
    assert(stack);
    while (stack->top >= 0) {
        pop_scope(stack);
    }
    free(stack->scopes);
    free(stack);
    *stack_ptr = NULL;
}

bool insert_in_current_scope(ScopeStack *stack, const char *symbol, const bool is_const, const int type, const int line) {
    if(!stack)return false;
    assert(stack);
    if (stack->top < 0) return false;
    if (trie_search(stack->scopes[stack->top], symbol)) {
        return false;  // Already exists in current scope (redeclaration error)
    }
    trie_insert(stack->scopes[stack->top], symbol, is_const, type, line);
    return true;
}

bool search_in_all_scopes(const ScopeStack *stack, const char *symbol) {
    if(!stack || stack->top < 0)return false;
    assert(stack);
    assert(stack->top >= 0);
    for (int i = stack->top; i >= 0; i--) {
        if (stack->scopes[i] && trie_search(stack->scopes[i], symbol)) {
            return true;  // Found in some scope
        }
    }
    return false;  // Not found
}

bool search_in_current_scope(const ScopeStack *stack, const char *symbol) {
    return (stack->top >= 0) && trie_search(stack->scopes[stack->top], symbol);
}

bool search_in_global_scope(const ScopeStack *stack, const char *symbol) {
    return (stack->top >= 0) && trie_search(stack->scopes[0], symbol);
}


