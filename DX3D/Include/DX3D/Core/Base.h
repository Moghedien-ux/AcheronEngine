#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>

namespace dx3d
{
    struct BaseDesc
    {
        Logger& logger;
    };

    class Base
    {
    public:
        explicit Base(const BaseDesc& desc) : m_logger(desc.logger) {}
        virtual ~Base() = default;

        Base(const Base&) = delete;
        Base& operator=(const Base&) = delete;

    protected:
        Logger& m_logger;
    };
}
