//
// Created by qiuyudai on 2023/12/14.
//

#ifndef MYSTL_VECTOR_H
#define MYSTL_VECTOR_H

namespace MySTL {
    template<typename T>
    class Vector final {
    public:
        Vector();
        explicit Vector(size_t sz, const T &t = T());
        ~Vector();

        bool empty();
        size_t size();
        size_t capacity();
        void resize(size_t sz);
        void resize(size_t sz, const T &t);
        void reserve(size_t sz);
        void shrink_to_fit();

        void push_back(const T& value);
        void push_back(T&& value);
        void pop_back();
        T& back();
        T& front();
        void clear();
        T& operator[](size_t index);
        const T& operator[](size_t index) const;
        T& at(size_t index);
        const T& at(size_t index) const;

    private:
        T *data;
        size_t len;
        size_t cap;

        static constexpr size_t MIN_SIZE = 8;

        T* allocate_memory(size_t new_capacity);
        void deallocate_memory(T* t);
        void grow_capacity();
        void shrink_capacity();
        void copy_elements_to(T* destination);
        void move_elements_to(T* destination);
    };
}


#endif //MYSTL_VECTOR_H
