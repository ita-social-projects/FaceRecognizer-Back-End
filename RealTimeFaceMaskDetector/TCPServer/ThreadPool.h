#pragma once
#include <condition_variable>
#include <functional>
#include <iostream>
#include <future>
#include <vector>
#include <thread>
#include <queue>

class ThreadPool
{
public:
    using Task = std::function<void()>;

    explicit ThreadPool(std::size_t numThreads) { Start(numThreads);}
    ~ThreadPool(){ Stop();}

    template<typename T, typename ...Args>
    auto DoTask(T&& task, Args&& ...args)->std::future<typename std::result_of<T(Args...)>::type>;
   
private:
    std::vector<std::thread> m_threads;
    std::condition_variable m_event_happened;
    std::mutex m_event_mutex;
    std::queue<Task> m_tasks;
    bool m_is_stopped = false;
    void Stop();
    void Start(std::size_t numThreads);
};

