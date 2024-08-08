//
// Created by qiuyudai on 24-8-8.
//

#ifndef GENERICITERATOR_H
#define GENERICITERATOR_H

#include <random>

template <typename T>
class GenericIterator {
public:
    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using different_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    explicit GenericIterator(pointer ptr = nullptr) : m_ptr(ptr) {}

    reference operator*() const {
        return *m_ptr;
    }

    reference operator->()
    {
        return m_ptr;
    }



private:
    pointer m_ptr;
};



#endif //GENERICITERATOR_H
