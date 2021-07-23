#pragma once

namespace m3d
{
    class IImpulse
    {
    public:
        virtual ~IImpulse();

    private:
        //m3d::IImpulse_vtbl* __vftable /*VFT*/;
    };

    class AuxImpulseInfo
    {
    public:
        void UnpackXy(float*, float*, float*, float*) const;
        AuxImpulseInfo(int, bool, int, unsigned int, unsigned int);
        float UnpackWheel() const;

    private:
        int m_impId;
        bool m_state;
        int m_gameMode;
        unsigned int m_info0;
        unsigned int m_info1;
    };
}
