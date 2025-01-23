#pragma once

class Transform;

class Camera
{
public:
    Camera();
    ~Camera();

public:
    BOOL Initialize();
    BOOL Update();

    void Resize(DWORD dwWidth, DWORD dwHeight);

public:
    void SetPosition(const XMFLOAT3& pos);
    void SetRotation(const XMFLOAT3& rot);

    void SetFovY(float fFovY);
    void SetAspect(float fWidth, float fHeight);
    void SetAspectRatio(float fRatio);
    void SetNear(float fNear);
    void SetFar(float fFar);

private:
    BOOL SetCamera(const XMFLOAT3& camEYE, const XMFLOAT3& camAT, const XMFLOAT3& camUP);

private:
    XMFLOAT3 m_vCamPosition;
    XMFLOAT3 m_vCamRotation;

    // Camera Parameters Cache
    XMFLOAT3 m_vCamEYE;
    XMFLOAT3 m_vCamAT;
    XMFLOAT3 m_vCamUP;

    float m_ffovY           = 0.f;
    float m_fAspectRatio    = 0.f;
    float m_fNear           = 0.f;
    float m_fFar            = 0.f;

    XMMATRIX m_matView;
    XMMATRIX m_matProjection;

    BOOL m_bViewUpdated = FALSE;
    BOOL m_bProjUpdated = FALSE;

};

