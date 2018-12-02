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

    FILE* inFile = fopen("titles.rec", "r");
    FILE* outFile = fopen("tmp.rec", "wt");
    if(!inFile) {
        printf("No such file or could not open it.\n");
        exit(1);
    }

    unsigned long long int CHUNK_SiZE = 1ULL << 33;
    unsigned int BUFFER_SIZE = 1UL << 30;
    unsigned int MaxHash = BUFFER_SIZE;
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
                
                while(table[key].count != 0 && strcmp(buffer[table[key].keyPos], chunk) != 0) {
                    key++;
                    if(key == MaxHash) key = 1;
                }

                if(table[key].count != 0) {
                    printf("String: %s, Key: %u, Count: %d, Last: %u\n", buffer[table[key].keyPos], key, table[key].count, keybuflast);
                    table[key].count++;
                } else {
                    insertKey(chunk, buffer, &keybuflast);
                    table[key].keyPos = keybuflast;
                    table[key].count++;
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
        sum += (unsigned int)str[i] << 5 + (unsigned int)str[i + 1];

    return sum & (maxhash - 1);
}

void insertKey(char* str, char** keybuf, int* last) {
    keybuf[*last] = strdup(str);
}