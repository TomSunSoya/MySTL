#ifndef MYSTL_CONCURRENTSTACK_H
#define MYSTL_CONCURRENTSTACK_H

#include "../Stack.h"

#include <mutex>


namespace MySTL {

    template<typename T>
    class ConcurrentStack {
    public:
        ConcurrentStack() = default;

        ~ConcurrentStack() = default;

        ConcurrentStack(const ConcurrentStack &) = delete;

        ConcurrentStack &operator=(const ConcurrentStack &) = delete;

        void push(const T &value);

        void push(T &&value);

        bool try_pop(T &value);

        void pop();

        bool empty() const;

        size_t size() const;

    private:
        mutable std::mutex mutex_;
        Stack<T> stack_;
    };

    template<typename T>
    void ConcurrentStack<T>::push(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push(value);
    }

    template<typename T>
    void ConcurrentStack<T>::push(T &&value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push(std::move(value));
    }

    template<typename T>
    bool ConcurrentStack<T>::try_pop(T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            return false;
        }
        value = std::move(stack_.top());
        stack_.pop();
        return true;
    }

    template<typename T>
    void ConcurrentStack<T>::pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            throw std::out_of_range("ConcurrentStack::pop");
        }
        stack_.pop();
    }

    template<typename T>
    bool ConcurrentStack<T>::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stack_.empty();
    }

    template<typename T>
    size_t ConcurrentStack<T>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stack_.size();
    }

} // namespace MySTL


#endif //MYSTL_CONCURRENTSTACK_H
