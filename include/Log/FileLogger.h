#ifndef MYSTL_FILELOGGER_H
#define MYSTL_FILELOGGER_H

#include "Logger.h"
#include <iomanip>
#include <fstream>
#include <sstream>
#include <mutex>

namespace MySTL {

    class FileLogger : public Logger {
    public:
        explicit FileLogger(const std::string &filename) : outFile(filename, std::ios::app), logLevel(LogLevel::DEBUG) {
            if (!outFile.is_open())
                throw std::runtime_error("Failed to open file: " + filename);
        }

        ~FileLogger() override {
            if (outFile.is_open())
                outFile.close();
        }

        void log(LogLevel level, String &msg) override {
            std::lock_guard<std::mutex> lock(mtx);
            if (level >= logLevel)
                outFile << "[" << getCurrentTime() << "] "
                        << toString(level) << " " << msg.c_str() << std::endl;
        }

        void setLevel(LogLevel level) override {
            logLevel = level;
        }

    private:
        std::ofstream outFile;
        std::mutex mtx;
        LogLevel logLevel;

        static std::string toString(LogLevel level) {
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

}


#endif //MYSTL_FILELOGGER_H
