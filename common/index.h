/* 
 * index.h - header file for CS50 'index' file in 'common' module
 *
 * provides a set of functions that can index a file, save it to a file, or load 
 * from an index file into a hashtable. It is worth noting that an "index" struct is
 * a hashtable where the key is a word, and the value is a counterset from ids to counts
 * 
 * Ethan Chen, October 2021
 */

#ifndef __INDEX
#define __INDEX

#include <stdbool.h>
#include <stdio.h>
#include "webpage.h"
#include "hashtable.h"

/**************** global types ****************/
typedef struct index index_t; // holds the hashtable used for indexing

/******************* functions *******************/

/******************* newIndex() ******************/
/*
 * Function used to create a new index struct
 * It creates a hashtable with the given size
*/
index_t* newIndex(const int tableSize);

/******************* deleteIndex() ******************/
/* deletes an index struct */
void deleteIndex(index_t* index);

/******************* saveIndexToFile() ********************/
/* Function used to save an index hashtable to a file in a given directory
 *
 *  Pseudocode:
 *      1. open the file to write to it
 *      2. Iterate through the items in the hashtable
 *      3. Print the word followed by the counters key and item using CTprint
*/
bool saveIndexToFile(char* filename, index_t* index);

/************** buildIndexFromCrawler() ******************/
/* the "testing" function/main function, which takes two arguments 
 * as inputs (other than the executable call), the directory containing the
 * files to index and the name of the index file to write
 * 
 * Pseudocode:
 *      1. Given the directory, load the webpage
 *      2. Open the file if possible and index the data
 *      3. Continue until the webpage no longer exists
*/
bool buildIndexFromCrawler(char* pageDir, index_t* index);

/******************* loadIndexFromFile() ********************/
/* Function used to read an index file and load the data
 * into a hashtable
 *
 * Pseudocode:
 *      1. create a new index hashtable
 *      2. build the filepath
 *      3. read the first word of each line and add it to the index
 *      4. scan the file for pairs of ints and add it to the word's counter
 *      5. repeat until the file has been completely read
*/
index_t* loadIndexFromFile(char* filepath);

/******************* indexWebpage() ********************/
/* Takes a webpage and loads its words into the hashtable
 *
 * Pseudocode:
 *      1. read the words in the file if possible and load the index
 *      2. delete the webpage
*/
bool indexWebpage(index_t* index, webpage_t* webpage, int* id);

/******************* getHashtable() ********************/
/* return the index's hashtable */
hashtable_t* getHashtable(index_t* index);

#endif