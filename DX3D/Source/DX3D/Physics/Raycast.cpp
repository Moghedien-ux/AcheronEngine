#include <DX3D/Physics/Raycast.h>
#include <DX3D/Camera/Camera.h>

dx3d::Ray dx3d::Raycast::screenPointToRay(
    i32 screenX, i32 screenY,
    i32 screenWidth, i32 screenHeight,
    const Mat4& viewMatrix,
    const Mat4& projMatrix,
    const Vec3& cameraPosition)
{
    if (screenWidth <= 0 || screenHeight <= 0)
        return Ray(cameraPosition, { 0.0f, 0.0f, 1.0f });

    // 1. Normalized Device Coordinates (NDC) in range [-1, 1]
    f32 ndcX = (2.0f * static_cast<f32>(screenX) / static_cast<f32>(screenWidth)) - 1.0f;
    f32 ndcY = 1.0f - (2.0f * static_cast<f32>(screenY) / static_cast<f32>(screenHeight));

    // 2. Unproject using Inverse Projection
    f32 tanHalfFov = 1.0f / projMatrix.m[1][1];
    f32 aspect = projMatrix.m[1][1] / projMatrix.m[0][0];

    f32 viewX = ndcX * tanHalfFov * aspect;
    f32 viewY = ndcY * tanHalfFov;
    f32 viewZ = 1.0f; // Forward in camera space

    Vec3 rayDirView = Vec3(viewX, viewY, viewZ).normalized();

    // 3. Transform from View Space to World Space
    Vec3 right = { viewMatrix.m[0][0], viewMatrix.m[1][0], viewMatrix.m[2][0] };
    Vec3 up    = { viewMatrix.m[0][1], viewMatrix.m[1][1], viewMatrix.m[2][1] };
    Vec3 fwd   = { viewMatrix.m[0][2], viewMatrix.m[1][2], viewMatrix.m[2][2] };

    Vec3 rayDirWorld = (right * rayDirView.x + up * rayDirView.y + fwd * rayDirView.z).normalized();

    return Ray(cameraPosition, rayDirWorld);
}

dx3d::RaycastHit dx3d::Raycast::pickObject(
    const Ray& ray,
    const std::vector<GameObjectPtr>& objects)
{
    RaycastHit closestHit;
    closestHit.distance = 1e9f;

    for (const auto& obj : objects)
    {
        if (!obj || !obj->m_active) continue;

        // Calculate bounding box in world space
        Vec3 halfScale = obj->m_scale * 0.5f;
        Vec3 minBounds = obj->m_position - halfScale;
        Vec3 maxBounds = obj->m_position + halfScale;

        f32 hitDist = 0.0f;
        if (ray.intersectAABB(minBounds, maxBounds, hitDist))
        {
            if (hitDist < closestHit.distance && hitDist > 0.05f)
            {
                closestHit.hasHit = true;
                closestHit.hitObject = obj.get();
                closestHit.distance = hitDist;
                closestHit.point = ray.getPoint(hitDist);
            }
        }
    }

    return closestHit;
}
