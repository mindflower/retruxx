#include "wheeltraces.h"
#include <stdexcept>

#include "m3dapp.h"
#include "core/kernel.h"

namespace m3d
{
    SkidStrip::SkidStrip()
    {
        this->m_lastFramestamp = 0;
        this->m_binUse = 0;
        this->m_timeStamp = 0;
        this->m_soilType = 0;
        this->m_stripSize = 0;
        this->m_texCoord = 0.0;
        this->m_boundCenter = {0.0, 0.0, 0.0};
        this->m_boundRadius = 0.0;
    }

    void WheelTraceMgr::Release()
    {
        throw std::logic_error("Not implemented");
    }

    void WheelTraceMgr::Render()
    {
        throw std::logic_error("Not implemented");
    }

    WheelTraceMgr::~WheelTraceMgr()
    {
        throw std::logic_error("Not implemented");
    }

    int WheelTraceMgr::EndSkidding(void*, bool)
    {
        throw std::logic_error("Not implemented");
    }

    int WheelTraceMgr::StartSkidding(void*, int)
    {
        throw std::logic_error("Not implemented");
    }

    void WheelTraceMgr::ClearTraces()
    {
        throw std::logic_error("Not implemented");
    }

    WheelTraceMgr::WheelTraceMgr()
    {
        //TODO: check this
        m_skidStrips = new SkidStrip;
        this->m_vb = M3D_RENDERER->AddVb(
            rend::VERTEX_XYZCT1,
            0x10000,
            "WheelTrace",
            0);
        this->m_ib = M3D_RENDERER->AddIb(130, 0);
        //TODO: check this
        auto ibPtr = static_cast<WORD*>(M3D_RENDERER->LockIb(this->m_ib, 0, 0, 0));
        for (int i = 0; i < 130; ++i)
            ibPtr[i] = i;
        M3D_RENDERER->UnlockIb(this->m_ib);
        this->m_shader = M3D_RENDERER->NewEffect("data/shaders/wheeltrace.fx", true);
        M3D_ASSERT(m_shader);
        this->m_shader->SetDefaultTechnique(true);
        m_profiler = m3d::Application::g_pApp->GetProfilerStack().GetProfiler(m3d::Application::g_pApp->GetProfilerStack().AddProfiler("wheeltraces", 0x1Eu));
    }

    void WheelTraceMgr::Init()
    {
        throw std::logic_error("Not implemented");
    }

    bool WheelTraceMgr::IsSkiddingStarted(void*)
    {
        throw std::logic_error("Not implemented");
    }

    void WheelTraceMgr::AddTrace(CVector const&, Quaternion const&, float, void*, int, bool)
    {
        throw std::logic_error("Not implemented");
    }

    void WheelTraceMgr::AddTextureBySoilType(int, CStr const&)
    {
        throw std::logic_error("Not implemented");
    }
}
