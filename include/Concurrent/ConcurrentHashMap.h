#ifndef MYSTL_CONCURRENTHASHMAP_H
#define MYSTL_CONCURRENTHASHMAP_H

#include "../HashMap.h"

#include <mutex>

namespace MySTL {

    template<typename K, typename V, typename Hash = std::hash<K>, typename Equal = std::equal_to<K>>
    class ConcurrentHashMap {
    public:
        explicit ConcurrentHashMap(size_t initCap = 16, double loadFactor = 0.75);

        void insert(const K &k, const V &v);

        void erase(const K &key);

        V *find(const K &key);

        V &at(const K &key);

        V &operator[](const K &key);

        bool contains(const K &key) const;

        size_t size() const;

        bool empty() const;

        void clear();

    private:
        HashMap<K, V, Hash, Equal> map_;
        mutable std::mutex mutex_;
        mutable Vector<std::mutex> mutexes;
    };

    template<typename K, typename V, typename Hash, typename Equal>
    ConcurrentHashMap<K, V, Hash, Equal>::ConcurrentHashMap(size_t initCap, double loadFactor)
            : map_(initCap, loadFactor), mutexes(initCap) {

    }

    template<typename K, typename V, typename Hash, typename Equal>
    void ConcurrentHashMap<K, V, Hash, Equal>::insert(const K &k, const V &v) {
        size_t index = map_.getHasher()(k) % map_.getCap();
        std::lock_guard<std::mutex> lock(mutexes[index]);
        map_.insert(k, v);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void ConcurrentHashMap<K, V, Hash, Equal>::erase(const K &key) {
        size_t index = map_.getHasher()(key) % map_.getCap();
        std::lock_guard<std::mutex> lock(mutexes[index]);
        map_.erase(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    V *ConcurrentHashMap<K, V, Hash, Equal>::find(const K &key) {
        size_t index = map_.getHasher()(key) % map_.getCap();
        std::lock_guard<std::mutex> lock(mutexes[index]);
        return map_.find(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    V &ConcurrentHashMap<K, V, Hash, Equal>::at(const K &key) {
        size_t index = map_.getHasher()(key) % map_.getCap();
        std::lock_guard<std::mutex> lock(mutexes[index]);
        return map_.at(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    V &ConcurrentHashMap<K, V, Hash, Equal>::operator[](const K &key) {
        return at(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    bool ConcurrentHashMap<K, V, Hash, Equal>::contains(const K &key) const {
        size_t index = map_.getHasher()(key) % map_.getCap();
        std::lock_guard<std::mutex> lock(mutexes[index]);
        return map_.contains(key);
    }

    template<typename K, typename V, typename Hash, typename Equal>
    size_t ConcurrentHashMap<K, V, Hash, Equal>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.size();
    }

    template<typename K, typename V, typename Hash, typename Equal>
    bool ConcurrentHashMap<K, V, Hash, Equal>::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.empty();
    }

    template<typename K, typename V, typename Hash, typename Equal>
    void ConcurrentHashMap<K, V, Hash, Equal>::clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        map_.clear();
    }

}


#endif //MYSTL_CONCURRENTHASHMAP_H
