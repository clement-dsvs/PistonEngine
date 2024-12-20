//
// Created by cleme on 17/12/2024.
//

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <deque>
#include <mutex>

namespace piston {
    template<typename T>
    class BlockingQueue {
    private:
        std::mutex m_mutex;
        std::condition_variable m_condition;
        std::deque<T> m_queue;
    public:
        void push(T const& value);

        T pop();
    };

    template<typename T>
    void BlockingQueue<T>::push(T const &value)
    {
        {
            std::unique_lock<std::mutex> l_lock(this->m_mutex);
            m_queue.push_front(value);
        }
        this->m_condition.notify_one();
    }

    template<typename T>
    T BlockingQueue<T>::pop() {
        std::unique_lock<std::mutex> l_lock(this->m_mutex);
        m_condition.wait(l_lock, [this] { return !m_queue.empty(); });
        T rc(std::move(this->m_queue.back()));
        this->m_queue.pop_back();
        return rc;
    }
} // piston

#endif //BLOCKINGQUEUE_H
