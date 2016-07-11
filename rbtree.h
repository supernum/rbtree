
#ifndef __RBTREE_H__
#define __RBTREE_H__

typedef struct rbt_tree_node {
    struct rbt_tree_node *parent;
    struct rbt_tree_node *left;
    struct rbt_tree_node *right;
    unsigned char color;
    long key;
} rbt_tree_node_t;

typedef struct rbt_tree {
    rbt_tree_node_t *root;
} rbt_tree_t;

#define rbt_red(n)         do { if (n) (n)->color = 0; } while(0)
#define rbt_black(n)       do { if (n) (n)->color = 1; } while(0)


#define rbt_is_black(n)    ((n && (n)->color))
#define rbt_is_red(n)      ((n && !((n)->color)))

static inline rbt_tree_node_t *grandparent(rbt_tree_node_t *node) {
    return node->parent->parent;   
}

static inline rbt_tree_node_t *uncle(rbt_tree_node_t *node) {
    if (node->parent == grandparent(node)->left)
        return grandparent(node)->right;
    else
        return grandparent(node)->left;
}

static inline rbt_tree_node_t *rbt_min(rbt_tree_node_t *node) {
    while (node && node->left) node = node->left;
    return node;
}

rbt_tree_t *rbt_create(void);
void rbt_release(rbt_tree_t *tree);
int rbt_insert(rbt_tree_t *tree, rbt_tree_node_t *n);
rbt_tree_node_t *rbt_delete(rbt_tree_t *tree, long key);
rbt_tree_node_t *rbt_search(rbt_tree_t *tree, long key);


#endif

