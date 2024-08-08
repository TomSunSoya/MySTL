#ifndef MYSTL_ALLOCATOR_H
#define MYSTL_ALLOCATOR_H

#include <cstddef>
#include <iostream>
#include <new> // For std::bad_alloc

namespace MySTL {

    template<class T>
    class Allocator {
    public:
        using value_type = T;
        using pointer = T*;
        using const_pointer = const T*;
        using reference = T&;
        using const_reference = const T&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        template<class U>
        struct rebind {
            using other = Allocator<U>;
        };

        pointer allocate(size_type n, const void* hint = nullptr) {
            if (n > this->max_size())
                throw std::bad_alloc();
            return static_cast<pointer>(::operator new[](n * sizeof(T)));               // new T[n]会调用T的默认构造函数
        }

        void deallocate(pointer p, size_type /* n */) noexcept {
            ::operator delete[](p);
        }

        template<typename U, typename... Args>
        void construct(U* p, Args&&... args) {
            new(static_cast<void*>(p)) U(std::forward<Args>(args)...);
        }

        void destroy(pointer p) noexcept {
            p->~T();
        }

        pointer address(reference x) const noexcept {
            return std::addressof(x);
        }

        const_pointer const_address(const_reference x) const noexcept {
            return std::addressof(x);
        }

        [[nodiscard]] size_type max_size() const noexcept {
            return static_cast<size_type>(-1) / sizeof(T);
        }
    };

}

#endif // MYSTL_ALLOCATOR_H
