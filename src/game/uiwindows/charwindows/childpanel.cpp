#include "childpanel.h"

RT_CLASS_DEFINE(ChildPanel);

m3d::Class* ChildPanel::GetClass() const
{
    throw std::logic_error("Not implemented");
}

void ChildPanel::SetVehicleId(int)
{
    throw std::logic_error("Not implemented");
}

int ChildPanel::GetVehicleId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Class* ChildPanel::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ChildPanel::CreateObject()
{
    throw std::logic_error("Not implemented");
}

ai::Vehicle* ChildPanel::GetVehicle() const
{
    throw std::logic_error("Not implemented");
}

ChildPanel::~ChildPanel()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* ChildPanel::Clone()
{
    throw std::logic_error("Not implemented");
}

void ChildPanel::OnRestoreStyles()
{
    throw std::logic_error("Not implemented");
}

ChildPanel::ChildPanel()
{
    throw std::logic_error("Not implemented");
}

ChildPanel::ChildPanel(ChildPanel const&)
{
    throw std::logic_error("Not implemented");
}

int ChildPanel::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int ChildPanel::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int ChildPanel::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

void ChildPanel::OnExit()
{
    throw std::logic_error("Not implemented");
}
