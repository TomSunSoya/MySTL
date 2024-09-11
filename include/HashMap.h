#ifndef MYSTL_HASHMAP_H
#define MYSTL_HASHMAP_H

#include <functional>

#include "List.h"
#include "Pair.h"
#include "Vector.h"

namespace MySTL {
    template<typename K, typename V, typename Hash = std::hash<K>,
            typename Equal = std::equal_to<K>>
    class HashMap {
    public:
        explicit HashMap(size_t initCap = 16, double loadFactor = 0.75);

        ~HashMap() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void clear();

        void insert(const K &k, const V &v);

        void insert(const Pair<K, V> &pair);

        void erase(const K &key);

        V &at(const K &key);

        V &operator[](const K &key);

        bool contains(const K &key) const;

        V *find(const K &key);

        Hash &getHasher() { return hasher; }

        size_t getCap() { return cap; }

    private:
        using PairType = Pair<K, V>;
        using ListType = List<PairType>;
        using VectorType = Vector<ListType>;

        VectorType table;
        size_t len;
        double loadFactor;
        size_t cap;
        Hash hasher;
        Equal equal;

        void rehash();
    };

    template<typename K, typename V, typename Hash, typename Equal>
    V *HashMap<K, V, Hash, Equal>::find(const K &key) {
        size_t index = hasher(key) % cap;
        for (auto &pair: table[index])
            if (equal(pair.first, key)) return &pair.second;
        return nullptr;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMap<K, V, Hash, Equal>::rehash() {
        VectorType newTable(cap * 2);
        for (auto &list: table) {
            for (auto &pair: list) {
                size_t index = hasher(pair.first) % (cap * 2);
                newTable[index].emplace_back(pair.first, pair.second);
            }
        }
        table = std::move(newTable);
        cap *= 2;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMap<K, V, Hash, Equal>::insert(const K &k, const V &v) {
        size_t index = hasher(k) % cap;
        for (auto &pair: table[index]) {
            if (equal(pair.first, k)) {
                pair.second = v;
                return;
            }
        }
        table[index].emplace_back(k, v);
        ++len;
        if (static_cast<double>(len) / cap > loadFactor) {
            rehash();
        }
    }

    template<typename K, typename V, typename Hash, typename Equal>
    HashMap<K, V, Hash, Equal>::HashMap(size_t initCap, double loadFactor)
            : table(initCap), len(0), loadFactor(loadFactor), cap(initCap) {}

    template<typename K, typename V, typename Hash, typename Equal>
    bool HashMap<K, V, Hash, Equal>::empty() const {
        return len == 0;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    size_t HashMap<K, V, Hash, Equal>::size() const {
        return len;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMap<K, V, Hash, Equal>::clear() {
        table.clear();
        len = 0;
        table.resize(cap);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMap<K, V, Hash, Equal>::insert(const Pair<K, V> &pair) {
        insert(pair.first, pair.second);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMap<K, V, Hash, Equal>::erase(const K &key) {
        size_t index = hasher(key) % cap;
        for (auto it = table[index].begin(); it != table[index].end(); ++it) {
            if (equal(it->first, key)) {
                table[index].erase(it);
                --len;
                return;
            }
        }
    }

    template<typename K, typename V, typename Hash, typename Equal>
    V &HashMap<K, V, Hash, Equal>::at(const K &key) {
        size_t index = hasher(key) % cap;
        for (auto &pair: table[index]) {
            if (equal(pair.first, key)) return pair.second;
        }
        table[index].emplace_back(key, V());
        ++len;
        if (static_cast<double>(len) / cap > loadFactor) rehash();
        return table[index].back().second;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    V &HashMap<K, V, Hash, Equal>::operator[](const K &key) {
        return at(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    bool HashMap<K, V, Hash, Equal>::contains(const K &key) const {
        size_t index = hasher(key) % cap;
        for (auto &pair: table[index])
            if (equal(pair.first, key)) return true;
        return false;
    }

}  // namespace MySTL

#endif  // MYSTL_HASHMAP_H
