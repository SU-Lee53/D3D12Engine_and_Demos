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

    XMFLOAT3 GetPosition() { return m_vCamPosition; }
    XMFLOAT3 GetRotation() { return m_vCamRotation; }

    XMFLOAT4X4 GetViewMatrix() { return m_matView; }
    XMFLOAT4X4 GetProjectionMatrix() { return m_matProjection; }

    XMFLOAT3 GetCameraFront()
    {
        XMFLOAT3 ret;
        XMMATRIX xmWorld = XMLoadFloat4x4(&m_matCamWorld);
        XMStoreFloat3(&ret, xmWorld.r[2]);
        return ret;
    }

    XMFLOAT3 GetCameraRight()
    {
        XMFLOAT3 ret;
        XMMATRIX xmWorld = XMLoadFloat4x4(&m_matCamWorld);
        XMStoreFloat3(&ret, xmWorld.r[0]);
        return ret;
    }

    XMFLOAT3 GetCameraUp()
    {
        XMFLOAT3 ret;
        XMMATRIX xmWorld = XMLoadFloat4x4(&m_matCamWorld);
        XMStoreFloat3(&ret, xmWorld.r[1]);
        return ret;
    }

    void DecomposeCameraUVN(XMFLOAT3& outFront, XMFLOAT3& outRight, XMFLOAT3& outUp)
    {
        XMMATRIX xmWorld = XMLoadFloat4x4(&m_matCamWorld);
        XMStoreFloat3(&outFront, xmWorld.r[2]); // Front is in 3rd row
        XMStoreFloat3(&outRight, xmWorld.r[0]); // Right is in 1st row
        XMStoreFloat3(&outUp, xmWorld.r[1]); // UP is in 2nd row
    }

private:
    BOOL SetCamera(const XMFLOAT3& camEYE, const XMFLOAT3& camAT, const XMFLOAT3& camUP);

private:
    XMFLOAT4X4 CreateWorldMatrix();

private:
    XMFLOAT3    m_vCamPosition;
    XMFLOAT3    m_vCamRotation;
    XMFLOAT4X4  m_matCamWorld;

    // Camera Parameters Cache
    XMFLOAT3 m_vCamEYE;
    XMFLOAT3 m_vCamAT;
    XMFLOAT3 m_vCamUP;

    float m_ffovY           = 0.f;
    float m_fAspectRatio    = 0.f;
    float m_fNear           = 0.f;
    float m_fFar            = 0.f;

    XMFLOAT4X4 m_matView;
    XMFLOAT4X4 m_matProjection;

    BOOL m_bViewUpdated = FALSE;
    BOOL m_bProjUpdated = FALSE;

};

