//
// Created by qiuyudai on 2024/8/15.
//

#ifndef MYSTL_QUEUE_H
#define MYSTL_QUEUE_H

#include "Deque.h"
#include "GenericIterator.h"

namespace MySTL {

    template<typename T>
    class Queue {
    public:
        Queue() = default;

        ~Queue() = default;

        void push(const T &value) {
            deque.push_back(value);
        }

        void pop() {
            deque.pop_front();
        }

        [[nodiscard]] bool empty() const {
            return deque.empty();
        }

        [[nodiscard]] size_t size() const {
            return deque.size();
        }

        const T &front() {
            return deque.front();
        }

        const T &back() {
            return deque.back();
        }

        using iterator = GenericIterator<T>;
        using const_iterator = GenericIterator<const T>;
        using reverse_iterator = GenericReverseIterator<T>;
        using const_reverse_iterator = GenericReverseIterator<const T>;

        iterator begin() {
            return iterator(front());
        }


    private:
        Deque<T> deque;
    };

}


#endif //MYSTL_QUEUE_H
