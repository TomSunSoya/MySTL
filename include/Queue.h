//
// Created by qiuyudai on 2024/8/15.
//

#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "Deque.h"

namespace MySTL {

    template<typename T>
    class Queue {
    public:
        Queue() = default;

        ~Queue() = default;

        void push(const T &value) { deque.push_back(value); }

        void push(T &&value) {
            deque.push_back(std::move(value));
        }

        void pop() { deque.pop_front(); }

        [[nodiscard]] bool empty() const { return deque.empty(); }

        [[nodiscard]] size_t size() const { return deque.size(); }

        const T &front() { return deque.front(); }

        const T &back() { return deque.back(); }

    private:
        Deque<T> deque;
    };

}  // namespace MySTL

#endif  // MYSTL_QUEUE_H
