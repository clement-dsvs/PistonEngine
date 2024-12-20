//
// Created by cleme on 17/12/2024.
//

#include "piston/core/Logger.hpp"

#include <iostream>
#include <mutex>
#include <utility>

piston::Logger* piston::Logger::m_pInstance{nullptr};
std::mutex piston::Logger::m_mutex;

void piston::Logger::run() {
    while (m_running) {
        Log l_log = m_logQueue.pop();
        std::cout << ctime(&l_log.m_creationDate) << " | " << l_log.m_message << "\n";
    }
    std::cout << std::flush;
    std::cout << "logger has stopped\n";
}

piston::Logger::Logger()
    : m_running(false)
{
}

piston::Logger * piston::Logger::os_GetInstance() {
    std::lock_guard<std::mutex> l_lock(m_mutex);
    if (m_pInstance == nullptr) {
        m_pInstance = new Logger();
    }

    return m_pInstance;
}

void piston::Logger::Info(const std::string& a_message) {
    m_logQueue.push(Log(LogLevel::INFO, a_message));
}

void piston::Logger::start() {
    std::cout << "starting logger\n";
    m_running = true;
    m_thread = std::thread(&Logger::run, this);
}

void piston::Logger::stop(bool wait) {
    std::cout << "stopping logger\n";
    m_running = false;
    if (wait && m_thread.joinable()) m_thread.join();
}
