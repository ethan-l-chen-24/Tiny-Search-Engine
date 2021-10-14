# Lab 4 - Ethan Chen
# CS50 Fall 2021

### crawler

Refer to `README.md` for specifics on the specs.

### Usage

The *crawler* module implemented in `crawler.c` provides methods that can be used for the TSE in other modules.
This includes the complete `crawler()` method as well as each submethod used in the process

```c
bool crawler(char* seedURL, char* pageDir, int depth);
void processWebpages(hashtable_t* visitedURLs, bag_t* toCrawl, int* idCounter, char* pageDir, int maxDepth);
bool pageFetcher(webpage_t* page);
char* pageScanner(webpage_t* page, int* pos);
bool pageSaver(webpage_t* page, int* id, char* pageDir);
```

### Implementation

The crawler is implemented according to the pseudocode given in the lab description.

The data structures used in this implementation were a `struct bag` and `struct hashtable` as defined in `bag.h` and `hashtable.h`, respectively.

The algorithm works as so: 
* Parse the command line and validate the parameters
* make a webpage for the _seedURL_, marked with depth = 0
* add that page to the `bag` of webpages to crawl
* add that _URL_ to the `hashtable` of URLs seen
* loop through the `bag` until empty, each time extracting a webpage
* fetch the _HTML_ of that page and then save it to the document
* if the `depth` of that webpage is less than the maximum, scan through the webpage's _HTML_ for all links
* extract those links, check that they are not normalized or internal, and insert them into the `hashtable`
* make a webpage for that _URL_ at `depth` + 1 and add it to the `bag`

For more specific pseudocode on each method, refer to the comments above each method in the `crawler.c` file 