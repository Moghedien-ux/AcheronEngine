cbuffer ConstantData : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_customColor;
    float4 m_highlightParams; // x: isHighlighted (1/0), y: unused
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 color : COLOR;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 color : COLOR;
    float3 worldPos : TEXCOORD0;
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;

    float4 worldPos = mul(float4(input.pos, 1.0f), m_world);
    float4 viewPos  = mul(worldPos, m_view);
    output.pos      = mul(viewPos, m_proj);

    // Transform normals
    output.normal   = mul(float4(input.normal, 0.0f), m_world).xyz;
    output.color    = input.color * m_customColor.rgb;
    output.worldPos = worldPos.xyz;

    return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
    // Directional Sunlight
    float3 lightDir = normalize(float3(0.5f, 0.8f, -0.4f));
    float3 N = normalize(input.normal);
    float NdotL = max(0.0f, dot(N, lightDir));

    // Ambient + Diffuse
    float3 ambient = float3(0.25f, 0.28f, 0.32f);
    float3 diffuse = input.color * (ambient + NdotL * 0.75f);

    // If highlighted in Unreal Engine style, blend in an orange border sheen
    if (m_highlightParams.x > 0.5f)
    {
        diffuse = lerp(diffuse, float3(1.0f, 0.55f, 0.1f), 0.25f);
    }

    return float4(diffuse, 1.0f);
}
