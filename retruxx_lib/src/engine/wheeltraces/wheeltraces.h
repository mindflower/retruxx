#pragma once
#include <math/vector.h>
#include <renderer/i_renderer.h>
#include "thirdparty/containers.h"

struct Quaternion;

namespace m3d
{
    class Profiler;

    class SkidQuad
    {
    private:
        CVector p1;
        CVector p2;
    };

    struct SkidStrip
    {
        /* 0x0000 */ int m_stripSize;
        /* 0x0004 */ m3d::SkidQuad m_last[2];
        /* 0x0034 */ bool m_binUse;
        /* 0x0035 */ char Padding_87[3];
        /* 0x0038 */ int m_timeStamp;
        /* 0x003c */ int m_lastFramestamp;
        /* 0x0040 */ int m_soilType;
        /* 0x0044 */ float m_texCoord;
        /* 0x0048 */ CVector m_boundCenter;
        /* 0x0054 */ float m_boundRadius;
        SkidStrip(const m3d::SkidStrip&);
        SkidStrip();
    }; /* size: 0x0058 */

    class WheelTraceMgr
    {
    private:
        /* 0x0000 */ m3d::SkidStrip* m_skidStrips;
        /* 0x0004 */ retruxx::map<void*, int, retruxx::less<void*>, retruxx::allocator<retruxx::pair<void* const, int> > > m_ownersToIdxMap;
        /* 0x0010 */ retruxx::vector<m3d::rend::TexHandle, retruxx::allocator<m3d::rend::TexHandle> > m_texHandles;
        /* 0x0020 */ m3d::rend::VbHandle m_vb;
        /* 0x0024 */ m3d::rend::IbHandle m_ib;
        /* 0x0028 */ m3d::rend::IEffect* m_shader;
        /* 0x002c */ m3d::Profiler* m_profiler;

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
    }; /* size: 0x0030 */

    static_assert(sizeof(WheelTraceMgr) == 0x0030);
}
