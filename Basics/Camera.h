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
        return CBCameraData{ XMMatrixTranspose(m_matView), XMMatrixTranspose(m_matProjection) };
    }

    void SetPosition(const Vec3& pos);
    void SetRotation(const Vec3& rot);

    void SetFovY(float fFovY);
    void SetAspect(float fWidth, float fHeight);
    void SetAspectRatio(float fRatio);
    void SetNear(float fNear);
    void SetFar(float fFar);

    std::shared_ptr<Transform> GetTransform() { return m_pTransform; }

private:
    BOOL SetCamera(const Vec3& camEYE, const Vec3& camAT, const Vec3& camUP);

private:
    std::shared_ptr<Transform> m_pTransform = nullptr;

    // Camera Parameters Cache
    Vec3 m_vCamEYE = Vec3::Zero;
    Vec3 m_vCamAT = Vec3::Zero;
    Vec3 m_vCamUP = Vec3::Zero;

    float m_ffovY = 0.f;
    float m_fAspectRatio = 0.f;
    float m_fNear = 0.f;
    float m_fFar = 0.f;

    Matrix m_matView        = Matrix::Identity;
    Matrix m_matProjection  = Matrix::Identity;

    BOOL m_bViewUpdated = FALSE;
    BOOL m_bProjUpdated = FALSE;
};

