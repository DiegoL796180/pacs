#pragma once

#include <atomic>
#include <functional>
#include <vector>
#include <thread>        // Necesario para std::thread y std::this_thread
#include <chrono>        // Necesario para std::chrono::milliseconds

#include <join_threads.hpp>
#include <threadsafe_queue.hpp>

class thread_pool
{
    std::atomic<bool> _done;
    using task_type = void();
    threadsafe_queue<std::function<task_type>> _queue;
    std::vector<std::thread> _threads;
    join_threads _joiner;
    std::atomic<size_t> _pending_tasks;

    void worker_thread()
    {
        while (!_done) {
            std::function<task_type> task;
            if (_queue.try_pop(task)) { 
                task(); 
                _pending_tasks--; 
            } else {
                std::this_thread::yield();
            }
        }
    }

public:
    thread_pool(size_t num_threads = std::thread::hardware_concurrency())
        : _done(false), _joiner(_threads), _pending_tasks(0)
    {
        try {
            for (size_t i = 0; i < num_threads; ++i) {
                _threads.emplace_back(&thread_pool::worker_thread, this);
            }
        } catch (...) {
            _done = true;
            throw;
        }
    }

    ~thread_pool()
    {
        wait();
        _done = true; 

    }

    void wait()
    {
        while (_pending_tasks > 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }

    template<typename F>
    void submit(F f)
    {
        _pending_tasks++;
        _queue.push(std::function<task_type>(std::move(f)));
    }
};