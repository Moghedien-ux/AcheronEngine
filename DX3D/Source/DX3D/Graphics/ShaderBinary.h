#pragma once
#include <DX3D/Core/Core.h>
#include <d3dcommon.h>
#include <wrl.h>

namespace dx3d
{
    class ShaderBinary
    {
    public:
        explicit ShaderBinary(Microsoft::WRL::ComPtr<ID3DBlob> blob) : m_blob(blob) {}
        const void* getBufferPointer() const { return m_blob ? m_blob->GetBufferPointer() : nullptr; }
        size_t getBufferSize() const { return m_blob ? m_blob->GetBufferSize() : 0; }
        ID3DBlob* getBlob() const { return m_blob.Get(); }

    private:
        Microsoft::WRL::ComPtr<ID3DBlob> m_blob;
    };
}
