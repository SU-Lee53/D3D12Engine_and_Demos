#include "Common.hlsl"

#ifndef _LIGHT_
#define _LIGHT_

#define MAX_LIGHT_COUNT 10

#define DIRECTIONAL_LIGHT 0
#define POINT_LIGHT 1
#define SPOT_LIGHT 2

struct DirectionalLightData
{
   float3 lightPos;
   float3 lightDir;
   float3 lightColor;
};

struct PointLightData
{
    float3 lightPos;
    float lightIntensity;
    float3 lightColor;
    float constantAttenuation;

    float linearAttenuation;
    float QuadraticAttenuation;
};

struct SpotLightData
{
    float3 lightPos;
    float lightIntensity;
    float3 lightDir;
    float innerCone; // cos(еш) of inner cone 
    float3 lightColor;
    float outerCone; // cos(еш) of outer cone 

    float lightRange;
    float attenuation;
};

struct RawLightData
{
    float4 data1;
    float4 data2;
    float4 data3;
    float2 data4;

    int type;
    bool bEnable;
};

cbuffer LightData : register(b3)
{
    RawLightData lights[MAX_LIGHT_COUNT];
    int nLights;
};

DirectionalLightData DecomposeRawDataToDirectionalLight(RawLightData rawData)
{
    DirectionalLightData ret;
    
    ret.lightPos = float3(rawData.data1.x, rawData.data1.y, rawData.data1.z);
    ret.lightDir = float3(rawData.data2.x, rawData.data2.y, rawData.data2.z);
    ret.lightColor = float3(rawData.data3.r, rawData.data3.g, rawData.data3.b);
    
    return ret;
}

PointLightData DecomposeRawDataToPointLight(RawLightData rawData)
{
    PointLightData ret;
    
    ret.lightPos = float3(rawData.data1.x, rawData.data1.y, rawData.data1.z);
    ret.lightIntensity = rawData.data1.w;
    ret.lightColor = float3(rawData.data2.r, rawData.data2.g, rawData.data2.b);
    ret.constantAttenuation = rawData.data2.w;
    ret.linearAttenuation = rawData.data3.r;
    ret.QuadraticAttenuation = rawData.data3.g;

    return ret;
}

SpotLightData DecomposeRawDataToSpotLight(RawLightData rawData)
{
    SpotLightData ret;
    
    ret.lightPos = float3(rawData.data1.x, rawData.data1.y, rawData.data1.z);
    ret.lightIntensity = rawData.data1.w;
    ret.lightDir = float3(rawData.data2.x, rawData.data2.y, rawData.data2.z);
    ret.innerCone = rawData.data2.w;
    ret.lightColor = float3(rawData.data3.r, rawData.data3.g, rawData.data3.b);
    ret.outerCone = rawData.data3.w;
    ret.lightRange = rawData.data4.x;
    ret.attenuation = rawData.data4.y;

    return ret;
}

float4 ComputeDirectionalLight(int index, float3 worldPos, float3 normal, float3 toCamera)
{
    DirectionalLightData data = DecomposeRawDataToDirectionalLight(lights[index]);
    
    float3 N = normalize(normal); // Normalized normal vector
    float3 L = normalize(data.lightPos - worldPos); // Noralized vector to light
    float3 V = normalize(camPos - worldPos); // Normalized vector to camera
    float3 H = normalize(L + V); // Normalized half vector
    
    // Diffuse(Lambert)
    float diffuseIntensity = max(0.0f, dot(N, L));
    float3 diffuse = diffuseIntensity * data.lightColor.rgb * colorDiffuse.rgb;
    
    // Blinn-Phong
    float shininess = max(1.0, colorSpecular.a);
    float specularIntensity = pow(max(0.0f, dot(N, H)), shininess);
    float3 specular = specularIntensity * data.lightColor.rgb * colorSpecular.rgb;
    
    // Ambient
    float3 ambient = colorAmbient.rgb * data.lightColor.rgb;
    
    float3 finalColor = diffuse + specular + ambient;
    return float4(finalColor, 1.0f);
}

float4 ComputePointLight(int index, float3 worldPos, float3 normal, float3 toCamera)
{
    PointLightData data = DecomposeRawDataToPointLight(lights[index]);
    
    normal = normalize(normal);
    float3 toLight = data.lightPos - worldPos;
    float distance = length(toLight);
    toLight = normalize(toLight);
    
    // Diffuse(Lambert)
    float diff = max(dot(normal, toLight), 0.0f);
    
    // Attenuation
    float attenuation = 1.0f / (data.constantAttenuation + (data.linearAttenuation * distance) + data.QuadraticAttenuation * (distance * distance));
    float3 color = data.lightColor * data.lightIntensity * diff * attenuation;
    
    return float4(color, 1.0f);
}

float4 ComputeSpotLight(int index, float3 worldPos, float3 normal, float3 toCamera)
{
    SpotLightData data = DecomposeRawDataToSpotLight(lights[index]);
    
    // Distance Attenuation
    float distanceAttenuation = 0.0f;
    float distance = length(data.lightPos - worldPos);
    if(distance < data.lightRange)
    {
        float attenuation = saturate(1.0f / (1.0f + data.attenuation * distance * distance));
        float rangeFactor = saturate(1.0f - (distance / data.lightRange));
        distanceAttenuation = attenuation * rangeFactor;
    }
        
    // SpotLight Attenuation
    float3 L = normalize(data.lightPos - worldPos);
    float3 D = normalize(-data.lightDir);
    float3 theta = dot(D, L);
    
    float spotlightFactor = saturate((theta - data.outerCone) / (data.innerCone - data.outerCone));

    return float4(data.lightColor * data.lightIntensity * spotlightFactor * distanceAttenuation, 1.f);
}

float4 ComputeLight(float3 worldPos, float3 normal)
{
    float3 toCamera = normalize(camPos - worldPos);
    
    float4 finalColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    [unroll(MAX_LIGHT_COUNT)]
    for (int i = 0; i < nLights; i++)
    {
        if(lights[i].bEnable)
        {
            if (lights[i].type == DIRECTIONAL_LIGHT)
            {
                finalColor += ComputeDirectionalLight(i, worldPos, normal, toCamera);
            }
            else if (lights[i].type == POINT_LIGHT)
            {
                finalColor += ComputePointLight(i, worldPos, normal, toCamera);
            }
            else if (lights[i].type == SPOT_LIGHT)
            {
                finalColor += ComputeSpotLight(i, worldPos, normal, toCamera);
            }
        }
    }
    finalColor.a = colorDiffuse.a;
    
    return finalColor;
}

#endif