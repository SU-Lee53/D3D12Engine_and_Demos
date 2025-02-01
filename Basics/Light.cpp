#include "pch.h"
#include "Light.h"

///////////////////////
// Directional Light //
///////////////////////

BOOL DirectionalLight::Initialize()
{
    m_lightType = LIGHT_TYPE_DIRECTIONAL;
    DirectionalLightDesc desc = {};
    ::ZeroMemory(&desc, sizeof(desc));
    {
        desc.lightPos = XMFLOAT3(10.f, 10.f, 10.f);
        desc.lightDir = XMFLOAT3(-1.f, -1.f, -1.f);
        desc.lightColor = XMFLOAT3(1.f, 1.f, 1.f);
    }
    m_desc.DirectionalLight = desc;

    return TRUE;
}

void DirectionalLight::Update()
{
}

CBLightData DirectionalLight::GetLightCBData()
{
    DirectionalLightDesc& desc = m_desc.DirectionalLight;

    return CBLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, 1.0f /*padding*/},
        {desc.lightDir.x, desc.lightDir.y, desc.lightDir.z, 1.0f /*padding*/},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, 1.0f /*padding*/},
        {0.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
        m_lightType
    };
}

/////////////////
// Point Light //
/////////////////

BOOL PointLight::Initialize()
{
    m_lightType = LIGHT_TYPE_POINT_LIGHT;
    PointLightDesc desc = {};
    ::ZeroMemory(&desc, sizeof(desc));
    {
        desc.lightPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
        desc.lightColor = XMFLOAT3(1.f, 1.f, 1.f);

        desc.lightIntensity = 1.f;
        desc.constantAttenuation = 1.f;
        desc.linearAttenuation = 0.09f;
        desc.QuadraticAttenuation = 0.032f;
    }
    m_desc.PointLight = desc;

    return TRUE;
}

void PointLight::Update()
{
}

CBLightData PointLight::GetLightCBData()
{
    PointLightDesc& desc = m_desc.PointLight;

    return CBLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, 1.0f /*padding*/},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, 1.0f /*padding*/},
        {desc.lightIntensity, desc.constantAttenuation, desc.linearAttenuation, desc.QuadraticAttenuation},
        {0.f, 0.f, 0.f, 0.f},
        {0.f, 0.f, 0.f, 0.f},
        m_lightType
    };
}

////////////////
// Spot Light //
////////////////

BOOL SpotLight::Initialize()
{
    m_lightType = LIGHT_TYPE_SPOT_LIGHT;
    SpotLightDesc desc = {};
    ::ZeroMemory(&desc, sizeof(desc));
    {
        desc.lightPos = XMFLOAT3(0.0f, 5.0f, 0.0f);
        desc.lightDir = XMFLOAT3(0.0f, -1.0f, 0.0f);
        desc.lightColor = XMFLOAT3(1.f, 1.f, 1.f);

        desc.lightIntensity = 1.f;
        desc.innerCone = 10.f;
        desc.outerCone = 45.f;
        desc.lightRange = 20.f;
        desc.attenuation = 1.f;
    }
    m_desc.SpotLight = desc;

    return TRUE;
}

void SpotLight::Update()
{
}

CBLightData SpotLight::GetLightCBData()
{
    SpotLightDesc& desc = m_desc.SpotLight;

    return CBLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, 1.0f /*padding*/},
        {desc.lightDir.x, desc.lightDir.y, desc.lightDir.z, 1.0f /*padding*/},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, 1.0f /*padding*/},
        {desc.lightIntensity, desc.innerCone, desc.outerCone, desc.lightRange},
        {desc.attenuation, 0.f, 0.f, 0.f},
        m_lightType
    };
}
