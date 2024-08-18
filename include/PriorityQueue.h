#ifndef MYSTL_PRIORITYQUEUE_H
#define MYSTL_PRIORITYQUEUE_H

#include <algorithm>
#include <functional>

#include "Vector.h"

namespace MySTL {

    template<typename T, typename Container = Vector<T>,
            typename Compare = std::less<T> >
    class PriorityQueue {
    public:
        explicit PriorityQueue(const Compare &comp = Compare(),
                               const Container &cont = Container());

        ~PriorityQueue() = default;

        [[nodiscard]] bool empty() const;

        [[nodiscard]] size_t size() const;

        const T &top() const;

        void push(const T &value);

        void pop();

        template<typename... Args>
        void emplace(Args &&...args);

        void clear();

        Compare get_compare();

    private:
        Container container;
        Compare cmp;
    };

    template<typename T, typename Container, typename Compare>
    bool PriorityQueue<T, Container, Compare>::empty() const {
        return container.empty();
    }

    template<typename T, typename Container, typename Compare>
    size_t PriorityQueue<T, Container, Compare>::size() const {
        return container.size();
    }

    template<typename T, typename Container, typename Compare>
    const T &PriorityQueue<T, Container, Compare>::top() const {
        return container.front();
    }

    template<typename T, typename Container, typename Compare>
    void PriorityQueue<T, Container, Compare>::push(const T &value) {
        container.push_back(value);
        std::push_heap(container.begin(), container.end(), cmp);
    }

    template<typename T, typename Container, typename Compare>
    void PriorityQueue<T, Container, Compare>::pop() {
        std::pop_heap(container.begin(), container.end(), cmp);
        container.pop_back();
    }

    template<typename T, typename Container, typename Compare>
    template<typename... Args>
    void PriorityQueue<T, Container, Compare>::emplace(Args &&...args) {
        container.emplace_back(std::forward<Args>(args)...);
        std::push_heap(container.begin(), container.end(), cmp);
    }

    template<typename T, typename Container, typename Compare>
    void PriorityQueue<T, Container, Compare>::clear() {
        container.clear();
    }

    template<typename T, typename Container, typename Compare>
    Compare PriorityQueue<T, Container, Compare>::get_compare() {
        return cmp;
    }

    template<typename T, typename Container, typename Compare>
    PriorityQueue<T, Container, Compare>::PriorityQueue(const Compare &comp,
                                                        const Container &cont)
            : container(cont), cmp(comp) {
        std::make_heap(container.begin(), container.end(), cmp);
    }
}  // namespace MySTL

#endif  // MYSTL_PRIORITYQUEUE_H
