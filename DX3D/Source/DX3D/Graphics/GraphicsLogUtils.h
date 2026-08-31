#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Logger.h>
#include <d3d11.h>
#include <string>

namespace dx3d
{
	inline void DX3DGraphicsLogThrowOnFail(HRESULT hr, const std::string& message)
	{
		if (FAILED(hr))
		{
			DX3DLogError(message + " (HRESULT: " + std::to_string(hr) + ")");
			throw DX3DException(message);
		}
	}
}
