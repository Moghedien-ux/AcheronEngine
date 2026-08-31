#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Ray.h>
#include <DX3D/Physics/GameObject.h>
#include <vector>
#include <memory>

namespace dx3d
{
    struct RaycastHit
    {
        bool hasHit{ false };
        GameObject* hitObject{ nullptr };
        Vec3 point{ 0.0f, 0.0f, 0.0f };
        Vec3 normal{ 0.0f, 1.0f, 0.0f };
        f32 distance{ 1e9f };
    };

    class Raycast
    {
    public:
        // Generates a world-space Ray from mouse screen coordinates and camera matrices
        static Ray screenPointToRay(
            i32 screenX, i32 screenY,
            i32 screenWidth, i32 screenHeight,
            const Mat4& viewMatrix,
            const Mat4& projMatrix,
            const Vec3& cameraPosition);

        // Raycasts against all GameObjects in the scene and returns the closest hit
        static RaycastHit pickObject(
            const Ray& ray,
            const std::vector<GameObjectPtr>& objects);
    };
}
