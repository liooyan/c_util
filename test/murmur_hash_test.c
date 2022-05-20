#include "../util/murmur_hash.h"
#include <stdio.h>

int  main(){

   char *q =  "liyancecece/0";

    uint64_t hash1 = murmurHash64B(q, 11, 3);
    uint64_t hash2 = murmurHash64B(q, 11, 3);
    uint64_t hash3 = murmurHash64B(q, 11, 3);
    uint64_t hash4 = murmurHash64B(q, 11, 3);
    uint64_t hash5 = murmurHash64B(q, 11, 3);
    uint64_t hash6 = murmurHash64B(q, 11, 3);
    printf("%llu\n",hash1);
    printf("%llu\n",hash2);
    printf("%llu\n",hash3);
    printf("%llu\n",hash4);
    printf("%llu\n",hash5);
    printf("%llu\n",hash6);
    return 0;
}