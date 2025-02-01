#ifndef _COMMON_
#define _COMMON_

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

#endif