//trie.c
#include <assert.h>
#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Create a new trie node
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

// Convert character to trie index (case-sensitive)
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

// Insert a key into the trie
void trie_insert(TrieNode *root, const char *key, bool is_cons, const int type) {
    TrieNode *current = root;
    for (int i = 0; key[i] != '\0'; i++) {
        int index = char_to_index(key[i]);
        if (index == -1) return; // Skip invalid characters
        
        if (!current->children[index]) {
            current->children[index] = trie_create_node();
            if (!current->children[index]) return; // Allocation failed
        }
        current = current->children[index];
    }
    current->is_end_of_word = true;
    current->is_constant = is_cons;
    current->type = type;
}



// Search for a key in the trie (return the Node)
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

// Search for a key in the trie
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


// Free trie memory
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