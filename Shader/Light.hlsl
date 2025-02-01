#include "Common.hlsl"
#include "ComputeLight.hlsl"

struct VSInput
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION1;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;
    
    float4 worldPos = mul(input.Pos, matWorld);
    matrix vp = mul(matView, matProj);
    output.Pos = mul(worldPos, vp);
    output.WorldPos = worldPos.xyz;
    
    output.Normal = mul(input.Normal, (float3x3)(matWorld));
    output.BiNormal = mul(input.BiNormal, (float3x3)(matWorld));
    output.Tangent = mul(input.Tangent, (float3x3)(matWorld));
    
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = ComputeLight(input.WorldPos, input.Normal);
    return color;
}

    