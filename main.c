#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"hash.h"

long int fileLen(FILE*);
unsigned int strPoly(char*, unsigned int);
void insertKey(char* str, char**, int*);

int main(int argc, char *argv[]) {
    if (!setlocale(LC_ALL, "zh_TW.UTF-8")) {
        fprintf(stderr, "Error:Please check LANG, LC_CTYPE, LC_ALL.\n");
        return 1;
    }

    FILE* inFile = fopen("sents.rec", "r");
    FILE* outFile = fopen("tmp.rec", "wt");
    if(!inFile) {
        printf("No such file or could not open it.\n");
        exit(1);
    }

    unsigned long long int CHUNK_SiZE = 1ULL << 33;
    unsigned int BUFFER_SIZE = 1UL << 22;
    unsigned int MaxHash = 1UL << 22;
    char* chunk = (char*)malloc(CHUNK_SiZE);
    char* chunkHead = chunk;
    HashTable* table = (HashTable*)malloc(MaxHash * sizeof(HashTable));
    char** buffer = (char**) malloc(BUFFER_SIZE);
    unsigned int keybuflast = 0;

    int count = 0;
    unsigned int len = fileLen(inFile);

    for(int i = 0; i < len;) {
        chunk = chunkHead;
        fseek(inFile, i, SEEK_SET);
        fread(chunk, sizeof(char),  CHUNK_SiZE * sizeof(char), inFile);
        
        while(strstr(chunk, "\n") != NULL) {
            char* newline = strstr(chunk, "\n");
            *newline = '\0';
            if(chunk[0] != '\0') {
                unsigned int key = strPoly(chunk, MaxHash);
                
                if(buffer[table[key].keyPos] != NULL && strcmp(buffer[table[key].keyPos], chunk) == 0) {
                    table[key].count++;
                    printf("String: %s, Count: %d\n", buffer[table[key].keyPos], table[key].count);
                } else {
                    unsigned int nextKey = key;
                    insertKey(chunk, buffer, &keybuflast);
                    while(table[nextKey].count != 0) nextKey++;
                    table[nextKey].count++;
                    table[nextKey].keyPos = keybuflast;
                    if(nextKey != key) table[key].next = &table[nextKey];
                    keybuflast++;
                }
            }
            
            i += strlen(chunk) + 1;
            chunk = newline + 1;
        }
    }
}

long int fileLen(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

unsigned int strPoly(char* str, unsigned int maxhash) {
    unsigned int sum = 0;
    for(int i = 0; i < strlen(str) - 1; i++)
        sum += (unsigned int)str[i] * 33 + (unsigned int)str[i + 1];

    return sum % maxhash;
}

void insertKey(char* str, char** keybuf, int* last) {
    keybuf[*last] = strdup(str);
}