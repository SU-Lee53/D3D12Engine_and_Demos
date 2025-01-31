

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
    float3 camPos;
};

cbuffer MaterialData : register(b2)
{
    float4 colorDiffuse;
    float4 colorSpecular;
    float4 colorAmbient;
    float4 colorEmissive;
};

cbuffer PointlightData : register(b3)
{
    float3 lightPos;
    float pad1;
    float3 lightColor;
    float pad2;
    
    float ligntIntensity;
    float constantAttenuation;
    float linearAttenuation;
    float QuadraticAttenuation;
};

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

float4 ComputeSpotlight(float3 normal, float3 worldPos)
{
    normal = normalize(normal);
    float3 toLight = lightPos - worldPos;
    float distance = length(toLight);
    toLight = normalize(toLight);
    
    // Diffuse(Lambert)
    float diff = max(dot(normal, toLight), 0.0f);
    
    // Attenuation
    float attenuation = 1.0f / (constantAttenuation + (linearAttenuation * distance) + QuadraticAttenuation * (distance * distance));
    float3 color = lightColor * ligntIntensity * diff * attenuation;
    
    return float4(color, 1.0f);
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float4 color = ComputeSpotlight(input.Normal, input.WorldPos);
    return color;
}

    