#ifndef MYSTL_CONCURRENTSTACK_H
#define MYSTL_CONCURRENTSTACK_H

#include "../Stack.h"

#include <mutex>


namespace MySTL {

    template<typename T>
    class ConCurrentStack {
    public:
        ConCurrentStack() = default;

        ~ConCurrentStack() = default;

        ConCurrentStack(const ConCurrentStack &) = delete;

        ConCurrentStack &operator=(const ConCurrentStack &) = delete;

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
    void ConCurrentStack<T>::push(const T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push(value);
    }

    template<typename T>
    void ConCurrentStack<T>::push(T &&value) {
        std::lock_guard<std::mutex> lock(mutex_);
        stack_.push(std::move(value));
    }

    template<typename T>
    bool ConCurrentStack<T>::try_pop(T &value) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            return false;
        }
        value = std::move(stack_.top());
        stack_.pop();
        return true;
    }

    template<typename T>
    void ConCurrentStack<T>::pop() {
        std::lock_guard<std::mutex> lock(mutex_);
        if (stack_.empty()) {
            throw std::out_of_range("ConCurrentStack::pop");
        }
        stack_.pop();
    }

    template<typename T>
    bool ConCurrentStack<T>::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stack_.empty();
    }

    template<typename T>
    size_t ConCurrentStack<T>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return stack_.size();
    }

} // namespace MySTL


#endif //MYSTL_CONCURRENTSTACK_H
