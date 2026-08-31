#pragma once
#include <stdexcept>
#include <memory>

namespace dx3d
{
    class Logger;
    class SwapChain;
    class DeviceContext;

    class Base;
    class Window;
    class Display;
    class Game;

    class GraphicsEngine;
    class GraphicsDevice;
    class ShaderBinary;
    class GraphicsPipelineState;
    class VertexBuffer;
    class IndexBuffer;
    class ConstantBuffer;

    class Camera;
    class InputSystem;

    class GameObject;
    class PhysicsEngine;
    class RigidBody;

    using i32 = int;
    using ui32 = unsigned int;
    using f32 = float;
    using d64 = double;

    using SwapChainPtr = std::shared_ptr<SwapChain>;
    using DeviceContextPtr = std::shared_ptr<DeviceContext>;
    using ShaderBinaryPtr = std::shared_ptr<ShaderBinary>;
    using GraphicsPilepineStatePtr = std::shared_ptr<GraphicsPipelineState>;
    using VertexBufferPtr = std::shared_ptr<VertexBuffer>;
    using IndexBufferPtr = std::shared_ptr<IndexBuffer>;
    using ConstantBufferPtr = std::shared_ptr<ConstantBuffer>;
    using CameraPtr = std::shared_ptr<Camera>;
    using InputSystemPtr = std::shared_ptr<InputSystem>;
}