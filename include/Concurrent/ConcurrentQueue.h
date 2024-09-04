#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include "../Queue.h"
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <stdexcept>

namespace MySTL {

    template<typename T>
    class ConcurrentQueue {
    public:
        // 构造函数：无界队列
        ConcurrentQueue() = default;

        // 构造函数：有界队列，指定容量
        explicit ConcurrentQueue(size_t capacity) : capacity_(capacity), is_bounded_(true) {}

        // 禁用拷贝构造和赋值
        ConcurrentQueue(const ConcurrentQueue &) = delete;

        ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;

        // 入队：阻塞版本
        void push(const T &item) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_not_full_.wait(lock, [this]() { return !is_bounded_ || queue_.size() < capacity_; });
            queue_.push(item);
            lock.unlock();
            cond_not_empty_.notify_one();
        }

        void push(T &&item) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_not_full_.wait(lock, [this]() { return !is_bounded_ || queue_.size() < capacity_; });
            queue_.push(std::move(item));
            lock.unlock();
            cond_not_empty_.notify_one();
        }

        // 尝试入队：非阻塞版本
        bool try_push(const T &item) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (is_bounded_ && queue_.size() >= capacity_) {
                return false;
            }
            queue_.push(item);
            lock.unlock();
            cond_not_empty_.notify_one();
            return true;
        }

        bool try_push(T &&item) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (is_bounded_ && queue_.size() >= capacity_) {
                return false;
            }
            queue_.push(std::move(item));
            lock.unlock();
            cond_not_empty_.notify_one();
            return true;
        }

        // 带超时的入队
        template<typename Rep, typename Period>
        bool push_with_timeout(const T &item, const std::chrono::duration<Rep, Period> &timeout_duration) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!cond_not_full_.wait_for(lock, timeout_duration,
                                         [this]() { return !is_bounded_ || queue_.size() < capacity_; })) {
                return false; // 超时
            }
            queue_.push(item);
            lock.unlock();
            cond_not_empty_.notify_one();
            return true;
        }

        // 出队：阻塞版本
        void pop(T &item) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_not_empty_.wait(lock, [this]() { return !queue_.empty(); });
            item = std::move(queue_.front());
            queue_.pop();
            lock.unlock();
            cond_not_full_.notify_one();
        }

        // 尝试出队：非阻塞版本
        bool try_pop(T &item) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (queue_.empty()) {
                return false;
            }
            item = std::move(queue_.front());
            queue_.pop();
            lock.unlock();
            cond_not_full_.notify_one();
            return true;
        }

        // 带超时的出队
        template<typename Rep, typename Period>
        bool pop_with_timeout(T &item, const std::chrono::duration<Rep, Period> &timeout_duration) {
            std::unique_lock<std::mutex> lock(mutex_);
            if (!cond_not_empty_.wait_for(lock, timeout_duration, [this]() { return !queue_.empty(); })) {
                return false; // 超时
            }
            item = std::move(queue_.front());
            queue_.pop();
            lock.unlock();
            cond_not_full_.notify_one();
            return true;
        }

        // 获取队列大小
        size_t size() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size();
        }

        // 检查队列是否为空
        bool empty() const {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.empty();
        }

        // 获取容量（仅当有界时有效）
        size_t capacity() const {
            if (!is_bounded_) {
                throw std::runtime_error("Unbounded queue does not have a capacity.");
            }
            return capacity_;
        }

        // 检查队列是否已满（仅当有界时有效）
        bool is_full() const {
            if (!is_bounded_) {
                return false;
            }
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.size() >= capacity_;
        }

        T &front() {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.front();
        }

        T &back() {
            std::lock_guard<std::mutex> lock(mutex_);
            return queue_.back();
        }

    private:
        mutable std::mutex mutex_;
        std::condition_variable cond_not_empty_;
        std::condition_variable cond_not_full_;
        Queue<T> queue_;

        size_t capacity_ = 0;
        bool is_bounded_ = false;
    };

} // namespace my_stl

#endif // CONCURRENT_QUEUE_H
