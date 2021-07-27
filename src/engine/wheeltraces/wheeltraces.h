#pragma once
#include <map>
#include <vector>
#include <math/vector.h>
#include <renderer/i_renderer.h>

namespace m3d
{
    class Profiler;

    class SkidQuad
    {
    private:
        CVector p1;
        CVector p2;
    };

    class SkidStrip
    {
    public:
        SkidStrip();

    private:
        int m_stripSize;
        SkidQuad m_last[2];
        bool m_binUse;
        int m_timeStamp;
        int m_lastFramestamp;
        int m_soilType;
        float m_texCoord;
        CVector m_boundCenter;
        float m_boundRadius;
    };

    class WheelTraceMgr
    {
    public:
        void Release();
        void Render();
        ~WheelTraceMgr();
        int EndSkidding(void*, bool);
        int StartSkidding(void*, int);
        void ClearTraces();
        WheelTraceMgr();
        void Init();
        bool IsSkiddingStarted(void*);
        void AddTrace(CVector const&, Quaternion const&, float, void*, int, bool);
        void AddTextureBySoilType(int, CStr const&);

    private:
        SkidStrip* m_skidStrips;
        std::map<void*, int> m_ownersToIdxMap;
        std::vector<rend::TexHandle> m_texHandles;
        rend::VbHandle m_vb;
        rend::IbHandle m_ib;
        rend::IEffect* m_shader;
        Profiler* m_profiler;
    };
}
