#pragma once

struct CBTransformData
{
    XMFLOAT4X4 matWorld;
};

struct CBCameraData
{
    XMFLOAT4X4 matView;
    XMFLOAT4X4 matProj;
    XMFLOAT3 CamPosition;
};

////////////
// Lights //
////////////
#include "LightDesc.h"
#define MAX_LIGHT_COUNT 10

struct RawLightData // 16 * 4 = 64byte
{
    XMFLOAT4 data1;
    XMFLOAT4 data2;
    XMFLOAT4 data3;
    XMFLOAT2 data4;
    int type;
    BOOL bEnable;
};

struct CBLightData
{
    RawLightData lightData[MAX_LIGHT_COUNT];
    int nLights = 0;
};