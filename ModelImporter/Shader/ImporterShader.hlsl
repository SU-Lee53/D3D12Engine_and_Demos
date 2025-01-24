
struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texCoord : TEXCOORD0;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

cbuffer TransformData : register(b0)
{
    matrix matLocal;
    matrix matWorld;
};
    
cbuffer CameraData : register(b1)
{
    matrix matView;
    matrix matProj;
};

Texture2D diffuseMap : register(t0);
SamplerState samplerDiffuse : register(s0);


PSInput VSMain(VSInput input)
{
    PSInput result = (PSInput) 0;
    
    
    matrix matViewProj = mul(matView, matProj); // view x proj
    matrix matWorldViewProj = mul(matWorld, matViewProj); // world x view x proj
    result.position = mul(input.position, matWorldViewProj); // pojtected vertex = vertex x world x view x proj
    result.texCoord = input.texCoord;
    result.color = input.color;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return float4(1.f, 0.f, 0.f, 1.f);
}
