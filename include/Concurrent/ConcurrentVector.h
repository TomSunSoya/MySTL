#ifndef MYSTL_CONCURRENTVECTOR_H
#define MYSTL_CONCURRENTVECTOR_H

#include "../Vector.h"

#include <shared_mutex>
#include <mutex>
#include <stdexcept>

namespace MySTL {
    template<typename T>
    class ConcurrentVector {
    public:
        ConcurrentVector() = default;

        ~ConcurrentVector() = default;

        void push_back(const T &value);

        T get(size_t index) const;

        void set(size_t index, const T &value);

        size_t size() const;

        bool empty() const;

        void clear();

        size_t capacity() const;

        void reserve(size_t new_cap);

        void shrink_to_fit();

        void insert(size_t pos, const T &value);

        void erase(size_t pos);

    private:
        mutable std::shared_mutex mutex_;
        Vector<T> data;
    };

    template<typename T>
    void ConcurrentVector<T>::push_back(const T &value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.push_back(value);
    }

    template<typename T>
    T ConcurrentVector<T>::get(size_t index) const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        if (index >= data.size()) {
            throw std::out_of_range("ConcurrentVector::get");
        }
        return data[index];
    }

    template<typename T>
    void ConcurrentVector<T>::set(size_t index, const T &value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        if (index >= data.size()) {
            throw std::out_of_range("ConcurrentVector::set");
        }
        data[index] = value;
    }

    template<typename T>
    size_t ConcurrentVector<T>::size() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return data.size();
    }

    template<typename T>
    bool ConcurrentVector<T>::empty() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return data.empty();
    }

    template<typename T>
    void ConcurrentVector<T>::clear() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.clear();
    }

    template<typename T>
    size_t ConcurrentVector<T>::capacity() const {
        std::shared_lock<std::shared_mutex> lock(mutex_);
        return data.capacity();
    }

    template<typename T>
    void ConcurrentVector<T>::reserve(size_t new_cap) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.reserve(new_cap);
    }

    template<typename T>
    void ConcurrentVector<T>::shrink_to_fit() {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.shrink_to_fit();
    }

    template<typename T>
    void ConcurrentVector<T>::insert(size_t pos, const T &value) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.insert(pos, value);
    }

    template<typename T>
    void ConcurrentVector<T>::erase(size_t pos) {
        std::unique_lock<std::shared_mutex> lock(mutex_);
        data.erase(pos);
    }
}


#endif //MYSTL_CONCURRENTVECTOR_H
