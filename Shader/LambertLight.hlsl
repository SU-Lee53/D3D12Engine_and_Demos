

SamplerState samplerDiffuse : register(s0);

cbuffer TransformData : register(b0)
{
    matrix matLocal;
    matrix matWorld;
};

cbuffer CameraData : register(b1)
{
    matrix matView;
    matrix matProj;
}

cbuffer MaterialData : register(b2)
{
    float4 colorDiffuse;
    float4 colorSpecular;
    float4 colorAmbient;
    float4 colorEmissive;
}

cbuffer LambertLightData : register(b3)
{
    float3 lightDir;
    float3 lightColor;
}

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
    
    output.Normal = mul(float4(input.Normal, 0.0f), matWorld).xyz;
    output.BiNormal = mul(float4(input.BiNormal, 0.0f), matWorld).xyz;
    output.Tangent = mul(float4(input.Tangent, 0.0f), matWorld).xyz;
    
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 N = normalize(input.Normal);
    float3 L = normalize(-lightDir);
    
    float diffuseIntensity = max(0.0f, dot(N, L));
    
    float3 diffuse = diffuseIntensity * lightColor * colorDiffuse.xyz;
    return float4(diffuse, 1.0f);
}
