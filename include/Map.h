#ifndef MYSTL_MAP_H
#define MYSTL_MAP_H

#include <cstdlib>
#include <functional>

#include "utils/RBTreeNode.h"

#include "ReverseIterator.h"

namespace MySTL {

    template<typename K, typename V, typename Compare = std::less<K> >
    class Map {
    public:
        Map();

        ~Map() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        V *find(const K &key);

        void insert(const K &key, const V &value);

        void erase(const K &key);

        void clear();

        bool contains(const K &key) const;

        class Iterator {
        public:
            using Node = RBTreeNode<K, V>;

            Iterator(Node *node) : node(node) {}

            V &operator*() const {
                return node->value;
            }

            V *operator->() const {
                return &node->value;
            }

            Iterator &operator++() {
                if (node->right != nullptr) {
                    node = node->right;
                    while (node->left != nullptr) node = node->left;
                } else {
                    auto parent = node->parent;
                    while (parent != nullptr && node == parent->right) {
                        node = parent;
                        parent = parent->parent;
                    }
                    node = parent;
                }
                return *this;
            }

            Iterator &operator--() {
                if (node->left != nullptr) {
                    node = node->left;
                    while (node->right != nullptr) node = node->right;
                } else {
                    auto parent = node->parent;
                    while (parent != nullptr && node == parent->left) {
                        node = parent;
                        parent = parent->parent;
                    }
                    node = parent;
                }
                return *this;
            }

            bool operator==(const Iterator &other) const {
                return node == other.node;
            }

            bool operator!=(const Iterator &other) const {
                return node != other.node;
            }

        private:
            Node *node;
        };

        using iterator = Iterator;
        using const_iterator = const Iterator;
        using reverse_iterator = ReverseIterator<iterator>;
        using reverse_const_iterator = ReverseIterator<const_iterator>;
        using iterator_category = std::bidirectional_iterator_tag;

        iterator begin() {
            return iterator(minimum(root));
        }

        iterator end() {
            return iterator(nullptr);
        }

        const_iterator cbegin() const {
            return const_iterator(minimum(root));
        }

        const_iterator cend() const {
            return const_iterator(nullptr);
        }

        reverse_iterator rbegin() {
            return reverse_iterator(maximum(root));
        }

        reverse_iterator rend() {
            return reverse_iterator(nullptr);
        }

        reverse_const_iterator crbegin() const {
            return reverse_const_iterator(maximum(root));
        }

        reverse_const_iterator crend() const {
            return reverse_const_iterator(nullptr);
        }

    private:
        RBTreeNode<K, V> *root;
        size_t len;
        Compare cmp;

        void leftRotate(RBTreeNode<K, V> *x);

        void rightRotate(RBTreeNode<K, V> *x);

        void insertFixup(RBTreeNode<K, V> *z);

        void transplant(RBTreeNode<K, V> *u, RBTreeNode<K, V> *v);

        void deleteFixup(RBTreeNode<K, V> *x);

        RBTreeNode<K, V> *minimum(RBTreeNode<K, V> *x);

        RBTreeNode<K, V> *maximum(RBTreeNode<K, V> *x);

        RBTreeNode<K, V> *findNode(K key);

        void clearNode(RBTreeNode<K, V> *x);

        void insertNode(RBTreeNode<K, V> *z);

        void deleteNode(RBTreeNode<K, V> *z);
    };

    template<typename K, typename V, typename Compare>
    Map<K, V, Compare>::Map() : root(nullptr), len(0), cmp(Compare()) {}

    template<typename K, typename V, typename Compare>
    bool Map<K, V, Compare>::empty() const {
        return len == 0;
    }

    template<typename K, typename V, typename Compare>
    size_t Map<K, V, Compare>::size() const {
        return len;
    }

    template<typename K, typename V, typename Compare>
    V *Map<K, V, Compare>::find(const K &key) {
        auto x = findNode(key);
        if (x != nullptr) return &x->value;
        return nullptr;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::insert(const K &key, const V &value) {
        auto newNode = new RBTreeNode<K, V>(key, value);
        insertNode(newNode);
        ++len;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::erase(const K &key) {
        auto node = findNode(key);
        if (node == nullptr) return;
        deleteNode(node);
        --len;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::clear() {
        clearNode(root);
        root = nullptr;
        len = 0;
    }

    template<typename K, typename V, typename Compare>
    bool Map<K, V, Compare>::contains(const K &key) const {
        return find(key) != nullptr;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::leftRotate(RBTreeNode<K, V> *x) {
        auto y = x->right;
        x->right = y->left;

        if (y->left != nullptr) y->left->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->left)
            x->parent->left = y;
        else
            x->parent->right = y;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::rightRotate(RBTreeNode<K, V> *x) {
        auto y = x->left;
        x->left = y->right;

        if (y->right != nullptr) y->right->parent = x;
        y->parent = x->parent;

        if (x->parent == nullptr)
            root = y;
        else if (x == x->parent->right)
            x->parent->right = y;
        else
            x->parent->left = y;

        y->right = x;
        x->parent = y;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::insertFixup(RBTreeNode<K, V> *z) {
        while (z->parent != nullptr && z->parent->color == RED) {
            if (z->parent == z->parent->parent->left) {
                auto y = z->parent->parent->right;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->right) {
                        z = z->parent;
                        leftRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    rightRotate(z->parent->parent);
                }
            } else {
                auto y = z->parent->parent->left;
                if (y != nullptr && y->color == RED) {
                    z->parent->color = BLACK;
                    y->color = BLACK;
                    z->parent->parent->color = RED;
                    z = z->parent->parent;
                } else {
                    if (z == z->parent->left) {
                        z = z->parent;
                        rightRotate(z);
                    }
                    z->parent->color = BLACK;
                    z->parent->parent->color = RED;
                    leftRotate(z->parent->parent);
                }
            }
        }
        root->color = BLACK;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::transplant(RBTreeNode<K, V> *u, RBTreeNode<K, V> *v) {
        if (u->parent == nullptr)
            root = v;
        else if (u == u->parent->left)
            u->parent->left = v;
        else
            u->parent->right = v;

        if (v != nullptr) v->parent = u->parent;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::deleteFixup(RBTreeNode<K, V> *x) {
        while (x != root && (x == nullptr || x->color == BLACK)) {
            if (x == x->parent->left) {
                auto w = x->parent->right;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    leftRotate(x->parent);
                    w = x->parent->right;
                }
                if ((w->left == nullptr || w->left->color == BLACK) &&
                    (w->right == nullptr || w->right->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->right == nullptr || w->right->color == BLACK) {
                        if (w->left != nullptr) w->left->color = BLACK;
                        w->color = RED;
                        rightRotate(w);
                        w = x->parent->right;
                    }
                    if (w != nullptr) w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->right != nullptr) w->right->color = BLACK;
                    leftRotate(x->parent);
                    x = root;
                }
            } else {
                auto w = x->right->left;
                if (w->color == RED) {
                    w->color = BLACK;
                    x->parent->color = RED;
                    rightRotate(x->parent);
                    w = x->parent->left;
                }
                if ((w->right == nullptr || w->right->color == BLACK) &&
                    (w->left == nullptr || w->left->color == BLACK)) {
                    w->color = RED;
                    x = x->parent;
                } else {
                    if (w->left == nullptr || w->left->color == BLACK) {
                        if (w->right != nullptr) w->right->color = BLACK;
                        w->color = RED;
                        leftRotate(w);
                        w = x->parent->left;
                    }
                    if (w != nullptr) w->color = x->parent->color;
                    x->parent->color = BLACK;
                    if (w->left != nullptr) w->left->color = BLACK;
                    rightRotate(x->parent);
                    x = root;
                }
            }
        }
        if (x != nullptr) x->color = BLACK;
    }

    template<typename K, typename V, typename Compare>
    RBTreeNode<K, V> *Map<K, V, Compare>::minimum(RBTreeNode<K, V> *x) {
        while (x->left != nullptr) x = x->left;
        return x;
    }

    template<typename K, typename V, typename Compare>
    RBTreeNode<K, V> *Map<K, V, Compare>::maximum(RBTreeNode<K, V> *x) {
        while (x->right != nullptr) x = x->right;
        return x;
    }

    template<typename K, typename V, typename Compare>
    RBTreeNode<K, V> *Map<K, V, Compare>::findNode(K key) {
        auto x = root;
        while (x != nullptr) {
            if (cmp(key, x->key))
                x = x->left;
            else if (cmp(x->key, key))
                x = x->right;
            else
                return x;
        }
        return nullptr;
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::clearNode(RBTreeNode<K, V> *x) {
        if (x != nullptr) {
            clearNode(x->left);
            clearNode(x->right);
            delete x;
        }
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::insertNode(RBTreeNode<K, V> *z) {
        RBTreeNode<K, V> *y = nullptr;
        auto x = root;
        while (x != nullptr) {
            y = x;
            if (cmp(z->key, x->key))
                x = x->left;
            else
                x = x->right;
        }
        z->parent = y;
        if (y == nullptr)
            root = z;
        else if (cmp(z->key, y->key))
            y->left = z;
        else
            y->right = z;
        z->left = nullptr;
        z->right = nullptr;
        z->color = RED;
        insertFixup(z);
    }

    template<typename K, typename V, typename Compare>
    void Map<K, V, Compare>::deleteNode(RBTreeNode<K, V> *z) {
        auto y = z;
        auto yOriginalColor = y->color;
        RBTreeNode<K, V> *x;
        if (z->left == nullptr) {
            x = z->right;
            transplant(z, z->right);
        } else if (z->right == nullptr) {
            x = z->left;
            transplant(z, z->left);
        } else {
            y = minimum(z->right);
            yOriginalColor = y->color;
            x = y->right;
            if (y->parent == z)
                x->parent = y;
            else {
                transplant(y, y->right);
                y->right = z->right;
                y->right->parent = y;
            }
            transplant(z, y);
            y->left = z->left;
            y->left->parent = y;
            y->color = z->color;
        }
        if (yOriginalColor == BLACK) deleteFixup(x);
        delete z;
    }
}  // namespace MySTL

#endif  // MYSTL_MAP_H
