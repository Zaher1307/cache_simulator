#include "simulate.h"

/*
 * Function: microOperation
 * ------------------------
 *  handels all tasks of the the validation as it match the tag of the line and if the tag is not exist it access new line or find a victim and update lines' orders of the set.
 *      
 *      set_index: the index of the targer set
 *      tag: the tag of the target line
 *      cache_ptr: a pointer to the cache
 * 
 *      return: the status of the operation, hit or miss or miss and evict
 */
static status_t microOperation (int set_index, int tag, cache_t* cache_ptr);

/*
 * Function: isTagExist
 * ---------------------
 *  Search for a line in a set.
 *      
 *      set_index: the index of the target set
 *      tag: the tag of the target line
 *      cache_ptr: a pointer to the cache
 * 
 *      return: the index of the line in the set if exists
 *              NOT_FOUND if not
 */
static int isTagExist (int set_index, int tag, cache_t* cache_ptr);

/*
 * Function: updateAccessOrder
 * ---------------------
 *  updates the order of every line in cache .
 *      
 *      set_index: the index of the target set
 *      tag: the tag of the target line
 *      cache_ptr: a pointer to the cache
 * 
 *      return: void.
 */
static void updateAccessOrder (int set_index, int usedLine, cache_t* cache_ptr);

/*
 * Function: findEmptyLine
 * -----------------------
 *  Search for an empty line in a set.
 * 
 *      set_index: the index of the targer set
 *      cache_ptr: a pointer to the cache
 * 
 *      return: the index of the empty line in the set if exists
 *              NOT_FOUND if not
 */
static int findEmptyLine (int set_index, cache_t* cache_ptr);

/*
 * Function: findVictim
 * ------------------------
 *  Search for a victem line to be evicted using LRU policy.
 *      
 *      set_index: the index of the targer set
 *      cache_ptr: a pointer to the cache
 * 
 *      return: the index of the empty line in the set
 */
static int findVictim (int set_index, cache_t* cache_ptr);


void validateCaching (int* hit, int* miss, int* evict, char operation, int address, int size, int verbose, cache_t* cache_ptr) {
    int tag = address >> ((cache_ptr->address_info.block_offset + cache_ptr->address_info.set_index));
    int set_index = (address >> cache_ptr->address_info.block_offset) & ~(-1 << cache_ptr->address_info.set_index);
    status_t status = microOperation(set_index, tag, cache_ptr);


    switch (operation)
    {
    case 'M':
        if (status == HIT) {
            (*hit) += 2;
            if (verbose) {
                printf("%c %x,%u hit hit\n", operation, address, size);
            }
        } else if (status == MISS) {
            (*miss)++;
            (*hit)++;
            if (verbose) {
                printf("%c %x,%u miss hit\n", operation, address, size);
            }
        } else {
            (*miss)++;
            (*hit)++;
            (*evict)++;
            if (verbose) {
                printf("%c %x,%u miss eviction hit\n", operation, address, size);
            }
        }
        break;

    case 'L':
    case 'S':
        if (status == HIT) {
            (*hit)++;
            if (verbose) {
                printf("%c %x,%u hit\n", operation, address, size);
            }   
        } else if (status == MISS) {
            (*miss)++;
            if (verbose) {
                printf("%c %x,%u miss\n", operation, address, size);
            }
        } else {
            (*miss)++;
            (*evict)++;
            if (verbose) {
                printf("%c %x,%u miss eviction\n", operation, address, size);
            }
        }
        break;
    
    default:
        break;
    }
}

static status_t microOperation (int set_index, int tag, cache_t* cache_ptr) {
    status_t status;
    int line = isTagExist(set_index, tag, cache_ptr);
    if (line != NOT_FOUND) {
        status = HIT;
        updateAccessOrder(set_index, line, cache_ptr);
    } else {
        if (cache_ptr->S[set_index].used_lines < cache_ptr->address_info.set_lines) {
            line = findEmptyLine(set_index, cache_ptr);
            cache_ptr->S[set_index].E[line-1].valid_bit = 1;
            cache_ptr->S[set_index].E[line-1].tag = tag;
            cache_ptr->S[set_index].E[line-1].last_time_used = cache_ptr->S[set_index].used_lines + 1;
            cache_ptr->S[set_index].used_lines++;
            status = MISS;
        } else {
            line = findVictim(set_index, cache_ptr);
            cache_ptr->S[set_index].E[line-1].tag = tag;
            updateAccessOrder(set_index, line, cache_ptr);
            status = EVICT;
        }
    }

    return status;
}

static int isTagExist (int set_index, int tag, cache_t* cache_ptr) {
    int isExist = NOT_FOUND;
    for (int i = 1; i <= cache_ptr->address_info.set_lines; ++i) {
        if (cache_ptr->S[set_index].E[i-1].tag == tag && cache_ptr->S[set_index].E[i-1].valid_bit == 1) {
            isExist = i;
            break;
        }
    }
    return isExist;
}

static void updateAccessOrder (int set_index, int usedLine, cache_t* cache_ptr) {
    int least_recent_used_order = cache_ptr->S[set_index].E[usedLine-1].last_time_used;
    for (int i = 1; i <= cache_ptr->address_info.set_lines; ++i) {
        if (usedLine == i) {
            cache_ptr->S[set_index].E[i-1].last_time_used = cache_ptr->S[set_index].used_lines;
        } else if (cache_ptr->S[set_index].E[i-1].last_time_used > least_recent_used_order) {
            cache_ptr->S[set_index].E[i-1].last_time_used--;
        }
    }
}

static int findEmptyLine (int set_index, cache_t* cache_ptr) {
    int line = NOT_FOUND;
    for (int i = 1; i <= cache_ptr->address_info.set_lines; ++i) {
        if (cache_ptr->S[set_index].E[i-1].valid_bit == 0) {
            line = i;
            break;
        }
    }
    return line;
}

static int findVictim (int set_index, cache_t* cache_ptr) {
    int line = 0;
    for (int i = 1; i <= cache_ptr->address_info.set_lines; ++i) {
        if (cache_ptr->S[set_index].E[i-1].last_time_used == 1) {
            line = i;
            break;
        }
    }
    return line;
}