#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/GraphicsLogUtils.h>
#include <d3dcompiler.h>

dx3d::GraphicsDevice::GraphicsDevice(const GraphicsDeviceDesc& desc) :
    Base(desc.base)
{
    UINT createDeviceFlags = 0;
#if defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
    };
    D3D_FEATURE_LEVEL featureLevel;

    DX3DGraphicsLogThrowOnFail(
        D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            createDeviceFlags,
            featureLevels,
            ARRAYSIZE(featureLevels),
            D3D11_SDK_VERSION,
            &m_device,
            &featureLevel,
            &m_context
        ),
        "D3D11CreateDevice failed."
    );

    m_deviceContext = std::make_unique<DeviceContext>(DeviceContextDesc{ { desc.base, *m_device.Get() }, m_context });
}

dx3d::ShaderBinary dx3d::GraphicsDevice::compileShader(
    const std::wstring& filePath,
    const std::string& entryPoint,
    const std::string& targetProfile)
{
    Microsoft::WRL::ComPtr<ID3DBlob> blob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    compileFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

    // Try multiple candidate relative paths so it loads regardless of working directory
    std::vector<std::wstring> candidatePaths = {
        filePath,
        L"DX3D/Shaders/" + filePath,
        L"Shaders/" + filePath,
        L"../DX3D/Shaders/" + filePath,
        L"../../DX3D/Shaders/" + filePath
    };

    size_t lastSlash = filePath.find_last_of(L"/\\");
    if (lastSlash != std::wstring::npos)
    {
        std::wstring filename = filePath.substr(lastSlash + 1);
        candidatePaths.push_back(filename);
        candidatePaths.push_back(L"Shaders/" + filename);
        candidatePaths.push_back(L"DX3D/Shaders/" + filename);
        candidatePaths.push_back(L"../DX3D/Shaders/" + filename);
        candidatePaths.push_back(L"../../DX3D/Shaders/" + filename);
    }

    HRESULT hr = E_FAIL;
    for (const auto& path : candidatePaths)
    {
        errorBlob.Reset();
        hr = D3DCompileFromFile(
            path.c_str(),
            nullptr,
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            entryPoint.c_str(),
            targetProfile.c_str(),
            compileFlags,
            0,
            &blob,
            &errorBlob
        );

        if (SUCCEEDED(hr))
        {
            break;
        }
        else if (hr != HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND) && hr != HRESULT_FROM_WIN32(ERROR_PATH_NOT_FOUND))
        {
            // Found the file, but compilation failed due to HLSL code syntax
            break;
        }
    }

    if (FAILED(hr))
    {
        if (errorBlob)
        {
            const char* compileErrors = static_cast<const char*>(errorBlob->GetBufferPointer());
            DX3DLogError("Shader compilation error: " + std::string(compileErrors));
        }
        else
        {
            // Convert wstring filePath safely to string for logging
            std::string pathNarrow;
            pathNarrow.reserve(filePath.size());
            for (wchar_t wc : filePath)
            {
                pathNarrow.push_back(static_cast<char>(wc & 0x7F));
            }
            DX3DLogError("Shader file not found. Attempted to find shader at candidate paths for: " + pathNarrow);
        }
        DX3DGraphicsLogThrowOnFail(hr, "Shader compilation failed.");
    }

    return ShaderBinary(blob);
}

dx3d::GraphicsDevice::~GraphicsDevice() = default;