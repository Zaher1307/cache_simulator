#include "init.h"


/*
 * Function: allocateCache
 * ------------------------
 * Allocate and initialize memory for cache.
 * 
 *      s: set bits
 *      E: lines per set
 *      b: block bits
 * 
 *      return: void.
 */
static void allocateCache (int s, int E, int b, cache_t* cache_ptr);

/*
 * Function: initAddress
 * ------------------------
 * it init the address info in cache struct
 * 
 *      s: set bits
 *      E: lines per set
 *      b: block bits
 * 
 *      return: void.
 */
static void initAddress (int s, int E, int b, cache_t* cache_ptr);

/*
 * Function: printHelp
 * -------------------
 *  Print the help message.
 *      return: void
 */
static void printHelp();

int initCacheSimulator (int argc, char** argv, cache_t* cache_ptr, FILE** file_ptr) {
    int opt, s, E, b, verbose = 0;

    while (EOF != (opt = getopt(argc, argv, "hvs:E:b:t:"))) {
        switch(opt) {
            case 's':
                s = atoi(optarg);
                break;
                
            case 'E':
                E = atoi(optarg);
                break;

            case 'b':
                b = atoi(optarg);
                break;

            case 't':
                *file_ptr = fopen(optarg, "r");
                break;
            
            case 'v':
                verbose = 1;
                break;

            case 'h':
                printHelp();
                break;

            default:
                exit(1);
        }
    }

    allocateCache(s, E, b, cache_ptr);
    initAddress (s, E, b, cache_ptr);    
    return verbose;
}

void deleteCache (cache_t* cache_ptr) {
    for (int i = 0; i < (1 << cache_ptr->address_info.set_index); ++i) {
        free(cache_ptr->S[i].E);
    }
    free(cache_ptr->S);
}

static void allocateCache (int s, int E, int b, cache_t* cache_ptr) {
    int sets = 1 << s;
    cache_ptr->S = (set_t*) calloc(sets, sizeof(set_t));
    for (int i = 0; i < sets; ++i) {
        cache_ptr->S[i].E = (line_t*) calloc((E), sizeof(line_t));
    }
}

static void initAddress (int s, int E, int b, cache_t* cache_ptr) {
    cache_ptr->address_info.set_index = s;
    cache_ptr->address_info.set_lines = E;
    cache_ptr->address_info.block_offset = b;
}

static void printHelp() {
    char c;
    FILE *help_msg = fopen("help-msg.txt", "r");
    while ((c = fgetc(help_msg)) != EOF) {
        printf("%c", c);
    }
    fclose(help_msg);
}
