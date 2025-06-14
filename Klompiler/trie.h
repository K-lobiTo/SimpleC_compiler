//trie.h
#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>

#define TRIE_CHAR_SET_SIZE (26*2 + 10 + 1) // A-Z, a-z, 0-9, _

typedef struct TrieNode {
    struct TrieNode *children[TRIE_CHAR_SET_SIZE];
    bool is_end_of_word;
    bool is_constant;
    bool is_global;
} TrieNode;

// Function prototypes
TrieNode* trie_create_node(void);
TrieNode* trie_search_node(TrieNode *root, const char *key);
bool inTrie(TrieNode *node);
bool inTrieConst(TrieNode *node);
void trie_insert(TrieNode *root, const char *key, bool is_cons);
bool trie_search(TrieNode *root, const char *key);
void trie_free(TrieNode *root);
int char_to_index(char c);

#endif