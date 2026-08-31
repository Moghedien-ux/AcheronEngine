#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>

namespace dx3d
{
    using i8  = int8_t;
    using i16 = int16_t;
    using i32 = int32_t;
    using i64 = int64_t;

    using u8  = uint8_t;
    using u16 = uint16_t;
    using u32 = uint32_t;
    using u64 = uint64_t;

    using f32 = float;
    using f64 = double;

    using usize = size_t;

    struct Point
    {
        i32 x{ 0 };
        i32 y{ 0 };
    };

    struct Rect
    {
        i32 width{ 0 };
        i32 height{ 0 };
    };

    class DX3DException : public std::runtime_error
    {
    public:
        explicit DX3DException(const char* msg) : std::runtime_error(msg) {}
        explicit DX3DException(const std::string& msg) : std::runtime_error(msg) {}
    };
}
