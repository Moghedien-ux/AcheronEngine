#include <DX3D/Graphics/GraphicsDevice.h>
#include <DX3D/Graphics/GraphicsEngine.h>
#include <DX3D/Graphics/DeviceContext.h>
#include <DX3D/Graphics/SwapChain.h>
#include <DX3D/Graphics/VertexBuffer.h>
#include <DX3D/Graphics/IndexBuffer.h>
#include <DX3D/Graphics/ConstantBuffer.h>
#include <DX3D/Graphics/GraphicsPipelineState.h>
#include <DX3D/Graphics/ShaderBinary.h>
#include <DX3D/Camera/Camera.h>

using namespace dx3d;

dx3d::GraphicsEngine::GraphicsEngine(const GraphicsEngineDesc& desc) : Base(desc.base)
{
	m_graphicsDevice = std::make_shared<GraphicsDevice>(GraphicsDeviceDesc{ m_logger });

	auto& device = *m_graphicsDevice;
	m_deviceContext = device.createDeviceContext();

	// HLSL Shader code with MVP matrix constant buffer
	constexpr char shaderSourceCode[] =
		R"(
cbuffer ConstantData : register(b0)
{
	row_major float4x4 m_world;
	row_major float4x4 m_view;
	row_major float4x4 m_proj;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float3 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float3 color : COLOR;
};

PS_INPUT VSMain(VS_INPUT input)
{
	PS_INPUT output;
	float4 worldPos = mul(float4(input.pos, 1.0f), m_world);
	float4 viewPos  = mul(worldPos, m_view);
	output.pos      = mul(viewPos, m_proj);
	output.color    = input.color;
	return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
	return float4(input.color, 1.0f);
}
)";

	constexpr char shaderSourceName[] = "CubeShader";
	constexpr auto shaderSourceCodeSize = std::size(shaderSourceCode);

	auto vs = device.compileShader({ shaderSourceName, shaderSourceCode, shaderSourceCodeSize,
		"VSMain", ShaderType::VertexShader });
	auto ps = device.compileShader({ shaderSourceName, shaderSourceCode, shaderSourceCodeSize,
		"PSMain", ShaderType::PixelShader });

	m_pipeline = device.createGraphicsPipelineState({ *vs, *ps });

	// 8 Unique Vertices for the 3D Cube (Position + Color)
	const Vertex vertexList[] =
	{
		// Front face
		{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 0.0f } }, // 0: Red
		{ { -0.5f,  0.5f, -0.5f }, { 0.0f, 1.0f, 0.0f } }, // 1: Green
		{ {  0.5f,  0.5f, -0.5f }, { 0.0f, 0.0f, 1.0f } }, // 2: Blue
		{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 1.0f, 0.0f } }, // 3: Yellow

		// Back face
		{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f } }, // 4: Magenta
		{ {  0.5f,  0.5f,  0.5f }, { 0.0f, 1.0f, 1.0f } }, // 5: Cyan
		{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 1.0f, 1.0f } }, // 6: White
		{ { -0.5f, -0.5f,  0.5f }, { 0.2f, 0.2f, 0.2f } }, // 7: Dark Gray
	};

	m_vb = device.createVertexBuffer({ vertexList, static_cast<ui32>(std::size(vertexList)), sizeof(Vertex) });

	// 36 Indices (12 triangles, 2 per each of the 6 cube faces)
	const ui32 indexList[] =
	{
		// Front
		0, 1, 2,  0, 2, 3,
		// Right
		3, 2, 5,  3, 5, 4,
		// Back
		4, 5, 6,  4, 6, 7,
		// Left
		7, 6, 1,  7, 1, 0,
		// Top
		1, 6, 5,  1, 5, 2,
		// Bottom
		7, 0, 3,  7, 3, 4
	};

	m_ib = device.createIndexBuffer({ indexList, static_cast<ui32>(std::size(indexList)) });

	// Constant Buffer for matrices
	ConstantData cData{};
	m_cb = device.createConstantBuffer({ &cData, sizeof(ConstantData) });
}

dx3d::GraphicsEngine::~GraphicsEngine()
{}

GraphicsDevice& dx3d::GraphicsEngine::getGraphicsDevice() noexcept
{
	return *m_graphicsDevice;
}

void dx3d::GraphicsEngine::render(SwapChain& swapChain, const Camera& camera, f32 cubeRotation)
{
	auto& context = *m_deviceContext;

	// Dark slate blue background
	context.clearAndSetBackBuffer(swapChain, { 0.1f, 0.12f, 0.16f, 1.0f });
	context.setGraphicsPipelineState(*m_pipeline);
	context.setViewportSize(swapChain.getSize());

	// Compute MVP transformation matrices
	ConstantData cData;
	Mat4 rotY = Mat4::RotationY(cubeRotation);
	Mat4 rotX = Mat4::RotationX(cubeRotation * 0.5f);
	cData.world = rotX * rotY;
	cData.view = camera.getViewMatrix();
	cData.proj = camera.getProjectionMatrix();

	// Update constant buffer and bind to shader slot 0
	m_cb->update(&cData, sizeof(ConstantData));
	context.setConstantBuffer(*m_cb, 0);

	// Bind Vertex & Index Buffers
	context.setVertexBuffer(*m_vb);
	context.setIndexBuffer(*m_ib);

	// Draw the 36 indices of the 3D cube
	context.drawIndexedTriangleList(m_ib->getIndexListSize(), 0, 0);

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();
}

// ==========================================
// În DX3D/Source/GraphicsEngine.cpp:
// ==========================================
void dx3d::GraphicsEngine::renderObjects(SwapChain& swapChain, const Camera& camera, const std::vector<GameObjectPtr>& objects)
{
	auto& context = *m_deviceContext;

	// Background slate-blue
	context.clearAndSetBackBuffer(swapChain, { 0.08f, 0.10f, 0.14f, 1.0f });
	context.setGraphicsPipelineState(*m_pipeline);
	context.setViewportSize(swapChain.getSize());

	context.setVertexBuffer(*m_vb);
	context.setIndexBuffer(*m_ib);

	ConstantData cData;
	cData.view = camera.getViewMatrix();
	cData.proj = camera.getProjectionMatrix();

	for (const auto& obj : objects)
	{
		if (!obj || !obj->m_active) continue;

		cData.world = obj->getWorldMatrix();
		m_cb->update(&cData, sizeof(ConstantData));
		context.setConstantBuffer(*m_cb, 0);

		context.drawIndexedTriangleList(m_ib->getIndexListSize(), 0, 0);
	}

	auto& device = *m_graphicsDevice;
	device.executeCommandList(context);
	swapChain.present();
}