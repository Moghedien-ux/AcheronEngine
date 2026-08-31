#pragma once
#include <DX3D/Core/Core.h>
#include <iostream>
#include <string>

namespace dx3d
{
    class Logger
    {
    public:
        enum class LogLevel
        {
            Debug = 0,
            Info,
            Warning,
            Error
        };

        explicit Logger(LogLevel level = LogLevel::Info) : m_logLevel(level) {}

        void log(LogLevel level, const std::string& message)
        {
            if (level < m_logLevel) return;

            switch (level)
            {
                case LogLevel::Debug:
                    std::cout << "[DEBUG] " << message << std::endl;
                    break;
                case LogLevel::Info:
                    std::cout << "[INFO] " << message << std::endl;
                    break;
                case LogLevel::Warning:
                    std::cout << "[WARNING] " << message << std::endl;
                    break;
                case LogLevel::Error:
                    std::cerr << "[ERROR] " << message << std::endl;
                    break;
            }
        }

        void logInfo(const std::string& msg) { log(LogLevel::Info, msg); }
        void logWarning(const std::string& msg) { log(LogLevel::Warning, msg); }
        void logError(const std::string& msg) { log(LogLevel::Error, msg); }

    private:
        LogLevel m_logLevel{ LogLevel::Info };
    };

#define DX3DLogInfo(msg) dx3d::GetGlobalLogger().logInfo(msg)
#define DX3DLogWarning(msg) dx3d::GetGlobalLogger().logWarning(msg)
#define DX3DLogError(msg) dx3d::GetGlobalLogger().logError(msg)

    Logger& GetGlobalLogger();
}
