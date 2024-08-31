#ifndef MYSTL_CONSOLELOGGER_H
#define MYSTL_CONSOLELOGGER_H

#include "Logger.h"
#include <iostream>
#include <mutex>

namespace MySTL {
    class ConsoleLogger : public Logger {
    public:
        ConsoleLogger() : logLevel(LogLevel::DEBUG) {}

        void log(LogLevel level, String &msg) override {
            std::lock_guard<std::mutex> lock(mtx);
            if (level >= logLevel) {
                std::cout << "[" << toString(level) << "] " << msg.c_str() << std::endl;
            }
        }

        void setLevel(LogLevel level) override {
            logLevel = level;
        }

    private:
        LogLevel logLevel;
        std::mutex mtx;

        static String toString(LogLevel level) {
            switch (level) {
                case LogLevel::DEBUG:
                    return "DEBUG";
                case LogLevel::INFO:
                    return "INFO";
                case LogLevel::WARN:
                    return "WARN";
                case LogLevel::ERROR:
                    return "ERROR";
                case LogLevel::FATAL:
                    return "FATAL";
                default:
                    return "UNKNOWN";
            }
        }
    };

#define LOG_DEBUG(logger, message) logger.log(message, LogLevel::DEBUG)
#define LOG_INFO(logger, message) logger.log(message, LogLevel::INFO)
#define LOG_WARNING(logger, message) logger.log(message, LogLevel::WARNING)
#define LOG_ERROR(logger, message) logger.log(message, LogLevel::ERROR)
#define LOG_FATAL(logger, message) logger.log(message, LogLevel::FATAL)
}


#endif //MYSTL_CONSOLELOGGER_H
