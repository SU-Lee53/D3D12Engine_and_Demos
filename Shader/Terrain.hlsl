
cbuffer TransformData : register(b0)
{
    matrix matWorld;
};

cbuffer CameraData : register(b1)
{
    matrix matView;
    matrix matProj;
    float3 cameraPos;   // unused for now
}

struct VSInput
{
    float4 Pos : POSITION;
    float2 TexCoordDiffuse : TEXCOORD0;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 TexCoord : TEXCOORD0;
};

Texture2D texDiffuse : register(t0);
SamplerState samplerDiffuse : register(s0);

PSInput VSMain(VSInput input)
{
    PSInput result = (PSInput) 0;
    
    
    matrix vp = mul(matView, matProj);
    
    result.Pos = mul(input.Pos, matWorld);
    result.Pos = mul(result.Pos, vp);
    
    result.TexCoord = input.TexCoordDiffuse;
    
    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return texDiffuse.Sample(samplerDiffuse, input.TexCoord);
    
    
}
