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

// Multiply Vertex Position by World -> View -> Projection
float4 worldPos = mul(float4(input.pos, 1.0f), m_world);
float4 viewPos = mul(worldPos, m_view);
output.pos = mul(viewPos, m_proj);

output.color = input.color;
return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
return float4(input.color, 1.0f);
}