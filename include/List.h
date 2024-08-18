#ifndef LIST_H
#define LIST_H

#include <stdexcept>

#include "GenericIterator.h"


namespace MySTL {
    template<typename T>
    class List {
    public:
        List();

        ~List();

        void push_back(const T &value);

        void pop_back();

        void push_front(const T &value);

        void pop_front();

        [[nodiscard]] bool empty() const;

        const T &back();

        const T &front();

        void clear();

        [[nodiscard]] size_t size() const;

        void insert(const T &value, size_t pos);

        // Erase all elements with the given value
        void erase(const T &value);

        void erase(size_t pos);

        void reverse();

        bool find(const T &value);

        using iterator = GenericIterator<T>;
        using const_iterator = GenericIterator<const T>;
        using reverse_iterator = GenericReverseIterator<T>;
        using const_reverse_iterator = GenericReverseIterator<const T>;

        iterator begin();

        iterator end();

        reverse_iterator rbegin();

        reverse_iterator rend();

        const_iterator cbegin() const;

        const_iterator cend() const;

        const_reverse_iterator crbegin() const;

        const_reverse_iterator crend() const;


    private:
        struct Node {
            T data;
            Node *prev;
            Node *next;

            explicit Node(const T &data) : data(data), prev(nullptr), next(nullptr) {}
        };

        Node *head;
        Node *tail;
        size_t len{};
    };

    template<typename T>
    List<T>::List() : head(nullptr), tail(nullptr), len(0) {}

    template<typename T>
    List<T>::~List() {
        clear();
    }

    template<typename T>
    const T &List<T>::back() {
        if (len == 0) throw std::out_of_range("List is empty");
        return tail->data;
    }

    template<typename T>
    const T &List<T>::front() {
        if (len == 0) throw std::out_of_range("List is empty");
        return head->data;
    }

    template<typename T>
    bool List<T>::empty() const {
        return len == 0;
    }

    template<typename T>
    void List<T>::push_back(const T &value) {
        auto node = new Node(value);
        if (tail) {
            tail->next = node;
            node->prev = tail;
            tail = node;
        } else
            head = tail = node;
        ++len;
    }

    template<typename T>
    void List<T>::pop_back() {
        if (len == 0) throw std::out_of_range("List is empty");
        if (len == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            auto t = tail->prev;
            delete tail;
            tail = t;
            tail->next = nullptr;
        }
        --len;
    }

    template<typename T>
    void List<T>::push_front(const T &value) {
        auto node = new Node(value);
        if (head) {
            head->prev = node;
            node->next = head;
            head = node;
        } else
            head = tail = node;
        ++len;
    }

    template<typename T>
    void List<T>::pop_front() {
        if (len == 0) throw std::out_of_range("List is empty");
        if (len == 1) {
            delete head;
            head = tail = nullptr;
        } else {
            auto t = head->next;
            delete head;
            head = t;
            head->prev = nullptr;
        }
        --len;
    }

    template<typename T>
    bool List<T>::find(const T &value) {
        Node *current = head;
        while (current) {
            if (current->data == value)
                return true;
            current = current->next;
        }
        return false;
    }

    template<typename T>
    void List<T>::reverse() {
        if (len <= 1) return;
        Node *current = head;
        while (current) {
            auto t = current->next;
            current->next = current->prev;
            current->prev = t;
            current = t;
        }
    }

    template<typename T>
    void List<T>::erase(const size_t pos) {
        if (pos >= len) throw std::out_of_range("Index out of range");
        if (pos == 0) pop_front();
        else if (pos == len - 1) pop_back();
        else {
            auto current = head;
            for (size_t i = 0; i < pos; ++i)
                current = current->next;
            current->prev->next = current->next;
            current->next->prev = current->prev;
            delete current;
            --len;
        }
    }

    template<typename T>
    void List<T>::erase(const T &value) {
        Node *current = head;
        while (current) {
            if (current->data == value) {
                if (current == head) pop_front();
                else if (current == tail) pop_back();
                else {
                    current->prev->next = current->next;
                    current->next->prev = current->prev;
                    delete current;
                    --len;
                }
            }
            current = current->next;
        }
    }

    template<typename T>
    void List<T>::insert(const T &value, const size_t pos) {
        if (pos > len) throw std::out_of_range("Index out of range");
        if (pos == 0) push_front(value);
        else if (pos == len) push_back(value);
        else {
            auto node = new Node(value);
            auto current = head;
            for (size_t i = 0; i < pos; ++i)
                current = current->next;
            current->prev->next = node;
            node->prev = current->prev;
            node->next = current;
            current->prev = node;
            ++len;
        }
    }

    template<typename T>
    size_t List<T>::size() const {
        return len;
    }

    template<typename T>
    void List<T>::clear() {
        while (head) {
            auto t = head->next;
            delete head;
            head = t;
        }
        tail = nullptr;
        len = 0;
    }

    template<typename T>
    typename List<T>::iterator List<T>::begin() {
        return iterator(head);
    }

    template<typename T>
    typename List<T>::iterator List<T>::end() {
        return iterator(nullptr);
    }

    template<typename T>
    typename List<T>::reverse_iterator List<T>::rbegin() {
        return reverse_iterator(tail);
    }

    template<typename T>
    typename List<T>::reverse_iterator List<T>::rend() {
        return reverse_iterator(nullptr);
    }

    template<typename T>
    typename List<T>::const_iterator List<T>::cbegin() const {
        return const_iterator(head);
    }

    template<typename T>
    typename List<T>::const_iterator List<T>::cend() const {
        return const_iterator(nullptr);
    }

    template<typename T>
    typename List<T>::const_reverse_iterator List<T>::crbegin() const {
        return const_reverse_iterator(tail);
    }

    template<typename T>
    typename List<T>::const_reverse_iterator List<T>::crend() const {
        return const_reverse_iterator(nullptr);
    }
}


#endif //LIST_H
