#ifndef MYSTL_HASHSET_H
#define MYSTL_HASHSET_H

#include "HashMap.h"

namespace MySTL {
    template<typename T, typename Hash = std::hash<T>,
            typename Equal = std::equal_to<T> >
    class HashSet {
    public:
        explicit HashSet(const size_t &initialCap = 16,
                         const double &loadFactor = 0.75);

        ~HashSet() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        void insert(const T &t);

        void erase(const T &t);

        bool contains(const T &t) const;

        void clear();

    private:
        HashMap<T, bool, Hash, Equal> hashMap;
    };

    template<typename T, typename Hash, typename Equal>
    HashSet<T, Hash, Equal>::HashSet(const size_t &initialCap,
                                     const double &loadFactor)
            : hashMap(initialCap, loadFactor) {}

    template<typename T, typename Hash, typename Equal>
    bool HashSet<T, Hash, Equal>::empty() const {
        return hashMap.empty();
    }

    template<typename T, typename Hash, typename Equal>
    size_t HashSet<T, Hash, Equal>::size() const {
        return hashMap.size();
    }

    template<typename T, typename Hash, typename Equal>
    void HashSet<T, Hash, Equal>::insert(const T &t) {
        hashMap.insert(t, true);
    }

    template<typename T, typename Hash, typename Equal>
    void HashSet<T, Hash, Equal>::erase(const T &t) {
        hashMap.erase(t);
    }

    template<typename T, typename Hash, typename Equal>
    bool HashSet<T, Hash, Equal>::contains(const T &t) const {
        return hashMap.contains(t, true);
    }

    template<typename T, typename Hash, typename Equal>
    void HashSet<T, Hash, Equal>::clear() {
        hashMap.clear();
    }

}  // namespace MySTL

#endif  // MYSTL_HASHSET_H
