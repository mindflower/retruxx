#include "wheeltraces.h"
#include <stdexcept>

namespace m3d
{
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
        throw std::logic_error("Not implemented");
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
