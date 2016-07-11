#include <malloc.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <sys/time.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include "rbtree.h"

static void _frist_order(rbt_tree_node_t *root, rbt_tree_node_t *node, int i) {
    if (!node) return;
    if (node->left) {
        _frist_order(root, node->left, i+1);    
    }
    if (root == node)
        printf("%ld[r,%d,%d] ", node->key, i, node->color);
    else
        printf("%ld[n,%d,%d] ", node->key, i, node->color);
    if (node->right) {
        _frist_order(root, node->right, i+1);    
    }
}

static void frist_order_tree(rbt_tree_t *tree) {
    int i = 0;
    rbt_tree_node_t *root = tree->root;
    _frist_order(root, root, i);
    printf("\n");
}

int main(int argc, char** argv) {
    rbt_tree_t *rb;

    /* create rbtree */
    if (!(rb = rbt_create())) {
        fprintf(stderr, "create rbtree object error, err=%s", strerror(errno));    
        exit(1);
    }
    
    /* insert data */
    int i, ret, num = 10;
    srand(time(NULL));
    for (i = 0; i < num; i++) {
        rbt_tree_node_t *n = malloc(sizeof(rbt_tree_node_t));
        //n->key = rand()%num;
        n->key = i;
        ret = rbt_insert(rb, n);
        if (ret) {
            printf("insert key=%ld, tree=", n->key);
            frist_order_tree(rb);
        } else {
            printf("insert failed, key=%ld\n", n->key);
        }
    }
    /* delete data */
    for (i = 0, num--; num >= 0; num--, i++) {
        //rbt_tree_node_t *n = rbt_delete(rb, i);
        rbt_tree_node_t *n = rbt_delete(rb, num);
        if (n) {
            printf("delete key=%ld, tree=", n->key);
            free(n);
            frist_order_tree(rb);
        } else {
            printf("no find key=%ld\n", n->key);
        }
    }

    /* release rbtree */
    rbt_release(rb);
    return 0;
}
