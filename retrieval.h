#define RT_combine_(a, b) a ## _ ## b
#define RT_combine(a, b) RT_combine_(a, b)
#define RT_concat(a) RT_combine(a, RT_TYPE)
#define RT_newthing(a) RT_combine(new, a)
#define RT_freething(a) RT_combine(free, a)

#define RT_tree RT_concat(RetrievalTree)
#define RT_list RT_combine(LinkedList, RT_tree)
#define RT_node RT_combine(NodeLinkedList, RT_tree)

typedef struct RT_tree* RT_tree;

#define LL_TYPE RT_tree
#include <linkedlists.h>
#undef LL_TYPE

typedef void(*function)(RT_params);

struct RT_tree {
    RT_TYPE value;
    function func;
    RT_list children;
};
RT_tree RT_newthing(RT_tree)(RT_TYPE value) {
    RT_tree t = (RT_tree) malloc(sizeof(struct RT_tree));
    t->value = value;
    t->func = 0;
    t->children = RT_concat(new_LinkedList_RetrievalTree)();
    return t;
}

RT_tree RT_concat(RT_append)(RT_tree t, RT_TYPE* arr, int len, function f) {
    RT_node current;
    int i, j;
    i = 0;
    while (i < len) {
        j = t->children->size;
        current = t->children->head;
        while (j) {
            if (current->value->value == arr[i]) {
                printf(">%c ", arr[i]);
                break;
            }
            current = current->next;
            j--;
        }
        if (j == 0) {
            current = 
            RT_concat(LL_append_RetrievalTree)(
                t->children,
                RT_newthing(RT_tree)(arr[i])
            );
        }
        t = current->value;
        i++;
    }
    if (t->func == 0) {
        t->func = f;
    }
    return t;
}

RT_tree RT_concat(RT_get)(RT_tree t, RT_TYPE* arr, int len) {
    RT_node current;
    int i, j;
    i = 0;
    while (i < len) {
        j = t->children->size;
        current = t->children->head;
        while (j) {
            if (current->value->value == arr[i]) {
                break;
            }
            current = current->next;
            j--;
        }
        if (j == 0) {
            return t;
        }
        t = current->value;
        i++;
    }
    return t;
}