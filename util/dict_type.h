#ifndef DICTTYPE
#define DICTTYPE

#include "dict.h"
#include "murmur_hash.h"

uint64_t dictStrHash(const void *key);

int hashKeyCompare(const void *key1, const void *key2);

extern dictType strObjectDictType;



#endif