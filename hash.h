#ifndef HASH_H
#define HASH_H

typedef struct hashtable {
    unsigned int keyPos;
    int count;
    struct hashtable* next;
} HashTable;
#endif