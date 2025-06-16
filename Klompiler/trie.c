#include <assert.h>
#include "trie.h"
#include "ast.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

TrieNode* trie_create_node(void) {
    TrieNode *node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node) {
        node->is_end_of_word = false;
        for (int i = 0; i < TRIE_CHAR_SET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

int char_to_index(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 'a'; // 0-25
    }
    else if (c >= 'A' && c <= 'Z') {
        return 26 + (c - 'A'); // 26-51
    }
    else if (c >= '0' && c <= '9') {
        return 52 + (c - '0'); // 52-61
    }
    else if (c == '_') {
        return 62; // 62
    }
    return -1; // Invalid character
}

void trie_insert(TrieNode *root, const char *key, bool is_cons, const int type, const int line) {
    TrieNode *current = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = char_to_index(key[i]);
        if (index == -1) return; // Skip invalid characters
        
        if (!current->children[index]) {
            current->children[index] = trie_create_node();
            if (!current->children[index]) return; // this shouldn't happen but just in case
        }
        current = current->children[index];
    }
    current->is_end_of_word = true;
    current->is_constant = is_cons;
    current->type = type;
    current->line = line;
}



// Search for a key (return the Node)
TrieNode* trie_search_node(TrieNode *root, const char *key){
    TrieNode *current = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = char_to_index(key[i]);
        if (index == -1) return false;
        
        if (!current->children[index]) {
            return false;
        }
        current = current->children[index];
    }
    return current;

}

bool trie_search(TrieNode *root, const char *key) {
    TrieNode *current = trie_search_node(root, key);
    return inTrie(current);
}

bool inTrie(TrieNode *node){
    return (node != NULL && node->is_end_of_word);
}
bool inTrieConst(TrieNode *node){
    return (node != NULL && node->is_end_of_word && node->is_constant);
}


void trie_free(TrieNode *root) {
    if (!root) return;
    assert(root);
    
    for (int i = 0; i < TRIE_CHAR_SET_SIZE; i++) {
        if (root->children[i]) {
            trie_free(root->children[i]);
        }
    }
    free(root);
}

static void print_trie_helper(TrieNode *node, char *buffer, int depth) {
    if (node == NULL){
        printf("it was null\n");
        return;
    }

    if (node->is_end_of_word) {
        buffer[depth] = '\0';
        printf("%-15s line:%-5d type:%-12s %-9s\n", 
        buffer, 
        node->line, 
        var_type_to_str(node->type),
        node->is_constant ? "constant" : "variable");
    }

    for (int i = 0; i < TRIE_CHAR_SET_SIZE; i++) {
        if (node->children[i]) {
            char c;
            if (i < 26) c = 'a' + i;               // a-z
            else if (i < 52) c = 'A' + (i - 26);   // A-Z
            else if (i < 62) c = '0' + (i - 52);   // 0-9
            else c = '_';                          // underscore

            buffer[depth] = c;
            print_trie_helper(node->children[i], buffer, depth + 1);
        }
    }
}

void print_trie(TrieNode *root) {
    if (root == NULL) {
        printf("(Empty scope)\n\n");
        return;
    }

    printf("---------------- Scope Contents ----------------\n");
    char buffer[256];
    print_trie_helper(root, buffer, 0);
    printf("------------------------------------------------\n\n");
}