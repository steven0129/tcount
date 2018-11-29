#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"hash.h"
#define CHUNK_SiZE 3000

long int fileLen(FILE*);
unsigned int strPoly(char*);
int searchKey(KeyBuf*, int, int);

int main(int argc, char *argv[]) {
    if (!setlocale(LC_ALL, "zh_TW.UTF-8")) {
        fprintf(stderr, "Error:Please check LANG, LC_CTYPE, LC_ALL.\n");
        return 1;
    }

    FILE *inFile, *keyFile, *dataFile;
    keyFile = fopen("key.tmp", "w");
    fclose(keyFile);
    dataFile = fopen("data.tmp", "w");
    fclose(dataFile);

    if(argc > 1)
        inFile = fopen(argv[argc - 1], "r");
    else
        inFile = stdin;
    
    if(!inFile) {
        printf("No such file or could not open it.\n");
        exit(1);
    }
    
    unsigned int HashIdx = 0, KeyIdx = 0;
    unsigned int BUFFER_SIZE = 1 << 29;
    unsigned int MaxHash = 1 << 29;
    char* chunk = (char*)malloc(CHUNK_SiZE);
    HashTable* table = (HashTable*)malloc(MaxHash * sizeof(HashTable));
    KeyBuf* buffer = (KeyBuf*)malloc(BUFFER_SIZE * sizeof(KeyBuf));

    while(fgets(chunk, CHUNK_SiZE, inFile) != NULL) {
        unsigned int key = strPoly(chunk);

        int searchIdx = searchKey(buffer, key, KeyIdx);
        if(searchIdx) {
            if(strcmp(buffer[searchIdx].data, chunk) == 0) {
                table[searchIdx].count++;
            } else {
                table[searchIdx].next = KeyIdx;
                buffer[KeyIdx].key = key;
                buffer[KeyIdx].data = (char*)malloc(CHUNK_SiZE);
                strcpy(buffer[KeyIdx].data, chunk);
                table[KeyIdx].keyPos = KeyIdx;
                table[KeyIdx].count = 1;
                KeyIdx++;
            }
        } else {
            buffer[KeyIdx].key = key;
            buffer[KeyIdx].data = (char*)malloc(CHUNK_SiZE);
            strcpy(buffer[KeyIdx].data, chunk);
            table[KeyIdx].keyPos = KeyIdx;
            table[KeyIdx].count = 1;
            KeyIdx++;
        }
    }

    for(int i=0; i < KeyIdx; i++) {
        printf("%d %s", table[i].count, buffer[i].data);
    }
}

long int fileLen(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

unsigned int strPoly(char* str) {
    int sum = 0;
    for(int i = 0; i < strlen(str) - 1; i++) {
        sum += (int)str[i] * 33 + (int)str[i + 1];
    }

    return (sum ^ (sum >> 31)) - (sum >> 31);
}

int searchKey(KeyBuf* buffer, int key, int size) {
    for(int i=0; i < size; i++)
        if(buffer[i].key == key) return i;
        
    return 0;
}