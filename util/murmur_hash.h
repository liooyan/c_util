#ifndef MURMURHASH
#define MURMURHASH

#include <stdint.h>


uint64_t murmurHash64B( const void * key, int len, unsigned int seed );

#endif