#include <cameracontroller.h>
#include <stdexcept>

namespace m3d
{
    void CameraController::AttachCamera(CCamera*)
    {
        throw std::logic_error("Not implemented");
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
        throw std::logic_error("Not implemented");
    }

    CameraController::CameraController()
    {
        throw std::logic_error("Not implemented");
    }

    void CameraController::DisableShaking()
    {
        throw std::logic_error("Not implemented");
    }

    CVector const& CameraController::GetShakingTranslation() const
    {
        throw std::logic_error("Not implemented");
    }

    void CameraController::DoTouch(CVector const&, float, float)
    {
        throw std::logic_error("Not implemented");
    }
}
