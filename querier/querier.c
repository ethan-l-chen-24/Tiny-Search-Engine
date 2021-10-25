#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "index.h"
#include "word.h"
#include "pagedir.h"
#include "file.h"
#include "counters.h"
#include "memory.h"

bool query(char* pageDirectory, char* indexFilename);
void processQuery(char* search, index_t* index, char* pageDirectory);
int countWordsInQuery(char* query);
char** parseQuery(char* query, int numWords);
void normalizeQuery(char** words, int numWords);
counters_t* getIDScores(char** words, index_t* index, char* pageDirectory);


int main(const int argc, char* argv[])
{
    char* program = argv[0];
    // check for the appropriate number of arguments
    if(argc != 3) {
        fprintf(stderr, "Usage: %s [pageDirectory] [indexFilename]\n", program);
        return 1;
    }

    // allocate memory and copy string for pageDir
    char* pageDirArg = argv[1];
    char* pageDir = count_malloc(strlen(pageDirArg) + 1);
    if (pageDir == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(pageDir, pageDirArg);

    // allocate memory and copy string for indexFilename
    char* indexFnameArg = argv[2];
    char* indexFilename = count_malloc(strlen(indexFnameArg) + 1);
    if (indexFilename == NULL) {
        fprintf(stderr, "Error: out of memory\n");
        return 1;
    } 
    strcpy(indexFilename, indexFnameArg);

    if (!pageDirValidate(pageDir)) {
        count_free(indexFilename);
        count_free(pageDir);
        fprintf(stderr, "Error: %s is an invalid crawler directory\n", pageDir);
        return false;
    }

    char* trueFilename = stringBuilder(NULL, indexFilename);
    if(fopen(trueFilename, "r") == NULL) {
        fprintf(stderr, "Error: provided filename %s is invalid\n", indexFilename);
    }

    // run the indexer
    if (query(pageDir, indexFilename)) {
        printf("SUCCESS!\n\n");
        return 0;
    } else {
        printf("FAILED\n\n");
        return 1;
    }

}

bool query(char* pageDirectory, char* indexFilename)
{
    FILE* fp = stdin;
    index_t* index = loadIndexFromFile(indexFilename);
    bool active = true;

    if(index != NULL) {

        while(active) {
            printf("Query: ");
            char* query = freadlinep(fp);
            if(query != NULL) {
                processQuery(query, index, pageDirectory);
            }
        }
    } else {
        return false;
    }
}

void processQuery(char* query, index_t* index, char* pageDirectory) 
{
    int numWords = countWordsInQuery(query);

#ifdef TEST
    printf("there are %d nums\n", numWords);
#endif

    char** words = parseQuery(query, numWords);
    normalizeQuery(words, numWords);

    if(words == NULL) {
        return;
    }

#ifdef TEST
    for(int i = 0; i < numWords; i++) {
        char* word = *words;
        words++;
        printf("word %d: %s\n", i, word);
    }
#endif

  //  counters_t* set = getIDScores(words, index, pageDirectory);
    // begin reading through files and assigning scores
}

int countWordsInQuery(char* query)
{
    int count = 0;
    bool lastSpace = true;
    for (char* i = query; *i != '\0'; i++) {
        if(isalpha(*i)) {
            if(lastSpace) count++;
            lastSpace = false;
        } else if (isspace(*i)) {
            lastSpace = true;
        }
    }

    return count;
}

char** parseQuery(char* query, int numWords)
{
    char** words = calloc(numWords, sizeof(char*));
    
    bool lastSpace = true;
    char** currWord = words;
    for (char* i = query; *i != '\0'; i++) {
        if(isalpha(*i)) {
            if(lastSpace) {
                *currWord = i;
                currWord++;
            } 
            lastSpace = false;

        } else if (isspace(*i)) {
            if(!lastSpace) *i = '\0';
            lastSpace = true;

        } else {
            fprintf(stderr, "Error: fsdjkfljdsljfl\n");
            return NULL;
        }
    }

    return words;
}

void normalizeQuery(char** words, int numWords) 
{
    if(words == NULL) return;
    for(int i = 0; i < numWords; i++) {
        char* word = *words;
        words++;
        normalizeWord(word);
    }

}

counters_t* getIDScores(char** words, index_t* index, char* pageDirectory) 
{
    counters_t* idScores = counters_new();
    for (char** p = words; *p != NULL; p++) {

    }
}