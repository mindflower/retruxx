#include "groundwnd.h"
#include "repositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(GroundWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GroundWnd);

GroundWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GroundWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

void GroundWnd::PickUpAll()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* GroundWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GroundWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* GroundWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

GroundWnd::~GroundWnd()
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::OnAfterAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::SetupRepository()
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::OnRepositoryChanged(void*)
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::OnStartLevel()
{
    throw std::logic_error("Not implemented");
}

ai::GeomRepository* GroundWnd::GetGroundRepository() const
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

GroundWnd::GroundWnd()
{
    throw std::logic_error("Not implemented");
}

GroundWnd::GroundWnd(GroundWnd const&)
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::UpdatePickupButtonState()
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::GameDataSetup()
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::OnAfterRemoveFromWndStation()
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::OnBeforeAddToWndStation()
{
    throw std::logic_error("Not implemented");
}

void GroundWnd::UpdateGroundPicture()
{
    throw std::logic_error("Not implemented");
}

int GroundWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}
