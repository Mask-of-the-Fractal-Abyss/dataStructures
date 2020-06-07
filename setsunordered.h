#define ST_combine_(a, b) a ## _ ## b
#define ST_combine(a, b) ST_combine_(a, b)
#define ST_concat(a) ST_combine(a, ST_TYPE)
#define ST_newthing(a) ST_combine(new, a)
#define ST_freething(a) ST_combine(free, a)

#define ST_set ST_concat(SetUnordered)
#define ST_pair ST_combine(ST_combine(pair, ST_TYPE), ST_bool)
#define ST_concatP(a) ST_combine(a, ST_pair)
#define ST_table ST_concatP(HashTable)
#define ST_list ST_combine(LinkedList, ST_set)
#define ST_node ST_combine(NodeLinkedList, ST_set)

typedef char ST_bool;
typedef struct ST_set* ST_set;

#define HT_KEY ST_TYPE
#define HT_VAL ST_bool
#include <hashtable.h>
#undef HT_KEY
#undef HT_VAL

#define LL_TYPE ST_set
#include <linkedlists.h>
#undef LL_TYPE

struct ST_set {
    ST_list supersets, subsets;
    ST_table elements;
};
ST_set ST_newthing(ST_set)() {
    ST_set s = (ST_set) malloc(sizeof(struct ST_set));
    s->subsets = ST_newthing(ST_list)();
    s->supersets = ST_newthing(ST_list)();
    s->elements = ST_newthing(ST_table)(8, 0.5);
    return s;
}
ST_bool ST_concat(inSet)(ST_set s, ST_TYPE elem) {
    if (ST_concatP(HT_get)(s->elements, elem)) {
        return 1;
    }
    return 0;
}
ST_TYPE ST_concat(ST_add)(ST_set s, ST_TYPE elem) {
    ST_node current;
    int i;
    ST_concatP(HT_add)(s->elements, elem, 0);
    current = s->supersets->head;
    i = 0;
    while (i < s->supersets->size) {
        ST_concat(ST_add)(current->value, elem);
        current = current->next;
        i++;
    }
    return elem;
}
ST_TYPE ST_concat(ST_remove)(ST_set s, ST_TYPE elem) {
    ST_node current;
    int i;
    if (ST_concat(inSet)(s, elem)) {
        ST_concatP(HT_remove)(s->elements, elem);
        current = s->subsets->head;
        i = 0;
        while (i < s->subsets->size) {
            ST_concat(ST_remove)(current->value, elem);
            current = current->next;
            i++;
        }
        return elem;
    }
    return 0;
}
ST_set ST_concat(addSubset)(ST_set super, ST_set sub) {
    ST_combine(LL_append, ST_set)(sub->supersets, super);
    return ST_combine(LL_append, ST_set)(super->subsets, sub)->value;
}
ST_bool ST_concat(isSubset)(ST_set super, ST_set sub) {
    ST_node current = super->subsets->head;
    int i = 0;
    while (i < super->subsets->size) {
        if (current->value == sub || ST_concat(isSubset)(current->value, sub)) {
            return 1;
        }
        current = current->next;
        i++;
    }
    return 0;
}