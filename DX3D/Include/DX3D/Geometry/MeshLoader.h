#pragma once
#include <DX3D/Geometry/Mesh.h>
#include <string>
#include <memory>

namespace dx3d
{
    class MeshLoader
    {
    public:
        // Loads a 3D Wavefront .OBJ mesh file from disk
        static MeshPtr LoadFromOBJ(const std::string& filePath);

        // Loads a 3D Wavefront .OBJ mesh directly from memory / text string
        static MeshPtr LoadFromOBJString(const std::string& objSource, const std::string& meshName = "ImportedMesh");
    };
}