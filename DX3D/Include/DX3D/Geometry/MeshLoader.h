#pragma once
#include <DX3D/Geometry/Mesh.h>
#include <string>
#include <memory>

namespace dx3d
{
    class MeshLoader
    {
    public:
        static MeshPtr LoadFromOBJ(const std::string& filePath);

        static MeshPtr LoadFromOBJString(const std::string& objSource, const std::string& meshName = "ImportedMesh");
    };
}