#ifndef MYSTL_LOGGER_H
#define MYSTL_LOGGER_H

#include "../String.h"
#include <iomanip>

namespace MySTL {
    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
    };

    class Logger {
    public:
        virtual ~Logger() = default;

        virtual void log(LogLevel level, String &msg) = 0;

        virtual void setLevel(LogLevel level) = 0;

        static std::string getCurrentTime() {
            std::time_t now = std::time(nullptr);
            std::tm *localTime = std::localtime(&now);
            std::stringstream ss;
            ss << std::put_time(localTime, "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }
    };

}


#endif //MYSTL_LOGGER_H
