#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template<typename T>
class threadsafe_queue
{
private:
    mutable std::mutex _mut;
    std::queue<T> _data;
    std::condition_variable _cv;

public:
    threadsafe_queue() {}

    threadsafe_queue(const threadsafe_queue& other)
    {
        std::lock_guard<std::mutex> lk(other._mut);
        _data = other._data;
        _cv = other._cv;
    }
    threadsafe_queue& operator=(const threadsafe_queue&) = delete;

    void push(T new_value)
    {
        std::lock_guard<std::mutex> lk(_mut); 
        _data.push(std::move(new_value));     
        _cv.notify_one();
    }

    bool try_pop(T& value)
    {
        std::lock_guard<std::mutex> lk(_mut); 
        if (_data.empty()) {
            return false; 
        }

        value = std::move(_data.front());
        _data.pop();
        return true;

    }

    void wait_and_pop(T& value)
    {

        std::unique_lock<std::mutex> lk(_mut);
        _cv.wait(lk, [this]{ return !_data.empty(); });
        value = std::move(_data.front());
        _data.pop();
        
    }

    std::shared_ptr<T> wait_and_pop()
    {
        std::unique_lock<std::mutex> lk(_mut);
        _cv.wait(lk, [this]{ return !_data.empty(); });
        std::shared_ptr<T> res = std::make_shared<T>(std::move(_data.front()));
        _data.pop();
        return res;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lk(_mut);
        return _data.empty();
    }
};