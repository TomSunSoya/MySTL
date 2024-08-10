#include <algorithm>

import MyModule.GenericIterator;
export module MyModule.Vector;

namespace MySTL {
    export template<typename T>
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

        void insert(size_t index, const T& value);
        void erase(size_t index);

        template <typename... Args>
        void emplace_back(Args&&... args);

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

        T* allocate_memory(size_t new_capacity);
        void deallocate_memory(T* t);
        void grow_capacity();
        void shrink_capacity();
        void copy_elements_to(T* destination);
        void move_elements_to(T* destination);
    };
}
