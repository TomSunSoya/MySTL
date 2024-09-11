#ifndef MYSTL_CONCURRENTDEQUE_H
#define MYSTL_CONCURRENTDEQUE_H

#include "../Deque.h"

#include <mutex>
#include <condition_variable>

namespace MySTL {
    template<typename T>
    class ConcurrentDeque {
    public:
        ConcurrentDeque() = default;

        ~ConcurrentDeque() = default;

        ConcurrentDeque(const ConcurrentDeque &) = delete;

        ConcurrentDeque &operator=(const ConcurrentDeque &) = delete;

        void push_front(const T &value);

        void push_front(T &&value);

        void push_back(const T &value);

        void push_back(T &&value);

        bool try_pop_front(T &value);

        bool try_pop_back(T &value);

        void pop_front();

        void pop_back();

        bool empty() const;

        size_t size() const;

        void clear();

        const T &front() const;

        const T &back() const;

    private:
        mutable std::mutex mutex_;
        Deque<T> deque_;
        std::condition_variable cond_not_empty_;
    };

    template<typename T>
    void ConcurrentDeque<T>::push_front(const T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        deque_.push_front(value);
        lock.unlock();
        cond_not_empty_.notify_one();
    }

    template<typename T>
    void ConcurrentDeque<T>::push_front(T &&value) {
        std::unique_lock<std::mutex> lock(mutex_);
        deque_.push_front(std::move(value));
        lock.unlock();
        cond_not_empty_.notify_one();
    }

    template<typename T>
    void ConcurrentDeque<T>::push_back(const T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        deque_.push_back(value);
        lock.unlock();
        cond_not_empty_.notify_one();
    }

    template<typename T>
    void ConcurrentDeque<T>::push_back(T &&value) {
        std::unique_lock<std::mutex> lock(mutex_);
        deque_.push_back(std::move(value));
        lock.unlock();
        cond_not_empty_.notify_one();
    }

    template<typename T>
    bool ConcurrentDeque<T>::try_pop_front(T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (deque_.empty()) {
            return false;
        }
        value = std::move(deque_.front());
        deque_.pop_front();
        return true;
    }

    template<typename T>
    bool ConcurrentDeque<T>::try_pop_back(T &value) {
        std::unique_lock<std::mutex> lock(mutex_);
        if (deque_.empty()) {
            return false;
        }
        value = std::move(deque_.back());
        deque_.pop_back();
        return true;
    }

    template<typename T>
    void ConcurrentDeque<T>::pop_front() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_not_empty_.wait(lock, [this]() { return !deque_.empty(); });
        deque_.pop_front();
    }

    template<typename T>
    void ConcurrentDeque<T>::pop_back() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_not_empty_.wait(lock, [this]() { return !deque_.empty(); });
        deque_.pop_back();
    }

    template<typename T>
    bool ConcurrentDeque<T>::empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deque_.empty();
    }

    template<typename T>
    size_t ConcurrentDeque<T>::size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deque_.size();
    }

    template<typename T>
    void ConcurrentDeque<T>::clear() {
        std::unique_lock<std::mutex> lock(mutex_);
        deque_.clear();
    }

    template<typename T>
    const T &ConcurrentDeque<T>::front() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deque_.front();
    }

    template<typename T>
    const T &ConcurrentDeque<T>::back() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return deque_.back();
    }

}


#endif //MYSTL_CONCURRENTDEQUE_H
