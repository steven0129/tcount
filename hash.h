#ifndef HASH_H
#define HASH_H

typedef struct hashtable {
    unsigned int keyPos;
    int count;
    unsigned int next;
} HashTable;

typedef struct keybuf {
    unsigned int key;
    char* data;
} KeyBuf;

#endif