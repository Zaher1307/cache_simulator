#ifndef INIT
#define INIT

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define NOT_FOUND -1

/* represents a status flag to determine what happened in cache each memory access */ 
typedef enum status {
    MISS,
    HIT,
    EVICT
} status_t;

typedef struct address
{
    int set_index;
    int set_lines;
    int block_offset;
} address_t;

typedef struct line {
    int valid_bit;
    int tag;
    int last_time_used;
} line_t;

typedef struct set {
    line_t* E;
    int used_lines;
} set_t;

typedef struct cache {
    address_t address_info;
    set_t* S;
} cache_t;

/*
 * Function: initCacheSimulator
 * ------------------------
 * parse the options given by the user and allocate memory to the cache .
 *  
 *      argc: number of arguments passed to main (main's argc)
 *      argv: pointer to the array of arguments (main's argv)
 *      cache_ptr: a pointer to the cache to work on
 *      file_ptr: a pointer to the pointer of the trace file
 *
 *      return: 1 if -v option(verbose) is set
 *              0 if -v option(verbose) is not set
 */
int initCacheSimulator (int argc, char** argv, cache_t* cache_ptr, FILE** file_ptr);

/*
 * Function: deleteCache
 * ------------------------
 * free the memory allocated by the cache.
 *  
 *      cache_ptr: a pointer to the cache to work on
 * 
 *      return: void 
 */
void deleteCache (cache_t* cache_ptr);

#endif
