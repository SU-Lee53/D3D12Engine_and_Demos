#pragma once

struct CB_TRANSFORM
{
	XMFLOAT4X4 matLocal;
	XMFLOAT4X4 matWorld;
};

struct CB_CAMERA
{
    XMFLOAT4X4 matView;
    XMFLOAT4X4 matProj;
};