#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/GraphicsUtils.h>
#include <d3dcompiler.h>

dx3d::ShaderBinary::ShaderBinary(const ShaderCompileDesc& desc, const GraphicsResourceDesc& gDesc):
    GraphicsResource(gDesc), m_type(desc.shaderType)
{
    if (!desc.sharedSourceName) DX3DLogThrowInvalidArgs("No shader source name provided.");
    if (!desc.sharedSourceCode) DX3DLogThrowInvalidArgs("No shader source code provided.");
    if (!desc.shaderCodeSourceSize) DX3DLogThrowInvalidArgs("No shader code source size provided.");
    if (!desc.shaderEntryPoint) DX3DLogThrowInvalidArgs("No shader entry point provided.");

    UINT compileFlags{};

#ifdef _DEBUG
    compileFlags |= D3DCOMPILE_DEBUG;
#endif

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
    DX3DGraphicsCheckShaderCompile(
        D3DCompile(
            desc.sharedSourceCode,
            desc.shaderCodeSourceSize,
            desc.sharedSourceName,
            nullptr,
            nullptr,
            desc.shaderEntryPoint,
            dx3d::GraphicsUtils::GetShaderModelTarget(desc.shaderType),
            compileFlags,
            0,
            &m_blob,
            &errorBlob),
        errorBlob.Get(),
        "D3DCompile failed."
    );
}

dx3d::ShaderBinaryData dx3d::ShaderBinary::getData() const noexcept
{
    return
    {
        m_blob->GetBufferPointer(),
        m_blob->GetBufferSize()
    };
}

dx3d::ShaderType dx3d::ShaderBinary::getType() const noexcept
{
    return m_type;
}