#ifndef MYSTL_HASHMULTIMAP_H
#define MYSTL_HASHMULTIMAP_H

#include <functional>

#include "List.h"
#include "Pair.h"
#include "Vector.h"

namespace MySTL {
    template<typename K, typename V, typename Hash = std::hash<K>,
            typename Equal = std::equal_to<K> >
    class HashMultiMap {
    public:
        explicit HashMultiMap(size_t initCap = 16, double loadFactor = 0.75);

        ~HashMultiMap() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void clear();

        void insert(const K &k, const V &v);

        void insert(const Pair<K, V> &pair);

        size_t erase(const K &key);

        void erase(const K &key, const V &value);

        bool contains(const K &key) const;

        List<V> find(const K &key) const;

        size_t count(const K &key) const;

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
    size_t HashMultiMap<K, V, Hash, Equal>::count(const K &key) const {
        size_t index = hasher(key) % cap;
        size_t count = 0;
        for (auto &pair: table[index])
            if (equal(pair.first, key)) ++count;
        return count;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    List<V> HashMultiMap<K, V, Hash, Equal>::find(const K &key) const {
        size_t index = hasher(key) % cap;
        List<V> res;
        for (auto &pair: table[index])
            if (equal(pair.first, key)) res.push_back(pair.second);
        return res;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMultiMap<K, V, Hash, Equal>::rehash() {
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
    void HashMultiMap<K, V, Hash, Equal>::insert(const K &k, const V &v) {
        size_t index = hasher(k) % cap;
        table[index].emplace_back(k, v);
        ++len;
        if (static_cast<double>(len) / cap > loadFactor) {
            rehash();
        }
    }

    template<typename K, typename V, typename Hash, typename Equal>
    HashMultiMap<K, V, Hash, Equal>::HashMultiMap(size_t initCap, double loadFactor)
            : table(initCap), len(0), loadFactor(loadFactor), cap(initCap) {}

    template<typename K, typename V, typename Hash, typename Equal>
    bool HashMultiMap<K, V, Hash, Equal>::empty() const {
        return len == 0;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    size_t HashMultiMap<K, V, Hash, Equal>::size() const {
        return len;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMultiMap<K, V, Hash, Equal>::clear() {
        for (auto &x: table) x.clear();
        table.clear();
        len = 0;
        table.resize(cap);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMultiMap<K, V, Hash, Equal>::insert(const Pair<K, V> &pair) {
        insert(pair.first, pair.second);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    size_t HashMultiMap<K, V, Hash, Equal>::erase(const K &key) {
        size_t index = hasher(key) % cap;
        int count = 0;
        for (auto it = table[index].begin(); it != table[index].end();) {
            if (equal(it->first, key)) {
                it = table[index].erase(it);
                --len;
                ++count;
            } else ++it;
        }
        return count;
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void HashMultiMap<K, V, Hash, Equal>::erase(const K &key, const V &value) {
        size_t index = hasher(key) % cap;
        for (auto it = table[index].begin(); it != table[index].end();) {
            if (equal(it->first, key) && it->second == value) {
                table[index].erase(it);
                --len;
                break;
            } else ++it;
        }
    }

    template<typename K, typename V, typename Hash, typename Equal>
    bool HashMultiMap<K, V, Hash, Equal>::contains(const K &key) const {
        size_t index = hasher(key) % cap;
        for (auto &pair: table[index])
            if (equal(pair.first, key)) return true;
        return false;
    }

}  // namespace MySTL

#endif //MYSTL_HASHMULTIMAP_H
