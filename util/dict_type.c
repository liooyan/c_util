#include "dict_type.h"
#include "string.h"

dictType strObjectDictType = {
        (uint64_t (*)(const void *)) dictStrHash,            /* hash function */
        NULL,                       /* key dup */
        NULL,                       /* val dup */
        hashKeyCompare,      /* key compare */
        NULL,          /* key destructor */
        NULL        /* val destructor */
};

uint64_t dictStrHash(const void *key) {

    return murmurHash64B(key, strlen(key), 0);
}


 int hashKeyCompare(const void *key1, const void *key2) {
    return dictStrHash(key1) == dictStrHash(key2);
}


