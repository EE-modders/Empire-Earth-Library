#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace eelib {
// LoggerManager Class
// We need to don't expose spdlog to the user, so we create a wrapper class.

class Logger {
private:
    static std::shared_ptr<spdlog::logger> _libLogger;

public:
    static void Init(const char* name);
    static void Uninit();

    template <typename... Args>
    static void Trace(const char* fmt, const Args&... args)
    {
        _libLogger->trace(fmt, args...);
    }

    template <typename... Args>
    static void Debug(const char* fmt, const Args&... args)
    {
        _libLogger->debug(fmt, args...);
    }

    template <typename... Args>
    static void Info(const char* fmt, const Args&... args)
    {
        _libLogger->info(fmt, args...);
    }

    template <typename... Args>
    static void Warn(const char* fmt, const Args&... args)
    {
        _libLogger->warn(fmt, args...);
    }

    template <typename... Args>
    static void Error(const char* fmt, const Args&... args)
    {
        _libLogger->error(fmt, args...);
    }

    template <typename... Args>
    static void Critical(const char* fmt, const Args&... args)
    {
        _libLogger->critical(fmt, args...);
    }
};
}