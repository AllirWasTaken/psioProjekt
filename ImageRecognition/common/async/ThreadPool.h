#pragma once

#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <stop_token>
#include <thread>
#include <vector>

class ThreadPool
{
public:
    using Task = std::function<void(std::stop_token)>; 

    ThreadPool(const std::size_t numberOfThreads = 10);
    ~ThreadPool();

    void start();
    void stop();

    void schedule(Task&& task);

private:
    void loop(std::stop_token stopToken);
    Task getTask(std::stop_token);

    std::mutex mutex;
    std::condition_variable condition;
    std::queue<Task> tasks;
    std::vector<std::jthread> threads;
};