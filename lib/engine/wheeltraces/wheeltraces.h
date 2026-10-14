#pragma once
#include <math/vector.h>
#include <renderer/i_renderer.h>
#include "retruxx/common.h"

struct Quaternion;

namespace m3d
{
    class Profiler;

    struct SkidQuad
    {
        CVector p1;
        CVector p2;
    };

    struct SkidStrip
    {
        int m_stripSize;
        m3d::SkidQuad m_last[2];
        bool m_binUse;
        int m_timeStamp;
        int m_lastFramestamp;
        int m_soilType;
        float m_texCoord;
        CVector m_boundCenter;
        float m_boundRadius;
        SkidStrip(const m3d::SkidStrip&);
        SkidStrip();
    };

    class WheelTraceMgr
    {
    private:
        m3d::SkidStrip* m_skidStrips;
        retruxx::map<void*, int, retruxx::less<void*>, retruxx::allocator<retruxx::pair<void* const, int> > > m_ownersToIdxMap;
        retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> > m_texHandles;
        m3d::rend::VbHandle m_vb;
        m3d::rend::IbHandle m_ib;
        m3d::rend::IEffect* m_shader;
        m3d::Profiler* m_profiler;

    public:
        WheelTraceMgr(const m3d::WheelTraceMgr&);
        WheelTraceMgr();
        ~WheelTraceMgr();
        void Init(int numSoilTypes);
        void Release();
        void ClearTraces();
        void AddTextureBySoilType(int soilType, const CStr& textureName);
        void Render();
        int StartSkidding(void* owner, int type);
        int EndSkidding(void* owner, bool smoothEnd);
        bool IsSkiddingStarted(void* owner);
        void AddTrace(const CVector& org, const Quaternion& quat, float scale, void* owner, int soilType, bool smoothStart);
    };
}
