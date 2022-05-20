#include <stdio.h>
#include "dict.h"
//模仿 redis 的 字典实现的一个map结构
#define dictIsRehashing(d) ((d)->rehashidx != -1)

static int init_dict(dict *di, dictType *type);

static void init_dictht(dictht *ht);

static void setVal(dict *d, dictEntry *entry, void *val);

static uint64_t dictHashKey(dict *d, void *key);

static DICT_BOOL dictCompareKeys(dict *d, const void *key1, void *key2);

static uint64_t dictKeyIndex(dict *d, const void *key, uint64_t hash, dictEntry **existing);

static void setKey(dict *d, dictEntry *entry, void *key);

dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing);

static void freeVal(dict *d, void *val);

static int expandIfNeeded(dict *d);

static int rehashStep(dict *d);

dict *dictCreate(dictType *type) {
    dict *di = (dict *) malloc(sizeof(dict));
    init_dict(di, type);
    return di;

}

DICT_BOOL dictAdd(dict *ht, void *key, void *val) {
    //将数据刷新至新的k空间中
    if (dictIsRehashing(ht)) rehashStep(ht);


    //判断当前节点是否需要扩容
    if (DICT_ERR == expandIfNeeded(ht)) {
        return DICT_ERR;
    }
    dictEntry *existing;
    dictEntry *entry = dictAddRaw(ht, key, &existing);
    if (entry) {
        setVal(ht, entry, val);
        return DICT_OK;
    }

    //entry为空说明 已经存在节点了，需要释放旧节点的内存
    freeVal(ht, existing->v.val);
    setVal(ht, existing, val);
    return DICT_ERR;
}

static int init_dict(dict *di, dictType *type) {
    di->type = type;
    di->rehashidx = -1;
    init_dictht(&di->ht[0]);
    init_dictht(&di->ht[1]);

    return DICT_OK;
}

static void init_dictht(dictht *ht) {
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}


/**
 * 根据key 插入节点，并返回dictEntry,
 * @param d 
 * @param key 
 * @param existing 
 * @return 
 */
dictEntry *dictAddRaw(dict *d, void *key, dictEntry **existing) {

    uint64_t idx = dictKeyIndex(d, key, dictHashKey(d, key), existing);
    //-1 说明key已经存在，并且赋值给了existing
    if (idx == -1) {
        return NULL;
    }
    //正在刷新就添加在第二个里面，否则在第一个添加
    dictht *ht = dictIsRehashing(d) ? &d->ht[1] : &d->ht[0];
    dictEntry *entry = malloc(sizeof(dictEntry));

    //在链表头部添加
    entry->next = ht->table[idx];
    ht->table[idx] = entry;
    ht->used++;
    //为当前节点设置key
    setKey(d, entry, key);
    return entry;

}


/**
 * 查询当前key 所在的 table数组的位置,如果 已经存在key则将值赋给existing
 * @param d
 * @param key
 * @param hash
 * @param existing
 * @return
 */
static uint64_t dictKeyIndex(dict *d, const void *key, uint64_t hash, dictEntry **existing) {
    uint64_t idx;
    // 查询到应该插入的数组索引
    for (int i = 0; i < 2; ++i) {
        idx = hash & d->ht[i].sizemask;

        //遍历 链表，查找当前key是否已经存在
        dictEntry *he = d->ht[i].table[idx];
        while (he) {
            if (key == he->key || dictCompareKeys(d, key, he->key)) {
                if (existing) *existing = he;
                return -1;
            }
            he = he->next;
        }


        if (!dictIsRehashing(d)) break;
    }

    return idx;
}

/**
 * 为当前节点设置属性值，判断type中是否包含valDup 函数，如果有则调用valDup 生成具体的值，否则 之间设置值
 * @param d 字典结构
 * @param entry 当前数据保存的节点节点
 * @param val 数据值
 */
static void setVal(dict *d, dictEntry *entry, void *val) {

    if (d->type->valDup) {
        entry->v.val = d->type->valDup(val);
    } else {
        entry->v.val = val;
    }

}

/**
 * 是否旧节点内存
 * @param d
 * @param val
 */
static void freeVal(dict *d, void *val) {
    if (d->type->valDestructor) {
        d->type->valDestructor(val);
    }
}

/**
 * 为当前节点设置key。
 * @param d
 * @param entry
 * @param key
 */
static void setKey(dict *d, dictEntry *entry, void *key) {

    if (d->type->keyDup) {
        entry->key = d->type->keyDup(key);
    } else {
        entry->key = key;
    }

}

static void freeKey(dict *d, void *val) {
    if (d->type->keyDestructor) {
        d->type->keyDestructor(val);
    }
}

/**
 * 计算hash值,根据type中的hashFunction 函数进行计算
 * @param d 字典结构
 * @param key key
 * @return
 */
static uint64_t dictHashKey(dict *d, void *key) {
    return d->type->hashFunction(key);
}


static DICT_BOOL dictCompareKeys(dict *d, const void *key1, void *key2) {
    return d->type->keyCompare ? d->type->keyCompare(key1, key2) : key1 == key2;
}

static int dictExpand(dict *d, unsigned long size) {
    dictht ht;
    ht.size = size;
    ht.sizemask = size - 1;
    dictEntry **entry = (dictEntry **) calloc(size, sizeof(*entry));
    ht.table = entry;
    ht.used = 0;

    if (d->ht[0].table == NULL) {
        d->ht[0] = ht;
        return DICT_OK;
    }

    d->ht[1] = ht;
    d->rehashidx = 0;
    return DICT_OK;
}

static int expandIfNeeded(dict *d) {

    //正在刷新的情况直接返回
    if (dictIsRehashing(d)) return DICT_OK;

    //当ht为0时，说明需要创建数组，则创建长度为4的数组
    if (d->ht[0].size == 0) return dictExpand(d, DICT_HT_INITIAL_SIZE);

    if (d->ht[0].used >= d->ht[0].size) {
        return dictExpand(d, d->ht[0].used * 2);
    }

    return DICT_OK;
}

static void dictReset(dictht *ht) {
    ht->table = NULL;
    ht->size = 0;
    ht->sizemask = 0;
    ht->used = 0;
}

/**
 * 将一批元素刷新至新的空间中
 * @param d
 * @return
 */
static int rehashStep(dict *d) {
    int n = 1;
    int empty_num = 10 * n;
    dictht *ht = &d->ht[0];
    while (n-- && d->ht->used != 0) {
        dictEntry *de = ht->table[d->rehashidx];
        while (de == NULL) {
            d->rehashidx++;
            de = ht->table[d->rehashidx];
            if (--empty_num == 0) return 1;
        }
        while (de) {
            dictEntry *nextde = de->next;
            uint64_t h = dictHashKey(d, de->key) & d->ht[1].sizemask;
            de->next = d->ht[1].table[h];
            d->ht[1].table[h] = de;
            d->ht[0].used--;
            d->ht[1].used++;
            de = nextde;
        }
        d->ht[0].table[d->rehashidx] = NULL;
        d->rehashidx++;
    }

    if (d->ht[0].used == 0) {
        free(d->ht[0].table);
        d->rehashidx = -1;
        d->ht[0] = d->ht[1];
        dictReset(&d->ht[1]);
        return 0;

    }

    return 1;
}

dictEntry *dictFind(dict *d, const void *key) {
    dictEntry *he;
    uint64_t h, idx, table;
    //判断是否有数据
    //对于正在刷新数据执行刷新操作
    if (dictIsRehashing(d)) rehashStep(d);
    //计算当前key的下标
    h = dictHashKey(d, key);
    //在两个字典中都获取数据
    for (table = 0; table <= 1; table++) {
        idx = h & d->ht[table].sizemask;
        he = d->ht[table].table[idx];
        while (he) {
            if (key == he->key || dictCompareKeys(d, key, he->key))
                return he;
            he = he->next;
        }
        if (!dictIsRehashing(d)) return NULL;
    }
    return NULL;
}

void dictDelete(dict *d, const void *key) {
    dictGenericDelete(d, key, DICT_OK);
}

dictEntry *dictGenericDelete(dict *d, const void *key, int nofree) {
    uint64_t h = dictHashKey(d, key);
    for (int table = 0; table <= 1; table++) {
        uint64_t idx = h & d->ht[table].sizemask;
        dictEntry *he = d->ht[table].table[idx];
        //记录链表上一个值;
        dictEntry *beforeHe = NULL;
        while (he) {
            if (key == he->key || dictCompareKeys(d, key, he->key)) {
                //说明找到的节点不是链表的第一个
                if (beforeHe) {
                    beforeHe->next = he->next;
                } else {
                    d->ht[table].table[idx] = NULL;
                }
                //释放空间
                if (nofree) {
                    freeVal(d, he);
                    freeKey(d, he);
                    free(he);
                }

                d->ht[table].used--;
                return he;
            }
            beforeHe = he;
            he = he->next;
        }
        if (!dictIsRehashing(d)) break;
    }
    return NULL;


}