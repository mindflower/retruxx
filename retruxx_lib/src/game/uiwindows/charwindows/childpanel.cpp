#include "childpanel.h"

RT_CLASS_EXPORTS_BEGIN(ChildPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ChildPanel);

m3d::Class* ChildPanel::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ChildPanel::SetVehicleId(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ChildPanel::GetVehicleId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* ChildPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* ChildPanel::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle* ChildPanel::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

ChildPanel::~ChildPanel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* ChildPanel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ChildPanel::OnRestoreStyles()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ChildPanel::ChildPanel()
{
    m_vehicleId = -1;
    m_guiId = -1;
}

ChildPanel::ChildPanel(ChildPanel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ChildPanel::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ChildPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int ChildPanel::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void ChildPanel::OnExit()
{
    RETRUXX_NOT_IMPLEMENTED;
}
