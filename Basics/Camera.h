#pragma once


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
    CBCameraData GetCameraCBData()
    {
        XMMATRIX xmMatView = XMLoadFloat4x4(&m_matView);
        XMMATRIX xmMatProj = XMLoadFloat4x4(&m_matProjection);

        xmMatView = XMMatrixTranspose(xmMatView);
        xmMatProj = XMMatrixTranspose(xmMatProj);

        XMFLOAT4X4 viewTransposed;
        XMFLOAT4X4 projTransposed;

        XMStoreFloat4x4(&viewTransposed, xmMatView);
        XMStoreFloat4x4(&projTransposed, xmMatProj);

        return CBCameraData{ viewTransposed, projTransposed };
    }

    void SetPosition(const XMFLOAT3& pos);
    void SetRotation(const XMFLOAT3& rot);

    void SetFovY(float fFovY);
    void SetAspect(float fWidth, float fHeight);
    void SetAspectRatio(float fRatio);
    void SetNear(float fNear);
    void SetFar(float fFar);

    std::shared_ptr<Transform> GetTransform() { return m_pTransform; }

private:
    BOOL SetCamera(const XMFLOAT3& camEYE, const XMFLOAT3& camAT, const XMFLOAT3& camUP);

private:
    std::shared_ptr<Transform> m_pTransform = nullptr;

    // Camera Parameters Cache
    XMFLOAT3 m_vCamEYE;
    XMFLOAT3 m_vCamAT;
    XMFLOAT3 m_vCamUP;

    float m_ffovY = 0.f;
    float m_fAspectRatio = 0.f;
    float m_fNear = 0.f;
    float m_fFar = 0.f;

    XMFLOAT4X4 m_matView        = Matrix::Identity;
    XMFLOAT4X4 m_matProjection  = Matrix::Identity;

    BOOL m_bViewUpdated = FALSE;
    BOOL m_bProjUpdated = FALSE;
};

