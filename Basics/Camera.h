#pragma once

class Camera
{
public:
    Camera();
    ~Camera();

public:
    BOOL Initialize();

public:
    CBCameraData GetCameraCBData()
    {
        return CBCameraData{ XMMatrixTranspose(m_matView), XMMatrixTranspose(m_matProjection) };
    }

private:
    BOOL SetCamera(const Vec3& camPos, const Vec3& camDir, const Vec3& camUp);

private:
    Vec3 m_vCamPos = Vec3::Zero;
    Vec3 m_vCamDir = Vec3::Zero;
    Vec3 m_vCamUp  = Vec3::Zero;

    float m_ffovY = 0.f;
    float m_fAspectRatio = 0.f;
    float m_fNear = 0.f;
    float m_fFar = 0.f;

    Matrix m_matView        = Matrix::Identity;
    Matrix m_matProjection  = Matrix::Identity;


};

