//
// Created by cleme on 17/12/2024.
//

#ifndef LOGGER_H
#define LOGGER_H

#include <thread>
#include <utility>

#include "piston/core/containers/BlockingQueue.hpp"

namespace piston {

    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    typedef struct Log {
        LogLevel m_logLevel;
        std::string m_message;
        time_t m_creationDate;

        Log(const LogLevel a_logLevel, std::string  a_message) : m_logLevel(a_logLevel), m_message(std::move(a_message)) {
            m_creationDate = time(nullptr);
        }
    }Log;

    class Logger {
    private:
        static Logger* m_pInstance;
        static std::mutex m_mutex;

        std::thread m_thread;
        std::atomic_bool m_running;

        BlockingQueue<Log> m_logQueue;

        void run();
        Logger();
        ~Logger() = default;

    public:

        Logger(Logger& other) = delete;
        void operator=(const Logger&) = delete;

        static Logger* os_GetInstance();

        void Info(const std::string& a_message);

        void start();
        void stop(bool wait = false);
    };
}

#endif //LOGGER_H
