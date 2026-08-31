#pragma once
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Mat4.h>
#include <algorithm>
#include <cmath>

namespace dx3d
{
    struct Ray
    {
        Vec3 origin{ 0.0f, 0.0f, 0.0f };
        Vec3 direction{ 0.0f, 0.0f, 1.0f };

        Ray() = default;
        Ray(const Vec3& orig, const Vec3& dir) : origin(orig), direction(dir.normalized()) {}

        Vec3 getPoint(f32 t) const
        {
            return origin + direction * t;
        }

        // Ray-AABB Intersection test (Slab method)
        // Returns true if ray intersects axis-aligned box between minBounds and maxBounds
        bool intersectAABB(const Vec3& minBounds, const Vec3& maxBounds, f32& outT) const
        {
            f32 tmin = (minBounds.x - origin.x) / (std::abs(direction.x) > 1e-6f ? direction.x : 1e-6f);
            f32 tmax = (maxBounds.x - origin.x) / (std::abs(direction.x) > 1e-6f ? direction.x : 1e-6f);

            if (tmin > tmax) std::swap(tmin, tmax);

            f32 tymin = (minBounds.y - origin.y) / (std::abs(direction.y) > 1e-6f ? direction.y : 1e-6f);
            f32 tymax = (maxBounds.y - origin.y) / (std::abs(direction.y) > 1e-6f ? direction.y : 1e-6f);

            if (tymin > tymax) std::swap(tymin, tymax);

            if ((tmin > tymax) || (tymin > tmax))
                return false;

            if (tymin > tmin)
                tmin = tymin;

            if (tymax < tmax)
                tmax = tymax;

            f32 tzmin = (minBounds.z - origin.z) / (std::abs(direction.z) > 1e-6f ? direction.z : 1e-6f);
            f32 tzmax = (maxBounds.z - origin.z) / (std::abs(direction.z) > 1e-6f ? direction.z : 1e-6f);

            if (tzmin > tzmax) std::swap(tzmin, tzmax);

            if ((tmin > tzmax) || (tzmin > tmax))
                return false;

            if (tzmin > tmin)
                tmin = tzmin;

            if (tzmax < tmax)
                tmax = tzmax;

            if (tmax < 0.0f)
                return false; // Box is behind ray

            outT = (tmin >= 0.0f) ? tmin : tmax;
            return true;
        }
    };
}
