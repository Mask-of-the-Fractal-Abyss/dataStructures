#define HT_combine_(a, b) a ## _ ## b
#define HT_combine(a, b) HT_combine_(a, b)
#define HT_concatK(a) HT_combine(a, HT_KEY)
#define HT_concatV(a) HT_combine(a, HT_VAL)
#define HT_newthing(a) HT_combine(new, a)
#define HT_freething(a) HT_combine(free, a)

#define HT_pair HT_concatV(HT_concatK(pair))
#define HT_concat(a) HT_combine(a, HT_pair)
#define HT_table HT_concat(HashTable)
#define HT_node HT_concat(NodeLinkedList)
#define HT_list HT_concat(LinkedList)

struct HT_pair {
    HT_KEY key;
    HT_VAL value;
};
typedef struct HT_pair* HT_pair;

#define LL_TYPE HT_pair
#include <linkedlists.h>
#undef LL_TYPE

// http://www.cse.yorku.ca/~oz/hash.html
unsigned long HT_concatK(HT_hash)(HT_KEY value) {
    unsigned long hash = 5381;
    char* s = (char*) (&value);
    for (int i = 0; i < sizeof(HT_KEY); i++) {
        hash = ((hash << 5) + hash) + (*s++);
    }
    return hash;
}
// 1 if equal, 0 otherwise
int HT_concatK(HT_compare)(HT_KEY v1, HT_KEY v2) {
    char* s1 = (char*) (&v1);
    char* s2 = (char*) (&v2);
    for (int i = 0; i < sizeof(HT_KEY); i++) {
        if ((*s1++) != (*s2++)) return 0;
    }
    return 1;
}

struct HT_table {
    int size, entryCount;
    double max;
    HT_list* buckets;
};
typedef struct HT_table* HT_table;
HT_list* HT_newthing(HT_concat(buckets))(int size) {
    HT_list* arr = (HT_list*) malloc(sizeof(HT_list));
    for (int i = 0; i < size; i++) {
        arr[i] = HT_concat(new_LinkedList)();
    }
    return arr;
}
HT_table HT_newthing(HT_table)(int initial, double max) {
    HT_table t = (HT_table) malloc(sizeof(struct HT_table));
    t->size = initial;
    t->entryCount = 0;
    t->max = max;
    t->buckets = HT_newthing(HT_concat(buckets))(initial);
    return t;
}
HT_node HT_concat(HT_get)(HT_table t, HT_KEY key) {
    unsigned h = HT_concatK(HT_hash)(key) % t->size;
    HT_list l = t->buckets[h];
    HT_node current = l->head;
    int i = 0;
    while (i < l->size) {
        if (HT_concatK(HT_compare)(current->value->key, key)) {
            return current;
        }
        current = current->next;
        i++;
    }
    return 0;
}
HT_pair HT_concat(HT_set)(HT_table t, HT_KEY key, HT_VAL value) {
    HT_pair p = HT_concat(HT_get)(t, key)->value;
    p->value = value;
    return p;
}
HT_node HT_concat(HT_rehash)(HT_table t, HT_pair pair);
HT_node HT_concat(HT_add)(HT_table t, HT_KEY key, HT_VAL value) {
    unsigned h = HT_concatK(HT_hash)(key) % t->size;
    t->entryCount += 1;
    HT_pair pair = (HT_pair) malloc(sizeof(HT_pair));
    pair->key = key;
    pair->value = value;
    HT_node n = HT_concat(LL_append)(t->buckets[h], pair); 
    if (((float) (t->entryCount) / t->size) < t->max) {
        return n;
    }
    return HT_concat(HT_rehash)(t, pair);
}
HT_node HT_concat(HT_remove)(HT_table t, HT_KEY key) {
    unsigned h = HT_concatK(HT_hash)(key) % t->size;
    HT_list l = t->buckets[h];
    HT_node prev, current;
    current = l->head;
    int i = 0;
    while (i < l->size) {
        if (HT_concatK(HT_compare)(current->value->key, key)) {
            t->entryCount--;
            if (i == 0 || i == (l->size-1)) {
                return HT_concat(LL_remove)(l, i);
            }
            return HT_concat(LL_positionRemove)(l, prev);
        }
        prev = current;
        current = current->next;
        i++;
    }
    return 0;
}
HT_node HT_concat(HT_addPair)(HT_table t, HT_pair pair) {
    unsigned h = HT_concatK(HT_hash)(pair->key) % t->size;
    t->entryCount += 1;
    HT_node n = HT_concat(LL_append)(t->buckets[h], pair);
    if (((float) (t->entryCount) / t->size) < t->max) {
        return n;
    }
    return HT_concat(HT_rehash)(t, pair);
}
HT_node HT_concat(HT_rehash)(HT_table t, HT_pair pair) {
    int size = t->size;
    t->size *= 2;
    HT_list* prev = t->buckets;
    t->buckets = HT_newthing(HT_concat(buckets))(t->size);
    t->entryCount = 0;
    int i, j;
    HT_node current, out;
    for (i = 0; i < size; i++) {
        j = 0;
        current = prev[i]->head;
        while (j < prev[i]->size) {
            unsigned h = HT_concatK(HT_hash)(current->value->key) % t->size;
            t->entryCount += 1;
            HT_node n = HT_concat(LL_append)(t->buckets[h], current->value);
            if (HT_concatK(HT_compare)(current->value->key, pair->key)) {
                out = n;
            }
            else {
                HT_concat(HT_addPair)(t, current->value);
            }
            current = current->next;
            j++;
        }
        HT_concat(free_LinkedList)(prev[i]);
    }
    free(prev);
    if (((float) (t->entryCount) / t->size) >= t->max) {
        HT_concat(HT_rehash)(t, pair);
    }
    return out;
}

#undef HT_combine_
#undef HT_combine
#undef HT_concat
#undef HT_concatK
#undef HT_concatV
#undef HT_newthing
#undef HT_freething

#undef HT_pair
#undef HT_table
#undef HT_node
#undef HT_list