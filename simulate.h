#ifndef SIMULATE
#define SIMULATE

#include "init.h"
/*
 * Function: validateCaching
 * -------------------
 *  it is the main function of the program, it works on the input line and validate it then edit the value of the hits, misses and evictions.
 * 
 *      hit: a pointer to number of cache hits
 *      miss: a pointer to number of cache misses
 *      evict: a pointer to number of cache evictions
 *      operation: indicate the operation if it load or store or modify
 *      size: the size of the data
 *      verbose: a flag to print in verbose format or not 
 *      cache_ptr: a pointer to the cache to work on
 * 
 *      return: void
 */

void validateCaching (int* hit, int* miss, int* evict, char operation, int address, int size, int verbose, cache_t* cache_ptr);

#endif
