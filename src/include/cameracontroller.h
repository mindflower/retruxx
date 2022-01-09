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
        void DisableShaking();
        CVector const & GetShakingTranslation() const ;
        void DoTouch(CVector const &,float,float);

    private:
        float m_minShakingRange = 50.0;
        float m_maxRollingAmplitude;
        float m_shakePower = 1.0;
        int m_duration;
        bool m_shakingAllowed = true;
        CCamera *m_camera = nullptr;
        int m_lastShakeTimeStamp = 0;
        CVector m_originPos;
        float m_originRadius;
        CVector m_translation{0.0, 0.0, 0.0};
        float m_rolling = 0.0;
    };
}
