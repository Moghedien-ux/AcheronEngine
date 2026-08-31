#include <DX3D/Core/Logger.h>
#include <iostream>

dx3d::Logger::Logger(LogLevel logLevel): m_logLevel(logLevel)
{
    std::clog << "Game Engine" << '\n';
    std::clog << "-----------" << '\n';
}

dx3d::Logger::~Logger()
{
}

void dx3d::Logger::log(LogLevel level, const char* message)
{
    auto logLevelToString = [](LogLevel level)
    {
        switch (level)
        {
            case dx3d::Logger::LogLevel::Error: return "ERROR";
            case dx3d::Logger::LogLevel::Warning: return "WARNING";
            case dx3d::Logger::LogLevel::Info: return "INFO";
            default: return "UNKNOWN";
        }
    };


    if (level > m_logLevel) return;

    std::clog << "[DX3D" << logLevelToString(level) << "]: " << message << "\n";

}
