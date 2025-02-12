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
    
    output.Normal = mul(input.Normal, (float3x3) (matWorld));
    output.BiNormal = mul(input.BiNormal, (float3x3) (matWorld));
    output.Tangent = mul(input.Tangent, (float3x3) (matWorld));
    
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);

float3 ComputeNormalMapping(float2 texCoord, float3 tangent, float3 binormal, float3 normal)
{
    // Sample vertex normal
    float3 sampledNormal = texNormal.Sample(samplerDiffuse, texCoord);
    
    // Clamp sampledNormal [0,1] to [-1,1]
    sampledNormal = sampledNormal * 2.f - 1.f;
    
    // Compute Tangent-Space
    float3x3 TBN = float3x3(tangent, binormal, normal);
    
    // Convert tangent-space normal to world space
    float3 finalNormal = normalize(mul(sampledNormal, TBN));
    
    return float3(finalNormal);
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 computedNormal = ComputeNormalMapping(input.TexCoord, input.Tangent, input.BiNormal, input.Normal);
    float4 lightColor = ComputeLight(input.WorldPos, computedNormal);
    float4 diffuseColor = texDiffuse.Sample(samplerDiffuse, input.TexCoord);
    
    return diffuseColor + lightColor;
}
