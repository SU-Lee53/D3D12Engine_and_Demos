#pragma once

struct CBTranformData
{
    XMFLOAT4X4 matWorld;
};

struct CBCameraData
{
    XMFLOAT4X4 matView;
    XMFLOAT4X4 matProj;
};
