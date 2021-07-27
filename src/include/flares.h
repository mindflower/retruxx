#pragma once

class CVector;

namespace m3d
{
    enum FlareMode
    {
        FLARE_SUN = 0x1,
        FLARE_FLASH_NO_TEST = 0x2,
        FLARE_TEST = 0x4,
        FLARE_BLINDING = 0x8,
        FLARE_SUN_FLASH = 0xE,

    };
    class CFlare
    {
    public:
        ~CFlare();
        static int __fastcall Init();
        static void __fastcall Release();
        CFlare();
        int Render(FlareMode, CVector const&, float, float);

    private:
        float m_ssx;
        float m_ssy;
        bool m_onScreen;
        float m_percentageVisible;
        unsigned int m_event;
        int m_glowOnScreen;
    };
}
