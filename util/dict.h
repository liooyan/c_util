#ifndef DICT
#define DICT


#include <stdint.h>
#include "stdlib.h"
#define DICT_HT_INITIAL_SIZE     4
#define DICT_OK 0
#define DICT_ERR 1
#define DICT_BOOL int

/**
 * 字典结构的以一个元素，通过next 构成链表
 */
typedef struct dictEntry {
    void *key;
    union {
        void *val; //val的索引
        uint64_t u64;
        int64_t s64;
        double d;
    } v;
    struct dictEntry *next; //链表下一个元素索引
} dictEntry;

/**
 * 字段类型，定义字段的一些方法，后续通过传入不同的值，进行不同的实现。
 * hashFunction hash计算的函数
 * keyDestructor key销毁函数
 * valDestructor value销毁函数
 *
 */
typedef struct dictType {
    uint64_t (*hashFunction)(const void *key);
    void *(*keyDup)( const void *key);
    void *(*valDup)( const void *obj); //字典val设置时，调用的函数
    int (*keyCompare)(const void *key1, const void *key2); //比较两个值是否相等
    void (*keyDestructor)(void *key);
    void (*valDestructor)(void *obj);
} dictType;

/**
 * 字段表结构。
 * 其中table 是一个dictEntry 的数组，而dictEntry自己本身就是一个链表。
 * 每个dict 都有两个dictht， 用于字典表的刷新。
 */
typedef struct dictht {
    dictEntry **table; //dictEntry 数组的指针
    unsigned long size; //记录当前table的长度
    unsigned long sizemask;
    unsigned long used;
} dictht;

/**
 * 字段表整体结构。
 */
typedef struct dict {
    dictType *type;
    dictht ht[2]; //两个dictht，用于字典表的扩容
    long rehashidx; //判断当前字典表是否正在扩容， -1 表示正在扩容
} dict;


dict *dictCreate(dictType *type);

int dictAdd(dict *ht, void *key, void *val);


dictEntry *dictFind(dict *d, const void *key);

/**
 * 删除指定key并释放空间
 * @param d
 * @param key
 */
void dictDelete(dict *d, const void *key);

/**
 * 删除指定key根据nofree 判断是否需要是否空间
 * @param d
 * @param key
 */
dictEntry *dictGenericDelete(dict *d, const void *key, int nofree);

#endif