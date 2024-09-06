#pragma once
#ifndef _THREAD_POOL_
#define _THREAD_POOL_
#include <core/core.h>

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>
#include <atomic>
namespace luna
{
    class threadPool {
    public:
        threadPool(size_t threads);
        ~threadPool();

        std::future<void> enqueue(std::function<void()> func) {
            // Create a packaged task with the callable object
            auto task = std::make_shared<std::packaged_task<void()>>(std::move(func));

            std::future<void> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queue_mutex);

                if (stop.load()) {
                    throw std::runtime_error("enqueue on stopped ThreadPool");
                }

                // Wrap the task into a std::function
                tasks.push([task]() { (*task)(); });
            }
            condition.notify_one();
            return res;
        }
        void sync(); //sync function to block and wait for all tasks
    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queue_mutex;
        std::condition_variable condition;
        std::condition_variable finished;

        std::atomic<bool> stop;
        std::atomic<size_t> active_tasks;

    };

    _ALWAYS_INLINE_ threadPool::threadPool(size_t threads)
        : stop(false), active_tasks(0) {
        for (size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                for (;;) {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->condition.wait(lock, [this] {
                            return this->stop.load() || !this->tasks.empty();
                            });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }
                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        active_tasks++;
                    }

                    task();

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        active_tasks--;
                        if (active_tasks == 0 && tasks.empty()) {
                            finished.notify_all();
                        }
                    }
                }
                });
        }
    }

    _ALWAYS_INLINE_ threadPool::~threadPool() {
        stop.store(true);
        condition.notify_all();
        for (std::thread& worker : workers) {
            worker.join();
        }
    }
   
    _ALWAYS_INLINE_ void threadPool::sync() {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            stop.store(true); // Temporarily block further task submissions
            condition.notify_all(); // Wake up all threads to finish queued tasks
            finished.wait(lock, [this] {
                return tasks.empty() && active_tasks == 0;
                });
            stop.store(false); // Allow task submission again after sync
        }
    }
}
#endif // _THREAD_POOL_