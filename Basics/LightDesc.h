#pragma once
#include <DirectXMath.h>

/////////////////////////////////////
// Directional Light (Blinn-Phong) //
/////////////////////////////////////

struct DirectionalLightData
{
	XMFLOAT3 lightPos;
	XMFLOAT3 lightDir;
	XMFLOAT3 lightColor;
};

/////////////////
// Point Light //
/////////////////

struct PointLightData
{
	XMFLOAT3 lightPos;
	float lightIntensity;
	XMFLOAT3 lightColor;
	float constantAttenuation;

	float linearAttenuation;
	float QuadraticAttenuation;
};

////////////////
// Spot Light //
////////////////

struct SpotLightData
{
	XMFLOAT3 lightPos;
	float lightIntensity;
	XMFLOAT3 lightDir;
	float innerCone;	// cos(еш) of inner cone 
	XMFLOAT3 lightColor;
	float outerCone;	// cos(еш) of outer cone 

	float lightRange;
	float attenuation;
};


///////////////////////
// Light Description //
///////////////////////

enum LIGHT_TYPE : int
{
	LIGHT_TYPE_DIRECTIONAL = 0,
	LIGHT_TYPE_POINT_LIGHT,
	LIGHT_TYPE_SPOT_LIGHT
};

struct UnifiedLightData
{
	union
	{
		DirectionalLightData DirectionalLight;
		SpotLightData SpotLight;
		PointLightData PointLight;
	};

	LIGHT_TYPE LightType;
};
