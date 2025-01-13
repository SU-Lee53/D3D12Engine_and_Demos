#pragma once

struct CBTranformData
{
    XMMATRIX matWorld;
};

struct CBCameraData
{
    XMMATRIX matView;
    XMMATRIX matProj;
};
