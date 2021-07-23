#pragma once
#include <math/vector.h>

class CCamera;

namespace m3d
{
    class CameraController
    {
    public:
        void AttachCamera(CCamera *);
        void EnableShaking();
        void Update();
        float GetShakingRolling() const ;
        CameraController();
        void DisableShaking();
        CVector const & GetShakingTranslation() const ;
        void DoTouch(CVector const &,float,float);

    private:
        float m_minShakingRange;
        float m_maxRollingAmplitude;
        float m_shakePower;
        int m_duration;
        bool m_shakingAllowed;
        CCamera *m_camera;
        int m_lastShakeTimeStamp;
        CVector m_originPos;
        float m_originRadius;
        CVector m_translation;
        float m_rolling;
    };
}
