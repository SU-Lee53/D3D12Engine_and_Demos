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

struct CBLightData
{
    XMFLOAT4 data1;
    XMFLOAT4 data2;
    XMFLOAT4 data3;
    XMFLOAT4 data4;
    XMFLOAT4 data5;

    LIGHT_TYPE desc;
};