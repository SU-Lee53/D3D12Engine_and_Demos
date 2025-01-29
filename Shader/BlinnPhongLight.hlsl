

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

cbuffer LambertLightData : register(b3)
{
    float3 lightPos;
    float3 lightDir;
    float4 lightColor;
};

struct VSInput
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

struct PSInput
{
    float4 Pos : SV_POSITION;
    float3 WorldPos : POSITION1;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 BiNormal : BINORMAL;
    float3 Tangent : TANGENT;
};

PSInput VSMain(VSInput input)
{
    PSInput output = (PSInput) 0;
    
    float4 worldPos = mul(input.Pos, matWorld);
    matrix vp = mul(matView, matProj);
    output.Pos = mul(worldPos, vp);
    output.WorldPos = worldPos.xyz;
    
    output.Normal = mul(input.Normal, (float3x3)(matWorld));
    output.BiNormal = mul(input.BiNormal, (float3x3)(matWorld));
    output.Tangent = mul(input.Tangent, (float3x3)(matWorld));
    
    output.TexCoord = input.TexCoord;
    output.Color = input.Color;
    
    return output;
}

//       Half vector(L + V == H)    -> in this situation : N == normalized H -> Most bright pixel
//      light(L)  (N)   camera(V)
//            \    |    /
//             \   |   /
//              \  |  / 
//               \ | /
//                \|/
//  ----------------------------------
    
float4 PSMain(PSInput input) : SV_TARGET
{
    float3 N = normalize(input.Normal);                 // Normalized normal vector
    float3 L = normalize(lightPos - input.WorldPos);    // Noralized vector to light
    float3 V = normalize(camPos - input.WorldPos);      // Normalized vector to camera
    float3 H = normalize(L + V);                        // Normalized half vector
    
    // Diffuse(Lambert)
    float diffuseIntensity = max(0.0f, dot(N, L));
    float3 diffuse = diffuseIntensity * lightColor.rgb * colorDiffuse.rgb;
    
    // Blinn-Phong
    float shininess = max(1.0, colorSpecular.a);
    float specularIntensity = pow(max(0.0f, dot(N, H)), shininess);
    float3 specular = specularIntensity * lightColor.rgb * colorSpecular.rgb;
    
    // Ambient
    float3 ambient = colorAmbient.rgb * lightColor.rgb;
    
    float3 finalColor = diffuse + specular + ambient;
    return float4(finalColor, 1.0f);
    
}

    