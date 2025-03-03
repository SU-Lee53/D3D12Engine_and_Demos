
struct CameraData
{
    matrix matView;
    matrix matProj;
    float4 cameraPos; // unused for now
};

struct VSInput
{
    float3 Pos : POSITION;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float2 texCoord : TEXCOORD;
    uint instanceID : TEXCOORD1;
};

struct PSOutput
{
    float4 target0 : SV_Target0;
    float4 target1 : SV_Target1;
    float4 target2 : SV_Target2;
    float4 target3 : SV_Target3;
    float4 target4 : SV_Target4;
    float4 target5 : SV_Target5;
};

PSInput VSMain(VSInput input, uint instanceID : SV_InstanceID)
{
    PSInput output;
    
    // Set position to clip space
    output.Pos = float4(input.Pos, 1.f);
    
    // UV transform [-1, 1] to [0, 1] 
    float2 ndc = input.Pos.xy;
    output.texCoord = (ndc * 0.5f) + 0.5f;
    
    output.instanceID = instanceID;
    
    return output;
}

Texture2D texSkybox : register(t0);
SamplerState samplerSkybox : register(s0);

#define PI 3.14159265359f

float2 EquirectUVFromDirection(float3 dir)
{
    float phi = atan2(dir.z, dir.x);    // Azimuth : [-PI, PI]
    float theta = asin(dir.y);          // Elevation : [-PI/2, PI/2]
    
    float u = (phi / PI + 1.f) * 0.5f;
    float v = (theta / (PI * 0.5f) + 1.f) * 0.5f;
    
    return float2(u, 1.f - v);
}

float3 FaceToUVDirection(uint face, float2 uv)
{
    // uv : [0, 1] -> [-1, 1]
    float2 xy = uv * 2.0f - 1.0f;
    float3 dir = (float3)0;
    
    switch (face)
    {
        case 0: // +x
            dir = float3(1.f, xy.y, -xy.x);
            break;
        
        case 1: // -x
            dir = float3(-1.f, xy.y, xy.x);
            break;
        
        case 2: // +y
            dir = float3(xy.x, 1.f, -xy.y);
            break;
        
        case 3: // -y
            dir = float3(xy.x, -1.f, xy.y);
            break;
        
        case 4: // +z
            dir = float3(xy.x, xy.y, 1.0f);
            break;
        
        case 5:
            dir = float3(-xy.x, xy.y, -1.f);
            break;
    }
    
    return normalize(dir);
}

PSOutput PSMain(PSInput input)
{
    PSOutput output;
    
    // (x,y) -> Direction
    
    [unroll(6)]
    for (int i = 0; i < 6; i++)
    {
        float3 dir = FaceToUVDirection(i, input.texCoord);
    
        // Sample from original equirectangular image
        float2 eqUV = EquirectUVFromDirection(dir);
        float4 color = texSkybox.Sample(samplerSkybox, eqUV);
    
        switch (i)
        {
            case 0:
                output.target0 = color;
                break;
            case 1:
                output.target1 = color;
                break;
            case 2:
                output.target2 = color;
                break;
            case 3:
                output.target3 = color;
                break;
            case 4:
                output.target4 = color;
                break;
            case 5:
                output.target5 = color;
                break;
        }
    }
    
    
    return output;
}