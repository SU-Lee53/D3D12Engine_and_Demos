SamplerState samplerDiffuse : register(s0);

cbuffer CameraData : register(b0)
{
    matrix matView;
    matrix matProj;
}

struct InstanceData
{
    matrix matWorld;
    float4 colorDiffuse;
};

StructuredBuffer<InstanceData> instanceBuffer : register(t0);

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

PSInput VSMain(VSInput input, uint nInstanceID : SV_InstanceID)
{
    PSInput output = (PSInput) 0;
    
    float4 worldPos = mul(input.Pos, instanceBuffer[nInstanceID].matWorld);
    matrix vp = mul(matView, matProj);
    output.Pos = mul(worldPos, vp);
    output.WorldPos = worldPos.xyz;
    
    output.Normal = mul(input.Normal, (float3x3)(instanceBuffer[nInstanceID].matWorld));
    output.BiNormal = mul(input.BiNormal, (float3x3)(instanceBuffer[nInstanceID].matWorld));
    output.Tangent = mul(input.Tangent, (float3x3)(instanceBuffer[nInstanceID].matWorld));
    
    output.TexCoord = input.TexCoord;
    output.Color = instanceBuffer[nInstanceID].colorDiffuse;
    
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.Color;
}
