#ifndef MYSTL_MULTISET_H
#define MYSTL_MULTISET_H


#include "MultiMap.h"

namespace MySTL {

    template<typename T, typename Compare = std::less<T> >
    class MultiSet {
    public:
        using iterator = typename MultiMap<T, bool, Compare>::iterator;
        using const_iterator = typename MultiMap<T, bool, Compare>::const_iterator;
        using reverse_iterator = typename MultiMap<T, bool, Compare>::reverse_iterator;
        using const_reverse_iterator = typename MultiMap<T, bool, Compare>::const_reverse_iterator;


        explicit MultiSet(const Compare &comp = Compare()) : map(comp), len(0) {}

        [[nodiscard]] bool empty() const { return len == 0; }

        [[nodiscard]] size_t size() const { return len; }

        void insert(const T &t) {
            map.insert(t, true);
            ++len;
        }

        size_t erase(const T &t) {
            auto count = map.erase(t);
            len -= count;
            return count;
        }

        size_t count(const T &t) {
            return map.count(t);
        }

        bool contains(const T &t) const { return map.contains(t); }

        void clear() {
            map.clear();
            len = 0;
        }


        iterator begin() { return map.begin(); }

        const_iterator cbegin() const { return map.cbegin(); }

        iterator end() { return map.end(); }

        const_iterator cend() const { return map.cend(); }

        reverse_iterator rbegin() { return map.rbegin(); }

        const_reverse_iterator crbegin() const { return map.crbegin(); }

        reverse_iterator rend() { return map.rend(); }

        const_reverse_iterator crend() const { return map.crend(); }

        iterator find(const T &t) { return map.find(t); }

        iterator lower_bound(const T &t) { return map.lower_bound(t); }

        iterator upper_bound(const T &t) { return map.upper_bound(t); }

        Pair<iterator, iterator> equal_range(const T &t) { return map.equal_range(t); }


    private:
        using MapType = MultiMap<T, bool, Compare>;
        MapType map;
        size_t len;
    };

}


#endif //MYSTL_MULTISET_H
