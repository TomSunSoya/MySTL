#ifndef MYSTL_CONCURRENTHASHSET_H
#define MYSTL_CONCURRENTHASHSET_H

#include "ConcurrentHashMap.h"

namespace MySTL {

    template<typename K, typename Hash = std::hash<K>, typename Equal = std::equal_to<K>>
    class ConcurrentHashSet {
    public:
        explicit ConcurrentHashSet(size_t initCap = 16, double loadFactor = 0.75);

        void insert(const K &k);

        void erase(const K &key);

        bool contains(const K &key) const;

        size_t size() const;

        bool empty() const;

        void clear();

    private:
        ConcurrentHashMap<K, bool, Hash, Equal> map_;
    };

    template<typename K, typename Hash, typename Equal>
    ConcurrentHashSet<K, Hash, Equal>::ConcurrentHashSet(size_t initCap, double loadFactor)
            : map_(initCap, loadFactor) {

    }

    template<typename K, typename Hash, typename Equal>
    void ConcurrentHashSet<K, Hash, Equal>::insert(const K &k) {
        map_.insert(k, true);
    }

    template<typename K, typename Hash, typename Equal>
    void ConcurrentHashSet<K, Hash, Equal>::erase(const K &key) {
        map_.erase(key);
    }

    template<typename K, typename Hash, typename Equal>
    bool ConcurrentHashSet<K, Hash, Equal>::contains(const K &key) const {
        return map_.contains(key);
    }

    template<typename K, typename Hash, typename Equal>
    size_t ConcurrentHashSet<K, Hash, Equal>::size() const {
        return map_.size();
    }

    template<typename K, typename Hash, typename Equal>
    bool ConcurrentHashSet<K, Hash, Equal>::empty() const {
        return map_.empty();
    }

    template<typename K, typename Hash, typename Equal>
    void ConcurrentHashSet<K, Hash, Equal>::clear() {
        map_.clear();
    }
}


#endif //MYSTL_CONCURRENTHASHSET_H
