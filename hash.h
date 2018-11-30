#ifndef HASH_H
#define HASH_H

typedef struct hashtable {
    int keyPos;
    int count;
    unsigned int* next;
} HashTable;

typedef struct keybuffer {
    char* str;
} KeyBuf;

#endif