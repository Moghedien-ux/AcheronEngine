#pragma once

// Core & Logging
#include <DX3D/Core/Core.h>
#include <DX3D/Core/Base.h>
#include <DX3D/Core/Common.h>
#include <DX3D/Core/Logger.h>

// Math & Geometry
#include <DX3D/Math/Vec2.h>
#include <DX3D/Math/Vec3.h>
#include <DX3D/Math/Vec4.h>
#include <DX3D/Math/Quat.h>
#include <DX3D/Math/Mat4.h>
#include <DX3D/Math/Transform.h>
#include <DX3D/Math/Ray.h>

// Geometry & Mesh Pipeline
#include <DX3D/Geometry/Vertex.h>
#include <DX3D/Geometry/Mesh.h>
#include <DX3D/Geometry/PrimitiveFactory.h>
#include <DX3D/Geometry/MeshLoader.h>

// Scene & Hierarchy
#include <DX3D/Scene/Component.h>
#include <DX3D/Scene/MeshComponent.h>
#include <DX3D/Scene/Actor.h>

// Input & Camera
#include <DX3D/Input/InputSystem.h>
#include <DX3D/Camera/Camera.h>

// Physics & Dynamics
#include <DX3D/Physics/RigidBody.h>
#include <DX3D/Physics/PhysicsEngine.h>
#include <DX3D/Physics/Raycast.h>

// Graphics Pipeline
#include <DX3D/Graphics/GraphicsResource.h>
#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/HighlightPass.h>

// UI & Unreal Editor Interface
#include <DX3D/UI/ImGuiLayer.h>
#include <DX3D/UI/DetailsPanel.h>
#include <DX3D/UI/WorldOutliner.h>

// Window & Game
#include <DX3D/Window/Window.h>
#include <DX3D/Game/Display.h>
#include <DX3D/Game/Game.h>