#pragma once
#include <DirectXMath.h>

/////////////////////////////////////
// Directional Light (Blinn-Phong) //
/////////////////////////////////////

struct DirectionalLightDesc
{
	XMFLOAT3 lightPos;
	XMFLOAT3 lightDir;
	XMFLOAT3 lightColor;
};

/////////////////
// Point Light //
/////////////////

struct PointLightDesc
{
	XMFLOAT3 lightPos;
	XMFLOAT3 lightColor;

	float lightIntensity;
	float constantAttenuation;
	float linearAttenuation;
	float QuadraticAttenuation;
};

////////////////
// Spot Light //
////////////////

struct SpotLightDesc
{
	XMFLOAT3 lightPos;
	XMFLOAT3 lightDir;
	XMFLOAT3 lightColor;

	float lightIntensity;
	float innerCone;	// cos(еш) of inner cone 
	float outerCone;	// cos(еш) of outer cone 
	float lightRange;
	float attenuation;
};


///////////////////////
// Light Description //
///////////////////////

enum LIGHT_TYPE : UINT8
{
	LIGHT_TYPE_DIRECTIONAL = 0,
	LIGHT_TYPE_POINT_LIGHT,
	LIGHT_TYPE_SPOT_LIGHT
};

struct LightDesc
{
	union
	{
		DirectionalLightDesc DirectionalLight;
		SpotLightDesc SpotLight;
		PointLightDesc PointLight;
	};
};
