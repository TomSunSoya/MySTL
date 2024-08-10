#ifndef VECTOR_H_
#define VECTOR_H_

#include <algorithm>

#include "GenericIterator.h"

namespace MySTL {
    template<typename T>
    class Vector final {
    public:
        Vector();

        explicit Vector(size_t sz, const T &t = T());

        Vector(const Vector &other);

        Vector(Vector &&other) noexcept;

        Vector &operator=(const Vector &other);

        Vector &operator=(Vector &&other) noexcept;

        Vector(std::initializer_list<T> list);

        ~Vector();

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        [[nodiscard]] size_t capacity() const;

        void resize(size_t sz);

        void resize(size_t sz, const T &t);

        void reserve(size_t sz);

        void shrink_to_fit();

        void push_back(const T &value);

        void push_back(T &&value);

        void pop_back();

        T &back();

        T &front();

        void clear();

        T &operator[](size_t index);

        const T &operator[](size_t index) const;

        T &at(size_t index);

        const T &at(size_t index) const;

        void insert(size_t index, const T &value);

        void erase(size_t index);

        template<typename... Args>
        void emplace_back(Args &&... args);

        void swap(Vector &other) noexcept;

        using iterator = GenericIterator<T>;
        using const_interator = GenericIterator<const T>;
        using reverse_iterator = GenericReverseIterator<T>;
        using const_reverse_iterator = GenericReverseIterator<const T>;

        iterator begin();

        iterator end();

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_interator cbegin();

        const_interator cend();

        const_reverse_iterator crbegin();

        const_reverse_iterator crend();

    private:
        T *data;
        size_t len;
        size_t cap;

        static constexpr size_t MIN_SIZE = 8;

        T *allocate_memory(size_t new_capacity);

        void deallocate_memory(T *t);

        void grow_capacity();

        void shrink_capacity();

        void copy_elements_to(T *destination);

        void move_elements_to(T *destination);
    };
}


namespace MySTL {
    template<typename T>
    Vector<T>::Vector() {
        data = allocate_memory(MIN_SIZE);
        len = 0;
        cap = MIN_SIZE;
    }


    template<typename T>
    Vector<T>::Vector(size_t sz, const T &t) {
        data = allocate_memory(sz);
        len = sz;
        cap = sz;
        for (int i = 0; i < sz; ++i) data[i] = t;
    }

    template<typename T>
    Vector<T>::Vector(const Vector &other) {
        data = allocate_memory(other.cap);
        len = other.len;
        cap = other.cap;
        other.copy_elements_to(data);
    }

    template<typename T>
    Vector<T>::Vector(Vector &&other) noexcept
            : data(other.data), len(other.len), cap(other.cap) {
        other.data = nullptr;
        other.len = 0;
        other.cap = 0;
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(const Vector &other) {
        if (this == &other) return *this;

        if (cap < other.len) {
            deallocate_memory(data);
            data = allocate_memory(other.cap);
            cap = other.cap;
        }

        len = other.len;
        other.copy_elements_to(data);

        return *this;
    }

    template<typename T>
    Vector<T> &Vector<T>::operator=(Vector &&other) noexcept {
        if (this == &other) return *this;

        deallocate_memory(data);
        data = other.data;
        len = other.len;
        cap = other.cap;

        other.data = nullptr;
        other.len = 0;
        other.cap = 0;

        return *this;
    }

    template<typename T>
    Vector<T>::Vector(std::initializer_list<T> list) : data(allocate_memory(list.size())), len(list.size()),
                                                       cap(list.size()) {
        std::copy(list.begin(), list.end(), data);
    }


    template<typename T>
    Vector<T>::~Vector() {
        deallocate_memory(data);
    }

    template<typename T>
    bool Vector<T>::empty() const {
        return len == 0;
    }

    template<typename T>
    size_t Vector<T>::size() const {
        return len;
    }

    template<typename T>
    size_t Vector<T>::capacity() const {
        return cap;
    }

    template<typename T>
    void Vector<T>::resize(size_t sz) {
        auto t = allocate_memory(sz * 2);
        deallocate_memory(data);
        data = t;
        len = sz;
        cap = sz * 2;
    }

    template<typename T>
    void Vector<T>::resize(size_t sz, const T &t) {
        auto tmp = allocate_memory(sz * 2);
        deallocate_memory(data);
        data = tmp;
        for (size_t i = 0; i < sz; ++i) data[i] = t;
        len = sz;
        cap = sz * 2;
    }

    template<typename T>
    void Vector<T>::reserve(size_t sz) {
        auto t = allocate_memory(sz);
    }

    template<typename T>
    T *Vector<T>::allocate_memory(size_t new_capacity) {
        if (new_capacity == 0) return nullptr;

        auto new_data = new T[new_capacity]();
        if (new_data == nullptr) throw std::bad_alloc();

        return new_data;
    }

    template<typename T>
    void Vector<T>::deallocate_memory(T *t) {
        delete[]t;
    }

    template<typename T>
    void Vector<T>::copy_elements_to(T *destination) {
        if (data == destination) return;
        for (int i = 0; i < len; ++i) destination[i] = data[i];
    }

    template<typename T>
    void Vector<T>::move_elements_to(T *destination) {
        if (data == destination) return;
        for (int i = 0; i < len; ++i) destination[i] = std::move(data[i]);
    }

    template<typename T>
    void Vector<T>::grow_capacity() {
        cap = cap == 0 ? MIN_SIZE : cap * 2;
        auto t = allocate_memory(cap);
        for (int i = 0; i < len; ++i) t[i] = data[i];
        deallocate_memory(data);
        data = t;
        cap *= 2;
    }

    template<typename T>
    void Vector<T>::shrink_capacity() {
        auto t = allocate_memory(cap / 2);
        for (int i = 0; i < len; ++i) t[i] = data[i];
        deallocate_memory(data);
        data = t;
        cap /= 2;
    }

    template<typename T>
    void Vector<T>::shrink_to_fit() {
        if (len < cap) {
            T *new_data = new T[len];
            for (size_t i = 0; i < len; ++i) new_data[i] = data[i];
            delete[] data;

            data = new_data;
            cap = len;
        }
    }


    template<typename T>
    void Vector<T>::push_back(const T &value) {
        if (len >= cap)
            grow_capacity();
        data[len] = value;
        ++len;
    }

    template<typename T>
    void Vector<T>::push_back(T &&value) {
        if (len >= cap)
            grow_capacity();
        data[len] = std::move(value);
        ++len;
    }

    template<typename T>
    void Vector<T>::pop_back() {
        if (len == 0) return;
        --len;
        data[len].~T();
    }

    template<typename T>
    T &Vector<T>::back() {
        return data[len - 1];
    }

    template<typename T>
    T &Vector<T>::front() {
        return data[0];
    }

    template<typename T>
    void Vector<T>::clear() {
        for (int i = 0; i < len; ++i) data[i].~T();
        len = 0;
    }

    template<typename T>
    T &Vector<T>::operator[](size_t index) {
        return data[index];
    }

    template<typename T>
    const T &Vector<T>::operator[](size_t index) const {
        return data[index];
    }

    template<typename T>
    T &Vector<T>::at(size_t index) {
        if (index >= len)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    template<typename T>
    const T &Vector<T>::at(size_t index) const {
        if (index >= len)
            throw std::out_of_range("Index out of range");
        return data[index];
    }

    template<typename T>
    void Vector<T>::insert(size_t index, const T &value) {
        if (len == cap) {
            grow_capacity();
        }

        std::move_backward(data + index, data + len, data + len + 1);
        data[index] = value;
        ++len;
    }

    template<typename T>
    void Vector<T>::erase(size_t index) {
        if (index < len) {
            std::move(data + index + 1, data + len, data + index);
            --len;
            data[len].~T();
        }
    }

    template<typename T>
    template<typename... Args>
    void Vector<T>::emplace_back(Args &&... args) {
        if (len == cap) {
            grow_capacity();
        }
        new(data + len) T(std::forward<Args>(args)...);
        ++len;
    }

    template<typename T>
    void Vector<T>::swap(Vector &other) noexcept {
        using std::swap;
        swap(data, other.data);
        swap(len, other.len);
        swap(cap, other.cap);
    }


    template<typename T>
    typename Vector<T>::iterator Vector<T>::begin() {
        return iterator(data);
    }

    template<typename T>
    typename Vector<T>::iterator Vector<T>::end() {
        return iterator(data + len);
    }

    template<typename T>
    typename Vector<T>::const_interator Vector<T>::cbegin() {
        return const_interator(data);
    }

    template<typename T>
    typename Vector<T>::const_interator Vector<T>::cend() {
        return const_interator(data + len);
    }

    template<typename T>
    typename Vector<T>::reverse_iterator Vector<T>::rbegin() {
        return reverse_iterator(data + len - 1);
    }

    template<typename T>
    typename Vector<T>::reverse_iterator Vector<T>::rend() {
        return reverse_iterator(data - 1);
    }

    template<typename T>
    typename Vector<T>::const_reverse_iterator Vector<T>::crbegin() {
        return const_reverse_iterator(data + len - 1);
    }

    template<typename T>
    typename Vector<T>::const_reverse_iterator Vector<T>::crend() {
        return const_reverse_iterator(data - 1);
    }
}

#endif