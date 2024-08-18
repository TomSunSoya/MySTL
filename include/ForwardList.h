#ifndef MYSTL_FORWARDLIST_H
#define MYSTL_FORWARDLIST_H

#include <initializer_list>
#include <stdexcept>

namespace MySTL {

    template<typename T>
    class ForwardList {
    public:
        ForwardList();
        ~ForwardList();

        ForwardList(const ForwardList &other);
        ForwardList &operator=(const ForwardList &other);
        ForwardList(ForwardList &&other) noexcept;
        ForwardList &operator=(ForwardList &&other) noexcept;
        ForwardList(std::initializer_list<T> list);

        bool empty();
        size_t size();
        void clear();

        T& front();
        T& back();
        void push_front(const T &value);
        void push_front(T &&value);
        void pop_front();
        void insert_after(const T &value, size_t pos);
        void insert_after(T &&value, size_t pos);

        template<typename... Args>
        void emplace_after(size_t pos, Args... args);

        void erase_after(size_t pos);
        void reverse();
        void resize(size_t sz);


    private:
        struct Node {
            T data;
            Node *next;

            explicit Node(const T &d = T(), Node *n = nullptr) : data(d), next(n) {}
            explicit Node(T&& d, Node *n = nullptr) : data(std::move(d)), next(n) {}
        };

        Node *head;
        size_t len;
    };

    template<typename T>
    ForwardList<T>::ForwardList() : head(nullptr), len(0) {

    }

    template<typename T>
    ForwardList<T>::~ForwardList() {
        clear();
    }

    template<typename T>
    ForwardList<T>::ForwardList(const ForwardList &other) {
        clear();
        auto p = other.head;
        auto q = head;
        Node* pre = nullptr;
        while (q) {
            q = new Node(p->data);
            if (head == nullptr)
                head = q;
            else
                pre->next = q;
            pre = q;
            p = p->next;
        }
        len = other.len;
    }

    template<typename T>
    ForwardList<T> &ForwardList<T>::operator=(const ForwardList &other) {
        if (this == &other)
            return *this;
        clear();
        auto p = other.head;
        auto q = head;
        Node* pre = nullptr;
        while (q) {
            q = new Node(p->data);
            if (head == nullptr)
                head = q;
            else
                pre->next = q;
            pre = q;
            p = p->next;
        }
        len = other.len;
        return *this;
    }

    template<typename T>
    ForwardList<T>::ForwardList(ForwardList &&other) noexcept {
        if (this == &other)
            return *this;
        clear();
        head = other.head;
        len = other.len;
        other.head = nullptr;
        other.len = 0;
        return *this;
    }

    template<typename T>
    ForwardList<T> &ForwardList<T>::operator=(ForwardList &&other) noexcept {
        if (this == &other)
            return *this;
        clear();
        head = other.head;
        len = other.len;
        other.head = nullptr;
        other.len = 0;
        return *this;
    }

    template<typename T>
    ForwardList<T>::ForwardList(std::initializer_list<T> list) {
        auto p = list.begin();
        auto q = head;
        Node* pre = nullptr;
        while (p != list.end()) {
            q = new Node(*p);
            if (head == nullptr)
                head = q;
            else
                pre->next = q;
            pre = q;
            ++p;
        }
        len = list.size();
    }

    template<typename T>
    bool ForwardList<T>::empty() {
        return len == 0;
    }

    template<typename T>
    size_t ForwardList<T>::size() {
        return len;
    }

    template<typename T>
    void ForwardList<T>::clear() {
        while (head)
            pop_front();
    }

    template<typename T>
    T &ForwardList<T>::front() {
        if (head == nullptr)
            throw std::out_of_range("ForwardList is empty");
        return head->data;
    }

    template<typename T>
    T &ForwardList<T>::back() {
        if (head == nullptr)
            throw std::out_of_range("ForwardList is empty");
        auto p = head;
        while (p->next)
            p = p->next;
        return p->data;
    }

    template<typename T>
    void ForwardList<T>::push_front(const T &value) {
        head = new Node(value, head);
        ++len;
    }

    template<typename T>
    void ForwardList<T>::push_front(T &&value) {
        head = new Node(std::move(value), head);
        ++len;
    }

    template<typename T>
    void ForwardList<T>::pop_front() {
        if (head == nullptr) return;
        auto p = head;
        head = head->next;
        delete p;
    }

    template<typename T>
    void ForwardList<T>::insert_after(const T &value, size_t pos) {
        int index = 0;
        auto p = head;
        Node* pre = nullptr;
        while (index < pos) {
            ++index;
            if (p == nullptr)
                throw std::out_of_range("Index out of range");
            pre = p;
            p = p->next;
        }
        auto q = new Node(value, p);
        pre->next = q;
    }

    template<typename T>
    void ForwardList<T>::insert_after(T &&value, size_t pos) {
        size_t index = 0;
        auto p = head;
        Node* pre = nullptr;
        while (index < pos) {
            ++index;
            if (p == nullptr)
                throw std::out_of_range("Index out of range");
            pre = p;
            p = p->next;
        }
        auto q = new Node(std::move(value), p);
        pre->next = q;
    }

    template<typename T>
    template<typename... Args>
    void ForwardList<T>::emplace_after(size_t pos, Args... args) {
        size_t index = 0;
        auto p = head;
        Node* pre = nullptr;
        while (index < pos) {
            ++index;
            if (p == nullptr)
                throw std::out_of_range("Index out of range");
            pre = p;
            p = p->next;
        }
        auto q = new Node(T(std::forward<Args>(args)...), p);
        pre->next = q;
    }

    template<typename T>
    void ForwardList<T>::erase_after(size_t pos) {
        size_t index = 0;
        auto p = head;
        Node* pre = nullptr;
        while (index < pos) {
            ++index;
            if (p == nullptr)
                throw std::out_of_range("Index out of range");
            pre = p;
            p = p->next;
        }
        pre->next = p->next;
        delete p;
    }

    template<typename T>
    void ForwardList<T>::reverse() {
        Node* pre = nullptr;
        auto p = head;
        while (p) {
            auto q = p->next;
            p->next = pre;
            pre = p;
            p = q;
        }
        head = pre;
    }

}


#endif //MYSTL_FORWARDLIST_H
