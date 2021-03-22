#include "ThreadPool.h"
void ThreadPool::Start(std::size_t numThreads)
{
    for (auto i = 0u; i < numThreads; ++i)
    {
        m_threads.emplace_back([=] {
            while (true)
            {
                Task task;

                {
                    std::unique_lock<std::mutex> lock{ m_event_mutex };

                    m_event_happened.wait(lock, [=] { return m_is_stopped || !m_tasks.empty(); });

                    if (m_is_stopped && m_tasks.empty())
                        break;

                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                task();
            }
        });
    }
}

void ThreadPool::Stop() 
{
    {
        std::unique_lock<std::mutex> lock{ m_event_mutex };
        m_is_stopped = true;
    }

    m_event_happened.notify_all();

    for (auto& thread : m_threads)
        thread.join();
}

template<typename T, typename ...Args>
inline auto ThreadPool::DoTask(T&& task, Args&& ...args) -> std::future<typename std::result_of<T(Args ...)>::type>
{
    using return_type = typename std::result_of<T(Args...)>::type;
    auto wrapper = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<T>(task), std::forward<Args>(args)...));//initialize wrapper for input function

    {//this scope is for synchronization
        std::unique_lock<std::mutex> lock{ m_event_mutex };
        m_tasks.emplace([wrapper] {
            (*wrapper)();
        });
    }

    m_event_happened.notify_one();
    return wrapper->get_future();
}