#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<string.h>
#include"hash.h"
#define MaxHash 10000

long int fileLen(FILE*);
unsigned int strPoly(char*, int);
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
    char* chunk = (char*)malloc(CHUNK_SiZE);
    char* chunkHead = chunk;
    HashTable* table = (HashTable*)malloc(MaxHash * sizeof(HashTable));
    char** buffer = (char**) malloc(BUFFER_SIZE);
    int keybuflast = 0;

    int count = 0;
    unsigned int len = fileLen(inFile);

    for(int i = 0; i < len;) {
        chunk = chunkHead;
        fseek(inFile, i, SEEK_SET);
        fread(chunk, sizeof(char),  CHUNK_SiZE * sizeof(char), inFile);
        
        while(strstr(chunk, "\n") != NULL) {
            char* newline = strstr(chunk, "\n");
            *newline = '\0';
            if(chunk[0] != '\0') insertKey(chunk, buffer, &keybuflast);
            i += strlen(chunk) + 1;
            printf("%d/%d\n", i, len);
            chunk = newline + 1;
        }
    }
}

long int fileLen(FILE* file) {
    fseek(file, 0, SEEK_END);
    return ftell(file);
}

unsigned int strPoly(char* str, int maxhash) {
    int sum = 0;
    for(int i = 0; i < strlen(str) - 1; i++)
        sum += (int)str[i] * 33 + (int)str[i + 1];

    return sum %= maxhash;
}

void insertKey(char* str, char** keybuf, int* last) {
    keybuf[*last] = strdup(str);
    (*last)++;
}