#include "pch.h"
#include "Light.h"

using namespace std;

///////////////////////
// Directional Light //
///////////////////////

BOOL DirectionalLight::Initialize()
{
    m_lightType = LIGHT_TYPE_DIRECTIONAL;
    DirectionalLightData desc = {};
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

RawLightData DirectionalLight::GetLightRawData()
{
    DirectionalLightData& desc = m_desc.DirectionalLight;

    return RawLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, 1.0f /*padding*/},
        {desc.lightDir.x, desc.lightDir.y, desc.lightDir.z, 1.0f /*padding*/},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, 1.0f /*padding*/},
        {0.f, 0.f},
        m_lightType,
        m_bEnable
    };
}

void DirectionalLight::ControlLightWithImGui(int showIndex)
{
    XMFLOAT3 pos = GetPosition();
    XMFLOAT3 dir = GetDirection();
    XMFLOAT3 color = GetColor();

    if (ImGui::TreeNode(to_string(showIndex).c_str()))
    {
        ImGui::Text("Light Type : Directional Light");
        ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
        ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.01f, -1.f, 1.f);
        ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);
        if (ImGui::Button(m_bEnable ? "ON" : "OFF"))
        {
            if (m_bEnable)
                m_bEnable = FALSE;
            else
                m_bEnable = TRUE;
        }

        SetPosition(pos);
        SetDirection(dir);
        SetColor(color);

        ImGui::TreePop();
    }
}

/////////////////
// Point Light //
/////////////////

BOOL PointLight::Initialize()
{
    m_lightType = LIGHT_TYPE_POINT_LIGHT;
    PointLightData desc = {};
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

RawLightData PointLight::GetLightRawData()
{
    PointLightData& desc = m_desc.PointLight;

    return RawLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, desc.lightIntensity},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, desc.constantAttenuation},
        {desc.linearAttenuation, desc.QuadraticAttenuation, 0.f, 0.f},
        {0.f, 0.f},
        m_lightType,
        m_bEnable
    };
}

void PointLight::ControlLightWithImGui(int showIndex)
{
    XMFLOAT3 pos = GetPosition();
    XMFLOAT3 color = GetColor();

    float intensity = GetLightIntensity();
    float constant = GetConstantAttenuation();
    float linear = GetLinearAttenuation();
    float quadratic = GetQuadraticAttenuation();

    string PosToStr = "("s + to_string(pos.x) + ", "s + to_string(pos.y) + ", " + to_string(pos.z) + ")"s;
    string strTabName = "PointLight On : "s + PosToStr;

    if (ImGui::TreeNode(to_string(showIndex).c_str()))
    {
        ImGui::Text("Light Type : Point Light");
        ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
        ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);

        ImGui::DragFloat("LightIntensity"s.c_str(), &intensity, 0.01f, 0.f, 5.f);
        ImGui::DragFloat("Constant Attenuation"s.c_str(), &constant, 0.01f, 0.f, 5.f);
        ImGui::DragFloat("Linear Attenuation"s.c_str(), &linear, 0.01f, 0.f, 5.f);
        ImGui::DragFloat("Quadratic Attenuation"s.c_str(), &quadratic, 0.01f, 0.f, 5.f);
        if (ImGui::Button(m_bEnable ? "ON" : "OFF"))
        {
            if (m_bEnable)
                m_bEnable = FALSE;
            else
                m_bEnable = TRUE;
        }

        SetPosition(pos);
        SetColor(color);

        SetLightIntensity(intensity);
        SetConstantAttenuation(constant);
        SetLinearAttenuation(linear);
        SetQuadraticAttenuation(quadratic);

        ImGui::TreePop();
    }
}

////////////////
// Spot Light //
////////////////

BOOL SpotLight::Initialize()
{
    m_lightType = LIGHT_TYPE_SPOT_LIGHT;
    SpotLightData desc = {};
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

RawLightData SpotLight::GetLightRawData()
{
    SpotLightData& desc = m_desc.SpotLight;

    return RawLightData
    {
        {desc.lightPos.x, desc.lightPos.y, desc.lightPos.z, desc.lightIntensity},
        {desc.lightDir.x, desc.lightDir.y, desc.lightDir.z, desc.innerCone},
        {desc.lightColor.x, desc.lightColor.y, desc.lightColor.z, desc.outerCone},
        {desc.lightRange, desc.attenuation},
        m_lightType,
        m_bEnable
    };
}

void SpotLight::ControlLightWithImGui(int showIndex)
{
    XMFLOAT3 pos = GetPosition();
    XMFLOAT3 dir = GetDirection();
    XMFLOAT3 color = GetColor();

    float lightIntensity = GetLightIntensity();
    float innerCone = GetInnerCone();
    float outerCone = GetOuterCone();
    float lightRange = GetLightRange();
    float attenuation = GetAttenuation();

    string PosToStr = "("s + to_string(pos.x) + ", "s + to_string(pos.y) + ", " + to_string(pos.z) + ")"s;
    string strTabName = "SpotLight On : "s + PosToStr;

    if (ImGui::TreeNode(to_string(showIndex).c_str()))
    {
        ImGui::Text("Light Type : Spot Light");
        ImGui::DragFloat3("Position"s.c_str(), (float*)&pos, 0.01f, -10.f, 10.f);
        ImGui::DragFloat3("Direction"s.c_str(), (float*)&dir, 0.01f, -1.f, 1.f);
        ImGui::DragFloat3("Color"s.c_str(), (float*)&color, 0.01f, 0.f, 1.f);

        ImGui::DragFloat("LightIntensity"s.c_str(), &lightIntensity, 0.1f, 0.f, 10.f);
        ImGui::DragFloat("InnerCone"s.c_str(), &innerCone, 0.1f, 0.f, 90.f);
        ImGui::DragFloat("OuterCone"s.c_str(), &outerCone, 0.1f, 0.f, 90.f);
        ImGui::DragFloat("LightRange"s.c_str(), &lightRange, 0.1f, 0.f, 50.f);
        ImGui::DragFloat("Attenuation"s.c_str(), &attenuation, 0.1f, 0.f, 1.f);
        if (ImGui::Button(m_bEnable ? "ON" : "OFF"))
        {
            if (m_bEnable)
                m_bEnable = FALSE;
            else 
                m_bEnable = TRUE;
        }


        SetPosition(pos);
        SetDirection(dir);
        SetColor(color);

        SetLightIntensity(lightIntensity);
        SetInnerCone(innerCone);
        SetOuterCone(outerCone);
        SetLightRange(lightRange);
        SetAttenuation(attenuation);

        ImGui::TreePop();
    }
}
