#pragma once

#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

class DebugLogger {
   public:
    static DebugLogger& instance() {
        static DebugLogger inst;
        return inst;
    }

    bool open(const std::string& filepath) {
        std::lock_guard<std::mutex> lock(mutex_);
        file_.open(filepath, std::ios::out | std::ios::app);
        return file_.is_open();
    }

    void log(const std::string& level, const std::string& file, int line,
             const std::string& func, const std::string& msg) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto& out =
            file_.is_open() ? static_cast<std::ostream&>(file_) : std::cerr;
        out << "[" << timestamp() << "] "
            << "[" << level << "] " << file << ":" << line << " (" << func
            << ") " << msg << "\n";
        out.flush();
    }

    ~DebugLogger() {
        if (file_.is_open()) file_.close();
    }

   private:
    DebugLogger() = default;
    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::ostringstream ss;
        ss << std::put_time(std::localtime(&t), "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }

    std::ofstream file_;
    std::mutex mutex_;
};

// Macros
#ifdef ENABLE_DEBUG_LOG
#define LOG_DEBUG(msg) \
    DebugLogger::instance().log("DEBUG", __FILE__, __LINE__, __func__, msg)
#else
#define LOG_DEBUG(msg) \
    do {               \
    } while (0)
#endif

#define LOG_INFO(msg) \
    DebugLogger::instance().log("INFO", __FILE__, __LINE__, __func__, msg)
#define LOG_WARN(msg) \
    DebugLogger::instance().log("WARN", __FILE__, __LINE__, __func__, msg)
#define LOG_ERROR(msg) \
    DebugLogger::instance().log("ERROR", __FILE__, __LINE__, __func__, msg)
