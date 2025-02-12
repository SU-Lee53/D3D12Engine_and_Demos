
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

cbuffer ColorData : register(b2)
{
    float4 ShowColor;
};

Texture2D diffuseMap : register(t0);
SamplerState samplerDiffuse : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput result = (PSInput) 0;
    
    matrix matViewProj = mul(matView, matProj); // view x proj
    matrix matLocalViewProj = mul(matLocal, matViewProj); // local x view x proj
    matrix matWorldViewProj = mul(matWorld, matLocalViewProj); // world x view x proj
    result.position = mul(input.position, matWorldViewProj); // pojtected vertex = vertex x world x view x proj
    result.texCoord = input.texCoord;
    result.color = input.color;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return ShowColor;
}
