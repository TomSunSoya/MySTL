#ifndef MYSTL_STACK_H
#define MYSTL_STACK_H

#include "Deque.h"

namespace MySTL {
    template<typename T>
    class Stack {
    public:
        Stack() = default;

        ~Stack() = default;

        void push(const T &value) { data.push_back(value); }

        void pop() { data.pop_back(); }

        [[nodiscard]] bool empty() const { return data.empty(); }

        [[nodiscard]] size_t size() const { return data.size(); }

        const T &top() { return data.back(); }

    private:
        Deque<T> data;
    };

}


#endif //MYSTL_STACK_H
