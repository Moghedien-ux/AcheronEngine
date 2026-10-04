cbuffer ConstantData : register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    float4 m_customColor;
    float4 m_highlightParams; // x: isHighlighted, y: roughness, z: metallic, w: wireframe
};

struct VS_INPUT
{
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texCoord : TEXCOORD0;
    float4 color : COLOR0;
};

struct PS_INPUT
{
    float4 pos : SV_Position;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 texCoord : TEXCOORD0;
    float4 color : COLOR0;
    float3 worldPos : TEXCOORD1;
};

PS_INPUT VSMain(VS_INPUT input)
{
    PS_INPUT output;

    float4 worldPos = mul(float4(input.pos, 1.0f), m_world);
    float4 viewPos  = mul(worldPos, m_view);
    output.pos      = mul(viewPos, m_proj);

    output.normal    = normalize(mul(float4(input.normal, 0.0f), m_world).xyz);
    output.tangent   = normalize(mul(float4(input.tangent, 0.0f), m_world).xyz);
    output.bitangent = normalize(mul(float4(input.bitangent, 0.0f), m_world).xyz);

    output.texCoord  = input.texCoord;
    output.color     = input.color * m_customColor;
    output.worldPos  = worldPos.xyz;

    return output;
}

float4 PSMain(PS_INPUT input) : SV_Target
{
    float3 lightDir = normalize(float3(0.5f, 0.8f, -0.4f));
    float3 N = normalize(input.normal);
    float NdotL = max(0.0f, dot(N, lightDir));

    float3 fillLightDir = normalize(float3(-0.4f, 0.3f, 0.5f));
    float fillNdotL = max(0.0f, dot(N, fillLightDir)) * 0.3f;

    float3 eyePos = float3(0.0f, 3.5f, -8.0f);
    float3 V = normalize(eyePos - input.worldPos);
    float3 H = normalize(lightDir + V);
    float NdotH = max(0.0f, dot(N, H));
    float roughness = max(0.05f, m_highlightParams.y);
    float specPower = (1.0f - roughness) * 64.0f + 4.0f;
    float specular = pow(NdotH, specPower) * (1.0f - roughness);

    float2 grid = frac(input.texCoord * 4.0f);
    float checker = (grid.x > 0.03f && grid.y > 0.03f) ? 1.0f : 0.85f;

    float3 ambient = float3(0.22f, 0.24f, 0.28f);
    float3 albedo = input.color.rgb * checker;
    float3 diffuse = albedo * (ambient + (NdotL + fillNdotL) * 0.85f) + float3(1, 1, 1) * specular * 0.3f;

    if (m_highlightParams.x > 0.5f)
    {
        diffuse = lerp(diffuse, float3(1.0f, 0.55f, 0.1f), 0.28f);
    }

    return float4(diffuse, 1.0f);
}