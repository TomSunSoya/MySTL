#ifndef MYSTL_SHAREDPTR_H
#define MYSTL_SHAREDPTR_H

#include <utility>
#include <atomic>

namespace MySTL {

    template<typename T>
    class SharedPtr {
    public:
        SharedPtr() noexcept: ptr(nullptr), refCount(new std::atomic_size_t(1)) {}

        explicit SharedPtr(const T *ptr) : ptr(ptr), refCount(new std::atomic_size_t(1)) {}

        SharedPtr(const SharedPtr<T> &other) : ptr(other.ptr), refCount(other.refCount) {
            ++(*refCount);
        }

        SharedPtr(SharedPtr<T> &&other) noexcept: ptr(other.ptr), refCount(other.refCount) {
            other.ptr = nullptr;
            other.refCount = nullptr;
        }

        ~SharedPtr() {
            reset();
        }

        SharedPtr<T> &operator=(const SharedPtr<T> &other) {
            if (this != &other) {
                reset();
                ptr = other.ptr;
                refCount = other.refCount;
                ++(*refCount);
            }
            return *this;
        }

        SharedPtr<T> &operator=(SharedPtr<T> &&other) noexcept {
            if (this != &other) {
                reset();
                ptr = other.ptr;
                refCount = other.refCount;
                other.ptr = nullptr;
                other.refCount = nullptr;
            }
            return *this;
        }

        T &operator*() const noexcept {
            return *ptr;
        }

        T *operator->() const noexcept {
            return ptr;
        }

        T *get() {
            return ptr;
        }

        T *release() noexcept {
            auto oldPtr = ptr;
            ptr = nullptr;
            if (refCount && --*(refCount) == 0) {
                delete refCount;
                refCount = nullptr;
            }
            return oldPtr;
        }

        [[nodiscard]] std::size_t useCount() const noexcept {
            if (refCount) return *refCount;
            return 0;
        }

        [[nodiscard]] bool unique() const noexcept {
            return useCount() == 1;
        }

        void reset(T *newPtr = nullptr) noexcept {
            if (ptr && --(*refCount) == 0) {
                delete ptr;
                delete refCount;
            }
            ptr = newPtr;
            refCount = ptr ? new std::atomic_size_t(1) : nullptr;
        }

    private:
        T *ptr;
        std::atomic_size_t *refCount;
    };

}


#endif //MYSTL_SHAREDPTR_H
