#ifndef MYSTL_HASHMULTISET_H
#define MYSTL_HASHMULTISET_H

#include "HashMultiMap.h"


namespace MySTL {

    template<typename T, typename Hash = std::hash<T>, typename Equal = std::equal_to<T>>
    class HashMultiSet {
    public:
        explicit HashMultiSet(size_t initCap = 16, double loadFactor = 0.75);

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void clear();

        void insert(const T &t);

        void erase(const T &t);

        bool contains(const T &t) const;

        size_t count(const T &t) const;

    private:
        HashMultiMap<T, bool, Hash, Equal> hashMap;
    };

    template<typename T, typename Hash, typename Equal>
    HashMultiSet<T, Hash, Equal>::HashMultiSet(size_t initCap, double loadFactor) {
        hashMap = HashMultiMap<T, bool, Hash, Equal>(initCap, loadFactor);
    }

    template<typename T, typename Hash, typename Equal>
    bool HashMultiSet<T, Hash, Equal>::empty() const {
        return hashMap.empty();
    }

    template<typename T, typename Hash, typename Equal>
    size_t HashMultiSet<T, Hash, Equal>::size() const {
        return hashMap.size();
    }

    template<typename T, typename Hash, typename Equal>
    void HashMultiSet<T, Hash, Equal>::clear() {
        hashMap.clear();
    }

    template<typename T, typename Hash, typename Equal>
    void HashMultiSet<T, Hash, Equal>::insert(const T &t) {
        hashMap.insert(t, true);
    }

    template<typename T, typename Hash, typename Equal>
    void HashMultiSet<T, Hash, Equal>::erase(const T &t) {
        hashMap.erase(t);
    }

    template<typename T, typename Hash, typename Equal>
    bool HashMultiSet<T, Hash, Equal>::contains(const T &t) const {
        return hashMap.contains(t);
    }

    template<typename T, typename Hash, typename Equal>
    size_t HashMultiSet<T, Hash, Equal>::count(const T &t) const {
        return hashMap.count(t);
    }
}


#endif //MYSTL_HASHMULTISET_H
