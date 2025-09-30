#include <cameracontroller.h>
#include <stdexcept>

namespace m3d
{
    void CameraController::AttachCamera(CCamera* camera)
    {
        m_camera = camera;
    }

    void CameraController::EnableShaking()
    {
        throw std::logic_error("Not implemented");
    }

    void CameraController::Update()
    {
        throw std::logic_error("Not implemented");
    }

    float CameraController::GetShakingRolling() const
    {
        return m_rolling;
    }

    void CameraController::DisableShaking()
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& CameraController::GetShakingTranslation() const
    {
        return m_translation;
    }

    void CameraController::DoTouch(CVector const&, float, float)
    {
        throw std::logic_error("Not implemented");
    }
}
