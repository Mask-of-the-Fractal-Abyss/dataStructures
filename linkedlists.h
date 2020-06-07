#define LL_combine_(a, b) a ## _ ## b
#define LL_combine(a, b) LL_combine_(a, b)
#define LL_concat(a) LL_combine(a, LL_TYPE)
#define LL_newthing(a) LL_combine(new, a)
#define LL_freething(a) LL_combine(free, a)

#define LL_node LL_concat(NodeLinkedList)
#define LL_list LL_concat(LinkedList)

#define LL_checkIndicies \
if (i < 0) {i += l->size; if (i < 0) exit(1);} \
else if (i > l->size) { \
    printf("Index %d out of range [%d]", i, l->size); \
    exit(2); \
}

struct LL_node {
    struct LL_node* next;
    LL_TYPE value;
};
typedef struct LL_node* LL_node;
LL_node LL_newthing(LL_node)(LL_TYPE value) {
    LL_node n = (LL_node) malloc(sizeof(struct LL_node));
    n->value = value;
    return n;
}

struct LL_list {
    int size;
    LL_node head, tail;
};
typedef struct LL_list* LL_list;
LL_list LL_newthing(LL_list)() {
    LL_list l = (LL_list) malloc(sizeof(LL_list));
    l->size = 0;
    return l;
}
LL_TYPE LL_concat(LL_get)(LL_list l, int i) {
    LL_checkIndicies
    LL_node current = l->head;
    while (i--) {
        current = current->next;
    }
    return current->value;
}
LL_node LL_concat(LL_positionGet)(LL_list l, int i) {
    LL_checkIndicies
    LL_node current = l->head;
    while (i--) {
        current = current->next;
    }
    return current;
}
LL_node LL_concat(LL_append)(LL_list l, LL_TYPE value) {
    LL_node n = LL_newthing(LL_node)(value);
    if (l->size == 0) {
        l->head = n;
        l->tail = n;
        l->size++;
        return n;
    }
    l->tail->next = n;
    l->tail = n;
    l->size++;
    return n;
}
LL_node LL_concat(LL_prepend)(LL_list l, LL_TYPE value) {
    LL_node n = LL_newthing(LL_node)(value);
    if (l->size == 0) {
        l->head = n;
        l->tail = n;
        l->size++;
        return n;
    }
    n->next = l->head;
    l->head = n;
    l->size++;
    return n;
}
LL_node LL_concat(LL_insert)(LL_list l, LL_TYPE value, int i) {
    LL_checkIndicies
    if (i == 0) {
        l->size++;
        return LL_concat(LL_prepend)(l, value);
    }
    else if (i == (l->size-1)) {
        l->size++;
        return LL_concat(LL_append)(l, value);
    }
    LL_node before = LL_concat(LL_positionGet)(l, i-1);
    LL_node after = before->next;
    LL_node n = LL_newthing(LL_node)(value);
    before->next = n;
    n->next = after;
    l->size++;
    return n;
}
LL_node LL_concat(LL_positionInsert)(LL_list l, LL_TYPE value, LL_node pos) {
    LL_node n = LL_newthing(LL_node)(value);
    n->next = pos->next->next;
    pos->next = n;
    return n;
}
LL_node LL_concat(LL_remove)(LL_list l, int i) {
    LL_checkIndicies
    if (l->size == 1) {
        l->size = 0;
        return l->head;
    }
    if (i == 0) {
        LL_node h = l->head;
        l->head = h->next;
        l->size--;
        return h;
    }
    else if (i == (l->size-1)) {
        LL_node prev = LL_concat(LL_positionGet)(l, i-1);
        l->tail = prev;
        l->size--;
        return prev->next;
    }
    LL_node prev = LL_concat(LL_positionGet)(l, i-1);
    LL_node n = prev->next;
    prev->next = n->next;
    l->size--;
    return n;
}
// cannot positional remove from the first or last element
LL_node LL_concat(LL_positionRemove)(LL_list l, LL_node pos) {
    LL_node n = pos->next;
    pos->next = n->next;
    l->size--;
    return n;
}
LL_list LL_concat(LL_map)(LL_list l, LL_TYPE (*func)(LL_TYPE)) {
    LL_node current = l->head;
    LL_list new_l = LL_newthing(LL_list)();
    int i = l->size;
    while (i--) {
        LL_concat(LL_append)(new_l, (*func)(current->value));
        current = current->next;
    }
    return new_l;
}
LL_TYPE* LL_concat(LL_toArr)(LL_list l) {
    LL_TYPE* arr = (LL_TYPE*) malloc(sizeof(LL_TYPE) * (l->size));
    LL_node current = l->head;
    int i = 0;
    while (i < l->size) {
        arr[i] = current->value;
        current = current->next;
        i++;
    }
    return arr;
}
LL_node* LL_concat(LL_toPositionArr)(LL_list l) {
    LL_node* arr = (LL_node*) malloc(sizeof(LL_node) * (l->size));
    LL_node current = l->head;
    int i = 0;
    while (i < l->size) {
        arr[i] = current;
        current = current->next;
        i++;
    }
    return arr;
}
LL_list LL_concat(LL_fromArr)(LL_TYPE* arr, int size) {
    LL_list l = LL_newthing(LL_list)();
    for (int i = 0; i < size; i++) {
        LL_concat(LL_append)(l, arr[i]);
    }
    return l;
}
void LL_freething(LL_list)(LL_list l) {
    LL_node* arr = LL_concat(LL_toPositionArr)(l);
    int i = l->size;
    while (i--) {
        free(arr[i]);
    }
    free(arr);
}

#undef LL_combine_
#undef LL_combine
#undef LL_concat
#undef LL_newthing
#undef LL_freething

#undef LL_node
#undef LL_list

#undef LL_checkIndicies