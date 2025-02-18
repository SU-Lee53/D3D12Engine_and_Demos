

cbuffer CameraData : register(b0)
{
    matrix matView;
    matrix matProj;
    float3 cameraPos; // unused for now
}

struct VSInput
{
    float3 Pos : POSITION;
    float3 TexCoord : TEXCOORD;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;
    
    float4 pos = float4(input.Pos, 1.0f);
    
    // delete translation from view matrix
    matrix view = matView;
    view._41 = 0.f;
    view._42 = 0.f;
    view._43 = 0.f;
    matrix proj = matProj;
    
    matrix vp = mul(view, proj);
    
    output.position = mul(pos, vp);
    output.texCoord = input.TexCoord;

    return output;
}

TextureCube<float4> texSkybox : register(t0);
SamplerState skyboxSampler : register(s0);

float4 PSMain(PSInput input) : SV_Target
{
    float3 uv = input.texCoord * 0.5 + 0.5;
    return texSkybox.Sample(skyboxSampler, uv);
}