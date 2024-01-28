#include "ThreadPool.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

ThreadPool::ThreadPool(const std::size_t numberOfThreads)
    : threads(numberOfThreads)
{
}

ThreadPool::~ThreadPool() { stop(); }

void ThreadPool::start()
{
    std::lock_guard lock { mutex };
    for (auto& thread : threads) {
        assert(!thread.joinable());
        thread = std::jthread(&ThreadPool::loop, this);
    }
}

void ThreadPool::stop()
{
    for (auto& thread : threads) {
        thread.request_stop();
    }
    condition.notify_all();
}

void ThreadPool::schedule(Task&& task)
{
    assert(task);
    std::lock_guard lock(mutex);
    tasks.push(std::move(task));
    condition.notify_one();
}

void ThreadPool::loop(std::stop_token stopToken)
{
    while (!stopToken.stop_requested()) {
        auto task = getTask(stopToken);
        if (stopToken.stop_requested())
            return;
        if (task) {
            try {
                task(stopToken);
            } catch (const std::exception& e) {
                std::cerr << "Unhandled exception in thread pool: "
                          << std::this_thread::get_id() << ": " << e.what()
                          << std::endl;
            } catch (...) {
                std::cerr << "Unknown unhandled exception in thread pool: "
                          << std::this_thread::get_id() << std::endl;
            }
        }
    }
}

ThreadPool::Task ThreadPool::getTask(std::stop_token stopToken)
{
    std::unique_lock lock { mutex };
    condition.wait(lock, [this, stopToken]() {
        return stopToken.stop_requested() || !tasks.empty();
    });
    if (stopToken.stop_requested())
        return nullptr;
    auto task = tasks.front();
    tasks.pop();
    return task;
}