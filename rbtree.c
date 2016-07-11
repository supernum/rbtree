#include <malloc.h>
#include <string.h>
#include "rbtree.h"

/* Create rbtree */
rbt_tree_t *rbt_create(void) {
    rbt_tree_t *tree = malloc(sizeof(rbt_tree_t));    
    if (tree == NULL) return NULL;
    tree->root = NULL;
    return tree;
}

void _rbt_node_release(rbt_tree_node_t *node) {
    if (!node) return;
    _rbt_node_release(node->left); 
    _rbt_node_release(node->right); 
    free(node);
    node = NULL;
}

/* Release rbtree */
void rbt_release(rbt_tree_t *tree) {
    if (!tree) return;
    _rbt_node_release(tree->root);
    free(tree);
}

void rbt_node_init(rbt_tree_node_t *node) {
    node->parent = node->left = node->right = NULL;
    rbt_red(node);
}

void rbt_left_rotate(rbt_tree_node_t **root, rbt_tree_node_t *node) {
    if (!node->right)
        return;

    if (node->parent) {
        if (node->parent->left == node)
            node->parent->left = node->right;
        else
            node->parent->right = node->right;
        node->right->parent = node->parent;
    } else {
        node->right->parent = NULL;
        *root = node->right;
    }
    node->parent = node->right;
    node->right = node->parent->left; 
    if (node->right)
        node->right->parent = node;
    node->parent->left = node;
}

void rbt_right_rotate(rbt_tree_node_t **root, rbt_tree_node_t *node) {
    if (!node->left)
        return;

    if (node->parent) {
        if (node->parent->left == node)
            node->parent->left = node->left;
        else
            node->parent->right = node->left;
        node->left->parent = node->parent;
    } else {
        node->left->parent = NULL;
        *root = node->left;
    }
    node->parent = node->left;
    node->left = node->parent->right; 
    if (node->left)
    node->left->parent = node;
    node->parent->right = node;
}

void rbt_insert_fixup(rbt_tree_node_t **root, rbt_tree_node_t *node) {
    rbt_tree_node_t *u;

    while (node != *root && rbt_is_red(node->parent)) {
        if (node->parent == grandparent(node)->left) {
            u = grandparent(node)->right;
            if (u && rbt_is_red(u)) {
                rbt_black(node->parent);
                rbt_black(u);
                rbt_red(grandparent(node));
                node = grandparent(node);
            } else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rbt_left_rotate(root, node);
                }
                rbt_black(node->parent);
                rbt_red(grandparent(node));
                rbt_right_rotate(root, grandparent(node));
            }
        } else {
            u = grandparent(node)->left;
            if (u && rbt_is_red(u)) {
                rbt_black(node->parent);
                rbt_black(u);
                rbt_red(grandparent(node));
                node = grandparent(node);
            } else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rbt_right_rotate(root, node);
                }
                rbt_black(node->parent);
                rbt_red(grandparent(node));
                rbt_left_rotate(root, grandparent(node));
            }
        }
    }
    rbt_black(*root);
    return;
}

/* 
 * Insert an element into a rbtree 
 * */
int rbt_insert(rbt_tree_t *tree, rbt_tree_node_t *node) {    
    rbt_tree_node_t *parent, **root = &tree->root;
    
    rbt_node_init(node);    
    if (*root == NULL) {
        *root = node;
        rbt_black(*root); 
    } else {
        for (parent = *root; ; ) {
            if (node->key > parent->key) {
                if (parent->right) {
                    parent = parent->right;
                } else {
                    parent->right = node;
                    break;
                }
            } else if (node->key < parent->key) {
                if (parent->left) {
                    parent = parent->left;
                } else {
                    parent->left = node;
                    break;
                }
            } else {
                return 0; 
            }
        }
        node->parent = parent;
        rbt_insert_fixup(root, node);
    }

    return 1;
}

void rbt_delete_fixup(rbt_tree_node_t **root, rbt_tree_node_t *node) {
    rbt_tree_node_t *w;

    while (node != *root && rbt_is_black(node)) {
        if (node == node->parent->left) {
            w = node->parent->right;
            if (rbt_is_red(w)) {
                rbt_black(w); 
                rbt_red(node->parent);
                rbt_left_rotate(root, node->parent);
                w = node->parent->right;
            } 
            if (rbt_is_black(w->left) && rbt_is_black(w->right)) {
                rbt_red(w);
                node = node->parent;
            } else {
                if (rbt_is_black(w->right)) {
                    rbt_black(w->left);
                    rbt_red(w);
                    rbt_right_rotate(root, w);
                    w = node->parent->right;
                }
                w->color = node->parent->color;
                rbt_black(node->parent);
                rbt_black(w->right);
                rbt_left_rotate(root, node->parent);
                node = *root;
            }
        } else {
            w = node->parent->left;
            if (rbt_is_red(w)) {
                rbt_black(w); 
                rbt_red(node->parent);
                rbt_right_rotate(root, node->parent);
                w = node->parent->left;
            } 
            if (rbt_is_black(w->left) && rbt_is_black(w->right)) {
                rbt_red(w);
                node = node->parent;
            } else {
                if (rbt_is_black(w->left)) {
                    rbt_black(w->right);
                    rbt_red(w);
                    rbt_left_rotate(root, w);
                    w = node->parent->left;
                }
                w->color = node->parent->color;
                rbt_black(node->parent);
                rbt_black(w->right);
                rbt_right_rotate(root, node->parent);
                node = *root;
            }
        }
    }    
    rbt_black(node);
}

/* 
 * Remove an element from a rbtree
 * */
rbt_tree_node_t *rbt_delete(rbt_tree_t *tree, long key) {
    rbt_tree_node_t *node, *sub, *tmp, **root = &tree->root;
    int isred;
    
    node = rbt_search(tree, key); 
    if (!node) return NULL;

    if (!node->left) {
        tmp = node->right;
        sub = node;
    } else if (!node->right) {
        tmp = node->left;    
        sub = node;
    } else {
        sub = rbt_min(node->right);
        tmp = sub->right;
    }

    if (sub == *root) {
        *root = tmp;
        if (*root) rbt_black(*root);
        return node;
    }

    if (sub == sub->parent->left) {
        sub->parent->left = tmp;
    } else {
        sub->parent->right = tmp;
    }
    
    isred = rbt_is_red(sub);
    if (sub == node) {
        if (tmp) {
            tmp->parent = sub->parent;
        }
    } else {
        if (tmp && sub->parent != node) {
            tmp->parent = sub->parent;
        }
        sub->left = node->left;
        sub->right = node->right;
        sub->parent = node->parent;
        sub->color = node->color;
        if (sub->left) {
            sub->left->parent = sub;
        }
        if (sub->right) {
            sub->right->parent = sub;  
        }
        if (node == *root) {
            *root = sub;
        } else {
            if (node == node->parent->left) {
                node->parent->left = sub;
            } else {
                node->parent->right = sub;
            }
        }
    }
    if (isred) {
        return node;
    }
    if (tmp)
        rbt_delete_fixup(root, tmp);
    return node;   
}

rbt_tree_node_t *rbt_search(rbt_tree_t *tree, long key) {
    rbt_tree_node_t *node, **root = &tree->root;

    for (node = *root; node; ) {
        if (key > node->key) {
            node = node->right; 
        } else if (key < node->key) {
            node = node->left;
        } else {
            return node;
        }
    }
    return NULL;
}
