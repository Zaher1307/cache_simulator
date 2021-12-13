#include "cachelab.h"
#include "simulate.h"


int main(int argc, char** argv) {
    cache_t cache_ptr;
    FILE * file_ptr;
    int hit_counter = 0, miss_counter = 0, evict_counter = 0, address, size, verbose;
    char identifier;
    verbose = initCacheSimulator(argc, argv, &cache_ptr, &file_ptr);
    while (fscanf(file_ptr," %c %x,%d", &identifier, &address, &size) > 0) {
        if (identifier != 'I') {
            validateCaching(&hit_counter, &miss_counter, &evict_counter, identifier, address, size, verbose, &cache_ptr);
        }
    }
    deleteCache(&cache_ptr);
    fclose(file_ptr);
    printSummary(hit_counter, miss_counter, evict_counter);
    return 0;
}
