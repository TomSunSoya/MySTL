#ifndef MYSTL_CONCURRENTSET_H
#define MYSTL_CONCURRENTSET_H

#include "ConcurrentMap.h"

namespace MySTL {
    template<typename K, typename Compare = std::less<K>>
    class ConcurrentSet {
    public:
        explicit ConcurrentSet(const Compare &comp = Compare()) : map_(comp) {}

        [[nodiscard]] bool empty() const { return map_.empty(); }

        [[nodiscard]] size_t size() const { return map_.size(); }

        void insert(const K &k) {
            map_.insert(k, true);
        }

        size_t erase(const K &k) {
            return map_.erase(k);
        }

        size_t count(const K &k) {
            return map_.contains(k);
        }

        bool contains(const K &k) const {
            return map_.contains(k);
        }

        void clear() {
            map_.clear();
        }

    private:
        ConcurrentMap<K, bool, Compare> map_;
    };

}


#endif //MYSTL_CONCURRENTSET_H
