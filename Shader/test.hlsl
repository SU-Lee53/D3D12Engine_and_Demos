
struct VS_IN
{
    float4 position : SV_POSITION;
    float4 color    : COLOR;
    //float4 worldPosition    : POSITION;
    //float3 normal           : NORMAL;
    //float3 biNormal         : BINORMAL;
    //float2 uv               : TEXCOORD;
};

struct PS_IN
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PS_IN VS_Main(VS_IN input)
{
    PS_IN output;
    
    return output;
}

float PS_Main(PS_IN input) : SV_Target
{
    return float4(1.f, 1.f, 1.f, 1.f);
}
