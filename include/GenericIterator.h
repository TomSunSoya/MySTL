#ifndef GENERICITERATOR_H
#define GENERICITERATOR_H

#include <random>

namespace MySTL {
    template<typename T>
    class GenericIterator {
    public:
        using iterator_category [[maybe_unused]] = std::random_access_iterator_tag;
        using value_type [[maybe_unused]] = T;
        using different_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        explicit GenericIterator(pointer ptr = nullptr) : m_ptr(ptr) {}

        reference operator*() const { return *m_ptr; }

        reference operator->() { return m_ptr; }

        GenericIterator &operator++() {
            ++m_ptr;
            return *this;
        }

        GenericIterator operator++(int) {
            GenericIterator temp = *this;
            ++(*this);
            return temp;
        }

        GenericIterator operator--() {
            --m_ptr;
            return *this;
        }

        GenericIterator operator--(int) {
            GenericIterator temp = *this;
            --(*this);
            return temp;
        }

        GenericIterator operator+(different_type n) const {
            return GenericIterator(m_ptr + n);
        }

        GenericIterator operator-(different_type n) const {
            return GenericIterator(m_ptr - n);
        }

        different_type operator-(const GenericIterator &other) const {
            return m_ptr - other.m_ptr;
        }

        bool operator==(const GenericIterator &other) const;

        bool operator!=(const GenericIterator &other) const;

        bool operator<(const GenericIterator &other) const;

        bool operator>(const GenericIterator &other) const;

        bool operator<=(const GenericIterator &other) const;

        bool operator>=(const GenericIterator &) const;

        reference operator[](different_type n) const { return *(m_ptr + n); }

    private:
        pointer m_ptr;
    };

    template<typename T>
    class GenericReverseIterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        // 构造函数
        explicit GenericReverseIterator(pointer ptr = nullptr) : m_ptr(ptr) {}

        // 解引用操作符
        reference operator*() const { return *m_ptr; }

        pointer operator->() { return m_ptr; }

        // 前置和后置自减
        GenericReverseIterator &operator++() {
            --m_ptr;
            return *this;
        }  // 反向迭代器的自增是指针的自减
        GenericReverseIterator operator++(int) {
            GenericReverseIterator temp = *this;
            --m_ptr;
            return temp;
        }

        // 前置和后置自增
        GenericReverseIterator &operator--() {
            ++m_ptr;
            return *this;
        }  // 反向迭代器的自减是指针的自增
        GenericReverseIterator operator--(int) {
            GenericReverseIterator temp = *this;
            ++m_ptr;
            return temp;
        }

        // 迭代器比较
        bool operator==(const GenericReverseIterator &other) const {
            return m_ptr == other.m_ptr;
        }

        bool operator!=(const GenericReverseIterator &other) const {
            return m_ptr != other.m_ptr;
        }

    private:
        pointer m_ptr;
    };
}  // namespace MySTL

namespace MySTL {
    template<typename T>
    bool GenericIterator<T>::operator==(const GenericIterator &other) const {
        return m_ptr == other.m_ptr;
    }

    template<typename T>
    bool GenericIterator<T>::operator!=(const GenericIterator &other) const {
        return m_ptr != other.m_ptr;
    }

    template<typename T>
    bool GenericIterator<T>::operator>(const GenericIterator &other) const {
        return m_ptr > other.m_ptr;
    }

    template<typename T>
    bool GenericIterator<T>::operator<(const GenericIterator &other) const {
        return m_ptr < other.m_ptr;
    }

    template<typename T>
    bool GenericIterator<T>::operator>=(const GenericIterator &other) const {
        return m_ptr >= other.m_ptr;
    }

    template<typename T>
    bool GenericIterator<T>::operator<=(const GenericIterator &other) const {
        return m_ptr <= other.m_ptr;
    }
}  // namespace MySTL

#endif  // GENERICITERATOR_H
