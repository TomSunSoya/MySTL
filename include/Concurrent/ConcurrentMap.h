#ifndef MYSTL_CONCURRENTMAP_H
#define MYSTL_CONCURRENTMAP_H

#include "../Map.h"

#include <mutex>

namespace MySTL {

    template<typename K, typename V, typename Compare = std::less<K>>
    class ConcurrentMap {
    public:
        explicit ConcurrentMap(Compare cmp = Compare()) = default;

        void insert(const K &k, const V &v);

        void erase(const K &k);

        V *find(const K &k);

        V &at(const K &k);

        V &operator[](const K &k);

        bool contains(const K &k) const;

        size_t size() const;

        bool empty() const;

        void clear();

    private:
        mutable std::mutex mutex_;
        Map<K, V, Compare> map_;
    };

    template<typename K, typename V, typename Compare>
    void ConcurrentMap<K, V, Compare>::insert(const K &k, const V &v) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_.insert(k, v);
    }

    template<typename K, typename V, typename Compare>
    void ConcurrentMap<K, V, Compare>::erase(const K &k) {
        std::lock_guard<std::mutex> lock(mutex_);
        map_.erase(k);
    }

    template<typename K, typename V, typename Compare>
    V &ConcurrentMap<K, V, Compare>::operator[](const K &k) {
        return at(k);
    }

    template<typename K, typename V, typename Compare>
    V *ConcurrentMap<K, V, Compare>::find(const K &k) {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.find(k);
    }

    template<typename K, typename V, typename Compare>
    V &ConcurrentMap<K, V, Compare>::at(const K &k) {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.at(k);
    }

    template<typename K, typename V, typename Compare>
    bool ConcurrentMap<K, V, Compare>::contains(const K &k) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.contains(k);
    }

    template<typename K, typename V, typename Compare>
    size_t ConcurrentMap<K, V, Compare>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.size();
    }

    template<typename K, typename V, typename Compare>
    bool ConcurrentMap<K, V, Compare>::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return map_.empty();
    }

    template<typename K, typename V, typename Compare>
    void ConcurrentMap<K, V, Compare>::clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        map_.clear();
    }
}


#endif //MYSTL_CONCURRENTMAP_H
