#pragma once

#include <deque>
#include <mutex>
#include <vector>

template<typename T = char> class Queue final {
  private:
    using lock_t = std::lock_guard<std::mutex>;

    uint64_t max_size{UINT64_MAX};

    std::mutex mtx{};
    std::deque<T> data{};

    bool canPush() {
        const uint64_t size{data.size()};
        return size != UINT64_MAX && size < max_size;
    }

  public:
    Queue() = default;
    Queue(const std::deque<T>& data) : data{data} {};
    Queue(uint64_t max_size) : max_size{max_size} {};
    Queue(const std::deque<T>& data, uint64_t max_size) : data{data}, max_size{max_size} {};

    uint64_t size() {
        lock_t lock(mtx);
        return data.size();
    }

    uint64_t maxSize() {
        lock_t lock(mtx);
        return max_size;
    }

    bool empty() {
        lock_t lock(mtx);
        return data.empty();
    }

    void setMaxSize(uint64_t max_size) {
        lock_t lock(mtx);
        this->max_size = max_size;
    }

    void clear() {
        lock_t lock(mtx);
        data.clear();
    }

    void pushBack(T data) {
        lock_t lock(mtx);
        if (canPush()) {
            this->data.push_back(data);
        }
    }

    void pushFront(T data) {
        lock_t lock(mtx);
        if (canPush()) {
            this->data.push_front(data);
        }
    }

    bool popBack() {
        lock_t lock(mtx);
        if (!data.empty()) {
            data.pop_back();
            return true;
        }
        return false;
    }

    bool popFront() {
        lock_t lock(mtx);
        if (!data.empty()) {
            data.pop_front();
            return true;
        }
        return false;
    }

    T back() {
        lock_t lock(mtx);
        return data.back();
    }

    T front() {
        lock_t lock(mtx);
        return data.front();
    }

    T getAndPopBack() {
        lock_t lock(mtx);
        T value{data.back()};
        data.pop_back();
        return value;
    }

    T getAndPopFront() {
        lock_t lock(mtx);
        T value{data.front()};
        data.pop_front();
        return value;
    }
};
