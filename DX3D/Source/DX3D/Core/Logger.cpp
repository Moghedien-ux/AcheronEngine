#include <DX3D/Core/Logger.h>

namespace
{
    dx3d::Logger g_defaultLogger(dx3d::Logger::LogLevel::Info);
}

dx3d::Logger& dx3d::GetGlobalLogger()
{
    return g_defaultLogger;
}
