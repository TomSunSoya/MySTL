#ifndef MYSTL_SET_H
#define MYSTL_SET_H

#include "Map.h"

namespace MySTL {

    template <typename T, typename Compare = std::less<T>>
    class Set {
    public:
        explicit Set(const Compare& comp = Compare()) : map(comp), len(0) {}

        [[nodiscard]] bool empty() const {
            return len == 0;
        }

        [[nodiscard]] size_t size() const {
            return len;
        }

        void insert(const T& t) {
            map.insert(t, true);
            ++len;
        }

        void erase(const T& t) {
            map.erase(t);
            --len;
        }

        bool contains(const T& t) const {
            return map.contains(t);
        }

        void clear() {
            map.clear();
            len = 0;
        }

    private:
        using MapType = Map<T, bool, Compare>;
        MapType map;
        size_t len;
    };

}


#endif //MYSTL_SET_H
