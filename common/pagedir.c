/* 
 * pagedir.c - library of directory functions
 *
 * see pagedir.h for more information.
 *
 * Ethan Chen, Oct. 2021
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "word.h"
#include "memory.h"
#include "webpage.h"
#include "file.h"

/************** validDirectory() ******************/
// see pagedir.h for description
bool validDirectory(char* directoryName) 
{
    char* end = ".crawler";
    // allocate the space and build that filepath
    char* testFile = stringBuilder(directoryName, end);

    // check if that directory exists by attempting to write to that file
    FILE *fp = fopen(testFile, "w");
    if (testFile != NULL) count_free(testFile);
    if (fp != NULL) {
        // success, write arbitrary lines in that file
        fprintf(fp, "This file is created by 'pagedir' to check if this directory exists");
        fclose(fp);
        return true;
    } else {
        // if it fails, print error and return false for use in crawler.c
        fprintf(stderr, "Error: directory %s is invalid!\n", directoryName);
        return false;
    }
}

/************** writeToDirectory() ******************/
// see pagedir.h for description
bool writeToDirectory(char* filepath, webpage_t* page, int* id) 
{
    FILE *fp = fopen(filepath, "w");
    if (fp != NULL) {
        // get the URL, depth, and HTML from the webpage
        char* pageURL = webpage_getURL(page);
        int pageDepth = webpage_getDepth(page);
        char* pageHTML = webpage_getHTML(page);

        // print the URL, depth, and HTML to the file
        if (pageURL != NULL) fprintf(fp, "%s\n", pageURL);
        fprintf(fp, "%d\n", pageDepth);
	    if (pageHTML != NULL) fprintf(fp, "%s", pageHTML);
            
        fclose(fp); // close the file

        *id += 1; // increment the id
        return true;
    } else {
        // handle errors
        fprintf(stderr, "Error: could not create file %s\n", filepath);
        return false;
    }
}

/************** pageDirValidate() ******************/
// see pagedir.h for description
bool pageDirValidate(char* pageDir)
{
    // build the filepath of the .crawler file
    char* crawlerFile = ".crawler";
    char* filepath = stringBuilder(pageDir, crawlerFile);
    // try to open the file (in read mode)
    if (filepath != NULL) {
        FILE* fp;
        if((fp = fopen(filepath, "r")) != NULL) {
            count_free(filepath);
            fclose(fp);
            return true;
        } else {
            count_free(filepath);
            return false;
        }
    } else {
        return false;
    }
}

/************** loadPageToWebpage() ******************/
// see pagedir.h for description
webpage_t* loadPageToWebpage(char* pageDir, int id) 
{
    // turn the id int into a string
    char* idString = intToString(id);
    if (idString == NULL) {
        fprintf(stderr, "Error: Out of memory");
        return NULL;
    }

    // build the filepath
    char* filepath = stringBuilder(pageDir, idString);
    count_free(idString);
    if (filepath == NULL) return NULL;

    // open the crawler file
    FILE* fp;
    if ((fp = fopen(filepath, "r")) != NULL) {
        printf("Reading file %s\n", filepath);
        count_free(filepath);
        // read the first line of the file as the URL
        char* URL = freadlinep(fp);
        fclose(fp);
        // check if it is normalized and internal
        if (!IsInternalURL(URL)) {
            fprintf(stderr, "Error: URL %s is invalid\n", URL);
            return NULL;
        }
        // build the webpage
        webpage_t* page = webpage_new(URL, 0, NULL);
        if (page == NULL) {
            fprintf(stderr, "Error: could not build webpage %s\n", URL);
            return NULL;
        }
        // fetch the pages HTML
        if (!webpage_fetch(page)) {
            fprintf(stderr, "Error: page %s cannot be fetched\n", URL);
            return NULL;
        }
        return page;
    } else {
        fprintf(stderr, "could not load webpage of URL %s\n", filepath);
        count_free(filepath);
        return NULL;
    }
}

/************** stringBuilder() ******************/
// see pagedir.h for description
char* stringBuilder(char* pageDir, char* end) 
{
    if(end == NULL) return NULL;
    char prefix[] = "../data";
    if (pageDir != NULL) {
        // count the number of characters that will be in the final filepath string
        int destSize = strlen(end) + strlen(pageDir) + strlen(prefix) + 3; // +1 for /0 character

        // allocate the space and print the filepath to that memory
        char* filename = count_malloc(destSize);
        if (filename != NULL) sprintf(filename, "%s/%s/%s", prefix, pageDir, end);
        else return NULL;

        return filename;
    } else {
        // if pageDir is null, write directly to data directory
        if (end != NULL) {
            char prefix[] = "../data";
            int destSize = strlen(end) + strlen(prefix) + 2; // +1 for /0 character
            char* filename = count_malloc(destSize);
            sprintf(filename, "%s/%s", prefix, end);
            return filename;
        } else {
            return NULL;
        }
    }
}

/************** stringBuilder2() ******************/
// see pagedir.h for description
char* stringBuilder2(char* pageDir, char* end) 
{
    if(end == NULL) return NULL;
    if (pageDir != NULL) {
        // count the number of characters that will be in the final filepath string
        int destSize = strlen(end) + strlen(pageDir) + 2; // +1 for /0 character

        // allocate the space and print the filepath to that memory
        char* filename = count_malloc(destSize);
        if (filename != NULL) sprintf(filename, "%s/%s", pageDir, end);
        else return NULL;

        return filename;
    } else {
        // if pageDir is null, write directly to data directory
        if (end != NULL) {
            int destSize = strlen(end) + 1; // +1 for /0 character
            char* filename = count_malloc(destSize);
            sprintf(filename, "%s", end);
            return filename;
        } else {
            return NULL;
        }
    }
}