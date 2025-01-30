

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

cbuffer SpotlightData : register(b3)
{
    float3 lightPos;
    float pad1;
    float3 lightDir;
    float pad2;
    float3 lightColor;
    float pad3;
    
    float lightIntensity;
    float innerCone;
    float outerCone;
    float lightRange;
    float attenuation;
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

// Attenuation based of distance (Inverse Square Falllof)
float3 ComputeDistanceAttenuation(float3 worldPos)
{
    float distance = length(lightPos - worldPos);
    if (distance > lightRange)
        return 0.0f;
    
    float attenuationFactor = saturate(1.0f / (1.0f + attenuation * distance * distance));
    float rangeFactor = saturate(1.0f - (distance / lightRange));
    
    return attenuationFactor * rangeFactor;
}

// Spotlight Attenuation
float ComputeSpotlightFactor(float3 worldPos)
{
    float3 L = normalize(lightPos - worldPos); // Normalized vector to origin of light(lightPos)
    float3 D = normalize(-lightDir); // Normalized vector of direction of spotlight
    
    float theta = dot(D, L);
    
    // Spotlight Attenuation
    float spotlightFactor = saturate((theta - outerCone) / (innerCone - outerCone));
    
    return spotlightFactor;
}

float3 ComputeSpotlight(float3 normal, float3 worldPos)
{
    float spotlightFactor = ComputeSpotlightFactor(worldPos);
    float distanceAttenuation = ComputeDistanceAttenuation(worldPos);
    
    return lightColor * lightIntensity * spotlightFactor * distanceAttenuation;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    float3 normal = normalize(input.Normal);
    float3 worldPos = input.WorldPos;
    
    float3 lighting = ComputeSpotlight(normal, worldPos);
    float3 finalColor = colorDiffuse.rgb * lighting;
    
    return float4(finalColor, 1.0f);
}

    