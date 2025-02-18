

cbuffer CameraData : register(b0)
{
    matrix matView;
    matrix matProj;
    float3 cameraPos; // unused for now
}

struct VSInput
{
    float3 Pos : POSITION;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;
    
    // delete translation from view matrix
    matrix viewWithoutTranslate = matView;
    viewWithoutTranslate._41 = 0.f;
    viewWithoutTranslate._42 = 0.f;
    viewWithoutTranslate._43 = 0.f;
    
    // Compute Transform
    float4 pos = mul(float4(input.Pos, 1.f), viewWithoutTranslate);
    pos = mul(pos, matProj);
    
    output.texCoord = input.Pos;
    output.position = pos;
    
    return output;
}

TextureCube<float4> texSkybox : register(t0);
SamplerState skyboxSampler : register(s0);

float4 PSMain(PSInput input) : SV_Target
{
    return texSkybox.Sample(skyboxSampler, input.texCoord);
}