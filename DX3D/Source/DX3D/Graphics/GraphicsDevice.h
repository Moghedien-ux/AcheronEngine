#pragma once
#include <DX3D/Core/Base.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/DeviceContext.h> // Include complete type
#include <d3d11.h>
#include <wrl.h>
#include <string>
#include <memory>

namespace dx3d
{
    struct GraphicsDeviceDesc
    {
        BaseDesc base;
    };

    class GraphicsDevice final : public Base
    {
    public:
        explicit GraphicsDevice(const GraphicsDeviceDesc& desc);
        virtual ~GraphicsDevice() override;

        ID3D11Device& getD3DDevice() { return *m_device.Get(); }
        ID3D11DeviceContext& getD3DDeviceContext() { return *m_context.Get(); }
        DeviceContext& getDeviceContext() { return *m_deviceContext; }

        ShaderBinary compileShader(const std::wstring& filePath, const std::string& entryPoint, const std::string& targetProfile);

    private:
        Microsoft::WRL::ComPtr<ID3D11Device> m_device{};
        Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_context{};
        std::unique_ptr<DeviceContext> m_deviceContext{};
    };
}