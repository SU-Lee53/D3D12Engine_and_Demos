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

        return CBCameraData{ viewTransposed, projTransposed, m_pTransform->GetWorldPosition()};
    }

    void SetPosition(const XMFLOAT3& pos);
    void SetRotation(const XMFLOAT3& rot);
    
    XMFLOAT3 GetPosition() { return m_pTransform->GetWorldPosition(); }
    XMFLOAT3 GetRotation() { return m_pTransform->GetWorldRotation(); }

    void SetFovY(float fFovY);
    void SetAspect(float fWidth, float fHeight);
    void SetAspectRatio(float fRatio);
    void SetNear(float fNear);
    void SetFar(float fFar);

    std::shared_ptr<Transform> GetTransform() { return m_pTransform; }

    XMFLOAT3 GetEYE() { return m_vCamEYE; }
    XMFLOAT3 GetAT() { return m_vCamAT; }
    XMFLOAT3 GetUP() { return m_vCamUP; }

public:
    void DecomposeCameraUVN(XMFLOAT3& outFront, XMFLOAT3& outRight, XMFLOAT3& outUp)
    {
        XMMATRIX xmWorld = XMLoadFloat4x4(&m_pTransform->GetWorldMatrix());
        XMStoreFloat3(&outFront, xmWorld.r[2]); // Front is in 3rd row
        XMStoreFloat3(&outRight, xmWorld.r[0]); // Right is in 1st row
        XMStoreFloat3(&outUp, xmWorld.r[1]); // UP is in 2nd row
    }

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

private:
    // Variable for move camera
    float m_fRotY = 0.f;

    // mouse
    POINT m_MousePoint;
    float m_fDeltaX = 0.f;
    float m_fDeltaY = 0.f;

    float m_sensitivity = 1.0f;

};

