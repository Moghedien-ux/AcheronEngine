#pragma once
#include <DX3D/Geometry/Mesh.h>
#include <memory>

namespace dx3d
{
    class PrimitiveFactory
    {
    public:
        static MeshPtr CreateCube(const Vec3& size = { 1.0f, 1.0f, 1.0f }, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreateSphere(f32 radius = 0.5f, u32 slices = 32, u32 stacks = 16, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreateCylinder(f32 radius = 0.5f, f32 height = 1.5f, u32 slices = 32, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreateCapsule(f32 radius = 0.5f, f32 height = 1.0f, u32 slices = 24, u32 stacks = 8, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreatePlane(f32 width = 10.0f, f32 depth = 10.0f, u32 subDivX = 10, u32 subDivZ = 10, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreateCone(f32 radius = 0.5f, f32 height = 1.5f, u32 slices = 32, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });

        static MeshPtr CreateTorus(f32 outerRadius = 0.8f, f32 innerRadius = 0.25f, u32 segments = 32, u32 tubeSegments = 16, const Vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
    };
}