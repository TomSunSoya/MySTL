//
// Created by qiuyudai on 24-8-8.
//

#include "../include/GenericIterator.h"

namespace MySTL
{
    template <typename T>
    bool GenericIterator<T>::operator==(const GenericIterator& other) const
    {
        return m_ptr == other.m_ptr;
    }

    template <typename T>
    bool GenericIterator<T>::operator!=(const GenericIterator& other) const
    {
        return m_ptr != other.m_ptr;
    }

    template <typename T>
    bool GenericIterator<T>::operator>(const GenericIterator& other) const
    {
        return m_ptr > other.m_ptr;
    }

    template <typename T>
    bool GenericIterator<T>::operator<(const GenericIterator& other) const
    {
        return m_ptr < other.m_ptr;
    }

    template <typename T>
    bool GenericIterator<T>::operator>=(const GenericIterator& other) const
    {
        return m_ptr >= other.m_ptr;
    }

    template <typename T>
    bool GenericIterator<T>::operator<=(const GenericIterator& other) const
    {
        return m_ptr <= other.m_ptr;
    }
}