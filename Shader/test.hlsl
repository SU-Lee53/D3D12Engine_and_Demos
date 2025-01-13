//Texture2D texDiffuse : register(t0);
SamplerState samplerDiffuse : register(s0);

cbuffer TransformData : register(b0)
{
    matrix matWorld;
};

cbuffer CameraData : register(b1)
{
    matrix matView;
    matrix matProj;
}

struct VSInput
{
    float4 Pos : POSITION;
    float4 color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

PSInput VSMain(VSInput input)
{
    PSInput result = (PSInput) 0;
    
    
    matrix matViewProj = mul(matView, matProj); // view x proj
    matrix matWorldViewProj = mul(matWorld, matViewProj); // world x view x proj
    result.position = mul(input.Pos, matWorldViewProj); // pojtected vertex = vertex x world x view x proj
    result.TexCoord = input.TexCoord;
    result.color = input.color;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
}
