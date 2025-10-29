#include "groundwnd.h"
#include "repositorywnd.h"

RT_CLASS_EXPORTS_BEGIN(GroundWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(GroundWnd);

GroundWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GroundWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

void GroundWnd::PickUpAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* GroundWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GroundWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* GroundWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

GroundWnd::~GroundWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::SetupRepository()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::OnRepositoryChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::OnStartLevel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::GeomRepository* GroundWnd::GetGroundRepository() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

GroundWnd::GroundWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

GroundWnd::GroundWnd(GroundWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::UpdatePickupButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::OnAfterRemoveFromWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void GroundWnd::UpdateGroundPicture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int GroundWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
