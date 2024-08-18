#ifndef MYSTL_PAIR_H
#define MYSTL_PAIR_H

#include <algorithm>

namespace MySTL {
    template <typename T1, typename T2>
    class Pair {
    public:
        Pair() = default;
        Pair(const T1& first, const T2& second);
        Pair(const Pair<T1, T2>& other) = default;
        Pair(Pair<T1, T2>&& other) noexcept = default;
        ~Pair() = default;

        Pair& operator=(const Pair<T1, T2>& other) = default;
        Pair& operator=(Pair<T1, T2>&& other) noexcept = default;

        bool operator==(const Pair<T1, T2>& other) const;
        bool operator!=(const Pair<T1, T2>& other) const;
        bool operator<(const Pair<T1, T2>& other) const;
        bool operator>(const Pair<T1, T2>& other) const;
        bool operator<=(const Pair<T1, T2>& other) const;
        bool operator>=(const Pair<T1, T2>& other) const;

        void swap(Pair<T1, T2>& other);

        T1 first;
        T2 second;
    };

    template<typename T1, typename T2>
    Pair<T1, T2>::Pair(const T1 &first, const T2 &second) : first(first), second(second) {
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator==(const Pair<T1, T2> &other) const {
        return first == other.first && second == other.second;
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator!=(const Pair<T1, T2> &other) const {
        return !(*this == other);
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator<(const Pair<T1, T2> &other) const {
        if (first < other.first) {
            return true;
        } else if (first == other.first) {
            return second < other.second;
        }
        return false;
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator>(const Pair<T1, T2> &other) const {
        if (first > other.first) {
            return true;
        } else if (first == other.first) {
            return second > other.second;
        }
        return false;
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator<=(const Pair<T1, T2> &other) const {
        return !(*this > other);
    }

    template<typename T1, typename T2>
    bool Pair<T1, T2>::operator>=(const Pair<T1, T2> &other) const {
        return !(*this < other);
    }

    template<typename T1, typename T2>
    void Pair<T1, T2>::swap(Pair<T1, T2> &other) {
        using std::swap;
        swap(first, other.first);
        swap(second, other.second);
    }

    template<typename T1, typename T2>
    void swap(Pair<T1, T2>& lhs, Pair<T1, T2>& rhs) noexcept {
        lhs.swap(rhs);
    }
}


#endif //MYSTL_PAIR_H
