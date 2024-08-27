#ifndef MYSTL_WEAKPTR_H
#define MYSTL_WEAKPTR_H

#include "SharedPtr.h"

namespace MySTL {

    template<typename T>
    class WeakPtr {
    public:
        WeakPtr() noexcept: ptr(nullptr), refCount(nullptr) {}

        explicit WeakPtr(const SharedPtr<T> &sharedPtr) : ptr(sharedPtr.ptr), refCount(sharedPtr.refCount) {}

        WeakPtr(const WeakPtr<T> &other) : ptr(other.ptr), refCount(other.refCount) {}

        WeakPtr(WeakPtr<T> &&other) noexcept: ptr(other.ptr), refCount(other.refCount) {
            other.ptr = nullptr;
            other.refCount = nullptr;
        }

        ~WeakPtr() {
            reset();
        }

        WeakPtr<T> &operator=(const WeakPtr<T> &other) {
            if (this != &other) {
                ptr = other.ptr;
                refCount = other.refCount;
            }
            return *this;
        }

        WeakPtr<T> &operator=(WeakPtr<T> &&other) noexcept {
            if (this != &other) {
                ptr = other.ptr;
                refCount = other.refCount;
                other.ptr = nullptr;
                other.refCount = nullptr;
            }
            return *this;
        }

        SharedPtr<T> lock() {
            return expired() ? SharedPtr<T>() : SharedPtr<T>(*this);
        }

        [[nodiscard]] bool expired() const {
            return refCount == nullptr || *refCount == 0;
        }

        void reset() {
            ptr = nullptr;
            refCount = nullptr;
        }

        [[nodiscard]] std::size_t use_count() const {
            if (refCount) return *refCount;
            return 0;
        }

    private:
        T *ptr;
        std::atomic_size_t *refCount;

    };

}


#endif //MYSTL_WEAKPTR_H
