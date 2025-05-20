#include <queue>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>

class TaskQueue {
public:
    void pushTask(const std::function<void()>& task) {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(task);
        m_condition.notify_one();
    }

    void processTasks() {
        std::function<void()> task;
        while (true) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);
            m_condition.wait(lock, [this] { return !m_queue.empty(); });
            task = m_queue.front();
            m_queue.pop();
        }
        task();
        }
    }

private:
    std::queue<std::function<void()>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};
