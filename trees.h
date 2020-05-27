#define Tr_combine_(a, b) a ## _ ## b
#define Tr_combine(a, b) Tr_combine_(a, b)
#define Tr_concat(a) Tr_combine(a, Tr_TYPE)

#define Tr_leaf Tr_combine(LeafBaseTree, Tr_TYPE)
typedef struct Tr_leaf* Tr_leaf;

#define LL_TYPE Tr_leaf
#include "linkedlists.h"
#undef LL_TYPE

#define Tr_newthing(a) Tr_combine(new, a)

#define Tr_list Tr_combine(LinkedList, Tr_leaf)
#define Tr_lnode Tr_combine(NodeLinkedList, Tr_leaf)

#define Tr_tree Tr_combine(BaseTree, Tr_TYPE)

struct Tr_leaf {
    Tr_TYPE value;
    Tr_list children;
};
typedef struct Tr_leaf* Tr_leaf;
Tr_leaf Tr_newthing(Tr_leaf)(Tr_TYPE value) {
    Tr_leaf l = (Tr_leaf) malloc(sizeof(Tr_leaf));
    l->value = value;
    l->children = Tr_concat(new_LinkedList_LeafBaseTree)();
    return l;
}

struct Tr_tree {
    Tr_leaf root;
    int (*comparison)(Tr_TYPE, Tr_TYPE);
};
typedef struct Tr_tree* Tr_tree;
Tr_tree Tr_newthing(Tr_tree)(int (*comparison)(Tr_TYPE, Tr_TYPE)) {
    Tr_tree tree = (Tr_tree) malloc(sizeof(Tr_tree));
    tree->comparison = comparison;
    return tree;
}

// return 0 if element isn't in the tree
Tr_leaf Tr_concat(Tr_get)(Tr_tree tree, Tr_TYPE value) {
    if ((*(tree->comparison))(value, tree->root->value) == 0) {return tree->root;}
    Tr_leaf current = tree->root;
    int i, comp;
    Tr_lnode cchild, prev;
    while (current->children->size) {
        prev = cchild = current->children->head;
        i = current->children->size;
        while (i--) {
            comp = (*(tree->comparison))(value, cchild->value->value);
            if (comp == 0) {
                return cchild->value;
            }
            else if (comp > 0) {
                current = prev->value;
                break;
            }
            prev = cchild;
            cchild = cchild->next;
        }
        if (i == -1) {current = prev->value;}
    }
    return 0;
}

// return 0 if value already exists
Tr_leaf Tr_concat(Tr_append)(Tr_tree tree, Tr_TYPE value) {
    if ((*(tree->comparison))(value, tree->root->value) == 0) {return tree->root;}
    Tr_leaf current = tree->root;
    int i, comp;
    Tr_lnode cchild, prev;
    while (current->children->size) {
        prev = cchild = current->children->head;
        i = current->children->size;
        while (i--) {
            comp = (*(tree->comparison))(value, cchild->value->value);
            if (comp == 0) {
                return 0;
            }
            else if (comp > 0) {
                if (i == (current->children->size-1)) {
                    return Tr_concat(LL_prepend_LeafBaseTree)(current->children, Tr_newthing(Tr_leaf)(value))->value;
                }
                current = prev->value;
                break;
            }
            prev = cchild;
            cchild = cchild->next;
        }
        if (i == -1) {
            return Tr_concat(LL_append_LeafBaseTree)(current->children, Tr_newthing(Tr_leaf)(value))->value;
        }
    }
    return Tr_concat(LL_append_LeafBaseTree)(current->children, Tr_newthing(Tr_leaf)(value))->value;
}

#undef Tr_combine_
#undef Tr_combine
#undef Tr_concat

#undef Tr_leaf
#undef Tr_tree
#undef Tr_list
#undef Tr_TYPE
#undef Tr_lnode
#undef Tr_newthing
#undef Tr_tree

#undef Tr_checkIndicies