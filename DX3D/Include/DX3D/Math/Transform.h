#pragma once
#include <DX3D/Core/Core.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Quat.h>
#include <DX3D/Math/Mat4.h>
#include <vector>
#include <algorithm>

namespace dx3d
{
    class Transform
    {
    public:
        Transform();
        Transform(const Vec3& location, const Quat& rotation = Quat::Identity(), const Vec3& scale = Vec3::One);
        ~Transform();

        // Local Transformations
        void setLocation(const Vec3& loc);
        void setRotation(const Quat& rot);
        void setRotationEuler(const Vec3& eulerRadians);
        void setRotationDegrees(const Vec3& eulerDegrees);
        void setScale(const Vec3& scale);

        const Vec3& getLocation() const { return m_location; }
        const Quat& getRotation() const { return m_rotation; }
        Vec3 getRotationEuler() const { return m_rotation.toEuler(); }
        Vec3 getRotationDegrees() const;
        const Vec3& getScale() const { return m_scale; }

        Vec3 getForward() const;
        Vec3 getRight() const;
        Vec3 getUp() const;

        void setParent(Transform* parent, bool keepWorldTransform = true);
        Transform* getParent() const { return m_parent; }
        const std::vector<Transform*>& getChildren() const { return m_children; }

        Mat4 getLocalMatrix() const;
        Mat4 getWorldMatrix() const;

        Vec3 getWorldLocation() const;
        Quat getWorldRotation() const;
        Vec3 getWorldScale() const;

        Vec3 transformPoint(const Vec3& point) const;
        Vec3 transformVector(const Vec3& vector) const;

        void markDirty();

    private:
        void updateWorldMatrix() const;
        void removeChild(Transform* child);

    private:
        Vec3 m_location{ 0.0f, 0.0f, 0.0f };
        Quat m_rotation{ 0.0f, 0.0f, 0.0f, 1.0f };
        Vec3 m_scale{ 1.0f, 1.0f, 1.0f };

        Transform* m_parent{ nullptr };
        std::vector<Transform*> m_children;

        mutable Mat4 m_cachedLocalMatrix{};
        mutable Mat4 m_cachedWorldMatrix{};
        mutable bool m_isDirty{ true };
    };
}