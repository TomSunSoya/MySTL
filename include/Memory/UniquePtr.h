#ifndef MYSTL_UNIQUEPTR_H
#define MYSTL_UNIQUEPTR_H

#include <utility>
#include <iostream>

namespace MySTL {

    template<typename T, typename Deleter = std::default_delete<T>>
    class UniquePtr {
    public:
        UniquePtr() noexcept: ptr(nullptr), deleter(Deleter()) {}

        explicit UniquePtr(T *ptr) noexcept: ptr(ptr), deleter(Deleter()) {}

        UniquePtr(T *ptr, const Deleter &deleter) noexcept: ptr(ptr), deleter(deleter) {}

        ~UniquePtr() {
            reset();
        }

        UniquePtr(const UniquePtr &) = delete;

        UniquePtr &operator=(const UniquePtr &) = delete;

        UniquePtr(UniquePtr &&other) noexcept {
            ptr = other.ptr;
            deleter = std::move(other.deleter);
            other.ptr = nullptr;
        }

        UniquePtr &operator=(UniquePtr &&other) noexcept {
            if (this != &other) {
                reset();
                ptr = other.ptr;
                deleter = std::move(other.deleter);
                other.ptr = nullptr;
            }
            return *this;
        }

        T &operator*() const {
            return *ptr;
        }

        T *operator->() const {
            return ptr;
        }

        T *get() const noexcept {
            return ptr;
        }

        T *release() noexcept {
            auto tmp = ptr;
            ptr = nullptr;
            return tmp;
        }

        void reset(T *newPtr = nullptr) {
            if (ptr) {
                deleter(ptr);
            }
            ptr = newPtr;
        }

        explicit operator bool() const noexcept {
            return ptr != nullptr;
        }

    private:
        T *ptr;
        Deleter deleter;
    };

}


#endif //MYSTL_UNIQUEPTR_H
