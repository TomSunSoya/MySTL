#ifndef MYSTL_MULTIMAP_H
#define MYSTL_MULTIMAP_H

#include "Pair.h"
#include "utils/RBTreeNode.h"
#include "List.h"

namespace MySTL {

    template<typename K, typename V, typename Compare = std::less<K>>
    class MultiMap {
    public:
        class Iterator {
        public:
            using value_type = Pair<K, V>;
            using pointer = Pair<K, V> *;
            using reference = Pair<K, V> &;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            Iterator(RBTreeNode<K, V> *node, RBTreeNode<K, V> *root) : node(node), root(root) {}

            Iterator(const Iterator &other) = default;

            Iterator &operator=(const Iterator &other) = default;

            ~Iterator() = default;

            reference operator*() const {
                return node->value;
            }

            pointer operator->() const {
                return &node->value;
            }

            Iterator &operator++() {
                node = increment(node);
                return *this;
            }

            Iterator operator++(int) {
                auto temp = *this;
                node = increment(node);
                return temp;
            }

            Iterator &operator--() {
                node = decrement(node);
                return *this;
            }

            Iterator operator--(int) {
                auto temp = *this;
                node = decrement(node);
                return temp;
            }

            bool operator==(const Iterator &other) const {
                return node == other.node;
            }

            bool operator!=(const Iterator &other) const {
                return node != other.node;
            }

            RBTreeNode<K, V> *node;
            RBTreeNode<K, V> *root;

            RBTreeNode<K, V> *increment(RBTreeNode<K, V> *x) {
                if (x->right != nullptr) {
                    x = x->right;
                    while (x->left)
                        x = x->left;
                } else {
                    auto parent = x->parent;
                    while (parent && x == parent->right) {
                        x = parent;
                        parent = parent->parent;
                    }
                    x = parent;
                }
                return x;
            }

            RBTreeNode<K, V> *decrement(RBTreeNode<K, V> *x) {
                if (x->left != nullptr) {
                    x = x->left;
                    while (x->right)
                        x = x->right;
                } else {
                    auto parent = x->parent;
                    while (parent && x == parent->left) {
                        x = parent;
                        parent = parent->parent;
                    }
                    x = parent;
                }
                return x;
            }
        };

        class ReverseIterator {
        public:
            using value_type = Pair<K, V>;
            using pointer = Pair<K, V> *;
            using reference = Pair<K, V> &;
            using difference_type = ptrdiff_t;
            using iterator_category = std::bidirectional_iterator_tag;

            ReverseIterator(RBTreeNode<K, V> *node, RBTreeNode<K, V> *root) : node(node), root(root) {}

            ReverseIterator(const ReverseIterator &other) = default;

            ReverseIterator &operator=(const ReverseIterator &other) = default;

            ~ReverseIterator() = default;

            reference operator*() const {
                return node->value;
            }

            pointer operator->() const {
                return &node->value;
            }

            ReverseIterator &operator++() {
                node = increment(node);
                return *this;
            }

            ReverseIterator operator++(int) {
                auto temp = *this;
                node = increment(node);
                return temp;
            }

            ReverseIterator &operator--() {
                node = decrement(node);
                return *this;
            }

            ReverseIterator operator--(int) {
                auto temp = *this;
                node = decrement(node);
                return temp;
            }

            bool operator==(const ReverseIterator &other) const {
                return node == other.node;
            }

            bool operator!=(const ReverseIterator &other) const {
                return node != other.node;
            }

        private:
            RBTreeNode<K, V> *node;
            RBTreeNode<K, V> *root;

            RBTreeNode<K, V> *increment(RBTreeNode<K, V> *x) {
                if (x->left != nullptr) {
                    x = x->left;
                    while (x->right)
                        x = x->right;
                } else {
                    auto parent = x->parent;
                    while (parent && x == parent->left) {
                        x = parent;
                        parent = parent->parent;
                    }
                    x = parent;
                }
                return x;
            }

            RBTreeNode<K, V> *decrement(RBTreeNode<K, V> *x) {
                if (x->right != nullptr) {
                    x = x->right;
                    while (x->left)
                        x = x->left;
                } else {
                    auto parent = x->parent;
                    while (parent && x == parent->right) {
                        x = parent;
                        parent = parent->parent;
                    }
                    x = parent;
                }
                return x;
            }
        };

    public:
        using iterator = Iterator;
        using const_iterator = const Iterator;
        using reverse_iterator = ReverseIterator;
        using const_reverse_iterator = const ReverseIterator;

        explicit MultiMap(size_t initCap = 16, double loadFactor = 0.75);

        ~MultiMap() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void clear();

        void insert(const K &key, const V &value);

        size_t erase(const K &key);

        void erase(const K &key, const V &value);

        bool contains(const K &key) const;

        iterator find(const K &key) const;

        size_t count(const K &key) const;

        iterator lower_bound(const K &key);

        iterator upper_bound(const K &key);

        Pair<iterator, iterator> equal_range(const K &key);

        iterator begin();

        iterator end();

        const_iterator cbegin();

        const_iterator cend();

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_reverse_iterator crbegin();

        const_reverse_iterator crend();

    private:
        RBTreeNode<K, V> *root;
        size_t len;
        Compare cmp;

        void clearNode(RBTreeNode<K, V> *node);

        RBTreeNode<K, V> *findNode(const K &key) const;

        void insertNode(RBTreeNode<K, V> *z);

        void eraseNode(RBTreeNode<K, V> *z);

        void eraseNode(RBTreeNode<K, V> *node, const V &value);

        void leftRotate(RBTreeNode<K, V> *x);

        void rightRotate(RBTreeNode<K, V> *x);

        void insertFixup(RBTreeNode<K, V> *z);

        void eraseFixup(RBTreeNode<K, V> *x);

        RBTreeNode<K, V> *minimumNode(RBTreeNode<K, V> *x) const;

        RBTreeNode<K, V> *maximumNode(RBTreeNode<K, V> *x) const;
    };


    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::MultiMap(const size_t initCap, const double loadFactor) : root(nullptr), len(0) {}

    template<typename K, typename V, typename Compare>
    bool MultiMap<K, V, Compare>::empty() const {
        return len == 0;
    }

    template<typename K, typename V, typename Compare>
    size_t MultiMap<K, V, Compare>::size() const {
        return len;
    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::clear() {
        clearNode(root);
        root = nullptr;
        len = 0;
    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::insert(const K &key, const V &value) {
        auto newNode = new RBTreeNode<K, V>(key, value);
        insertNode(newNode);
        ++len;
    }

    template<typename K, typename V, typename Compare>
    size_t MultiMap<K, V, Compare>::erase(const K &key) {
        auto range = equal_range(key);
        size_t count = 0;

        for (auto it = range.first; it != range.second; ++it) {
            eraseNode(it.node);
            ++count;
        }
        len -= count;
        return count;
    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::erase(const K &key, const V &value) {
        auto range = equal_range(key);
        for (auto it = range.first; it != range.second; ++it) {
            if (it->second == value) {
                eraseNode(it.node);
                --len;
                return;
            }
        }
    }

    template<typename K, typename V, typename Compare>
    bool MultiMap<K, V, Compare>::contains(const K &key) const {
        return findNode(key) != nullptr;
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::iterator MultiMap<K, V, Compare>::find(const K &key) const {
        return iterator(findNode(key), root);
    }

    template<typename K, typename V, typename Compare>
    size_t MultiMap<K, V, Compare>::count(const K &key) const {
        auto range = equal_range(key);
        size_t count = 0;
        for (auto it = range.first; it != range.second; ++it) {
            ++count;
        }
        return count;
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::iterator MultiMap<K, V, Compare>::lower_bound(const K &key) {
        auto x = root;
        decltype(root) res = nullptr;
        while (x != nullptr) {
            if (!cmp(x->key, key)) {
                res = x;
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(res, root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::iterator MultiMap<K, V, Compare>::upper_bound(const K &key) {
        auto x = root;
        decltype(root) res = nullptr;
        while (x != nullptr) {
            if (cmp(key, x->key)) {
                res = x;
                x = x->left;
            } else {
                x = x->right;
            }
        }
        return iterator(res, root);
    }

    template<typename K, typename V, typename Compare>
    Pair<typename MultiMap<K, V, Compare>::iterator, typename MultiMap<K, V, Compare>::iterator>
    MultiMap<K, V, Compare>::equal_range(const K &key) {
        return Pair<typename MultiMap<K, V, Compare>::iterator, typename MultiMap<K, V, Compare>::iterator>(
                lower_bound(key), upper_bound(key));
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::iterator MultiMap<K, V, Compare>::begin() {
        return MultiMap<K, V, Compare>::iterator(minimumNode(root), root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::iterator MultiMap<K, V, Compare>::end() {
        return MultiMap<K, V, Compare>::iterator(nullptr, root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::const_iterator MultiMap<K, V, Compare>::cbegin() {
        return MultiMap<K, V, Compare>::const_iterator(minimumNode(root), root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::const_iterator MultiMap<K, V, Compare>::cend() {
        return MultiMap<K, V, Compare>::const_iterator(nullptr, root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::reverse_iterator MultiMap<K, V, Compare>::rbegin() {
        return MultiMap<K, V, Compare>::reverse_iterator(maximumNode(root), root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::reverse_iterator MultiMap<K, V, Compare>::rend() {
        return MultiMap<K, V, Compare>::reverse_iterator(nullptr, root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::const_reverse_iterator MultiMap<K, V, Compare>::crbegin() {
        return MultiMap<K, V, Compare>::const_reverse_iterator(maximumNode(root), root);
    }

    template<typename K, typename V, typename Compare>
    MultiMap<K, V, Compare>::const_reverse_iterator MultiMap<K, V, Compare>::crend() {
        return MultiMap<K, V, Compare>::const_reverse_iterator(nullptr, root);
    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::clearNode(RBTreeNode<K, V> *node) {
        if (node != nullptr) {
            clearNode(node->left);
            clearNode(node->right);
            delete node;
        }
    }

    template<typename K, typename V, typename Compare>
    RBTreeNode<K, V> *MultiMap<K, V, Compare>::findNode(const K &key) const {
        auto x = root;
        while (x != nullptr) {
            if (cmp(key, x->key)) {
                x = x->left;
            } else if (cmp(x->key, key)) {
                x = x->right;
            } else {
                return x;
            }
        }
        return nullptr;
    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::insertNode(RBTreeNode<K, V> *z) {
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
    void MultiMap<K, V, Compare>::eraseNode(RBTreeNode<K, V> *z) {
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

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::eraseNode(RBTreeNode<K, V> *node, const V &value) {

    }

    template<typename K, typename V, typename Compare>
    void MultiMap<K, V, Compare>::leftRotate(RBTreeNode<K, V> *x) {
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
    void MultiMap<K, V, Compare>::rightRotate(RBTreeNode<K, V> *x) {
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
    void MultiMap<K, V, Compare>::insertFixup(RBTreeNode<K, V> *z) {
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
    void MultiMap<K, V, Compare>::eraseFixup(RBTreeNode<K, V> *x) {
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
    RBTreeNode<K, V> *MultiMap<K, V, Compare>::minimumNode(RBTreeNode<K, V> *x) const {
        while (x->left != nullptr) x = x->left;
        return x;
    }

    template<typename K, typename V, typename Compare>
    RBTreeNode<K, V> *MultiMap<K, V, Compare>::maximumNode(RBTreeNode<K, V> *x) const {
        while (x->right != nullptr) x = x->right;
        return x;
    }
}


#endif //MYSTL_MULTIMAP_H
