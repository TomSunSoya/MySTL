#ifndef MYSTL_RBTREENODE_H
#define MYSTL_RBTREENODE_H

namespace MySTL {
    enum Color {
        RED, BLACK
    };

    template<typename K, typename V>
    struct RBTreeNode {
        K key;
        V value;
        Color color;
        RBTreeNode *left;
        RBTreeNode *right;
        RBTreeNode *parent;

        RBTreeNode(K key, V value)
                : key(key),
                  value(value),
                  color(RED),
                  left(nullptr),
                  right(nullptr),
                  parent(nullptr) {}
    };
}


#endif //MYSTL_RBTREENODE_H
