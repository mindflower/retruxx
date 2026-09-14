#include <cameracontroller.h>
#include <stdexcept>

#include "config.h"
#include "core/kernel.h"
#include "core/timer.h"
#include "math/camera.h"
#include "retruxx/common.h"
#include "server/objects/base/globalproperties.h"

float GetFloatUnit()
{
    return (rand() % 1000) * 0.001;
};

namespace m3d
{
    void CameraController::AttachCamera(CCamera* camera)
    {
        m_camera = camera;
    }

    void CameraController::EnableShaking()
    {
        m_shakingAllowed = true;
    }

    void CameraController::Update()
    {
        auto const shakeLinearAmplitude = M3D_ENGINE_CFG.m_g_shakeLinearAmplitude.GetF();
        auto const shakeAngleAmplitude = M3D_ENGINE_CFG.m_g_shakeAngleAmplitude.GetF();

        auto pos = m_originPos - m_camera->m_worldOrigin;
        float distance = sqrt(pos.x * pos.x + pos.y * pos.y + pos.z * pos.z);
        if (distance <= m_originRadius)
        {
            auto timeSinceLastShake = M3D_KERNEL->GetTimer().GetCurTime() - m_lastShakeTimeStamp;
            if (m_lastShakeTimeStamp && timeSinceLastShake <= m_duration)
            {
                float distanceFactor = distance / this->m_originRadius;
                float effectiveDuration = this->m_duration * (1.0f - distanceFactor);
                if (timeSinceLastShake <= effectiveDuration && effectiveDuration > 0.001)
                {
                    float intensity = this->m_shakePower * (1.0f - distanceFactor);
                    float timeFactor = 1.0f - (static_cast<float>(timeSinceLastShake) / effectiveDuration);
                    float currentShakePower = intensity * timeFactor;

                    // Apply rolling shake (camera rotation)
                    float rollShake = currentShakePower * shakeAngleAmplitude;
                    this->m_rolling = (GetFloatUnit() - 0.5f) * rollShake * 2.0f;

                    // Apply translational shake
                    float translationShake = currentShakePower * shakeLinearAmplitude;
                    CVector shakeDirection(GetFloatUnit() - 0.5f, GetFloatUnit() - 0.5f, GetFloatUnit() - 0.5f);
                    shakeDirection.normalizeInplace();
                    m_translation = shakeDirection * translationShake;
                }
            }
            else
            {
                m_translation.zero();
                m_rolling = 0.0f;
                m_lastShakeTimeStamp = 0;
            }
        }
    }

    float CameraController::GetShakingRolling() const
    {
        return m_rolling;
    }

    void CameraController::DisableShaking()
    {
        m_shakingAllowed = false;
    }

    CVector const& CameraController::GetShakingTranslation() const
    {
        return m_translation;
    }

    void CameraController::DoTouch(CVector const& origin, float radius, float damage)
    {
        auto const x = origin.x - m_camera->m_worldOrigin.x;
        auto const y = origin.y - m_camera->m_worldOrigin.y;
        auto const z = origin.z - m_camera->m_worldOrigin.z;
        if (sqrt(x * x + y * y + z * z) <= radius)
        {
            m_originPos = origin;
            m_originRadius = radius;

            m_lastShakeTimeStamp = M3D_KERNEL->GetTimer().GetCurTime();

            if (damage < 0.0)
                damage = 0.0;
            if (damage > ai::theGlobProp.m_maxShakeDamage)
                damage = ai::theGlobProp.m_maxShakeDamage;

            m_shakePower = damage / ai::theGlobProp.m_maxShakeDamage;
            m_duration = ai::theGlobProp.m_shakeDamageToDurationCoeff * m_shakePower * 1000.0;
        }
    }
}  // namespace m3d
