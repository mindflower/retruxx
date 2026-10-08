#include "childpanel.h"

#include <game/m3dgame.h>
#include <server/objects/vehicle.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(ChildPanel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(ChildPanel);

m3d::Class* ChildPanel::GetClass() const
{
    return RT_CLASS_LOCAL(ChildPanel);
}

void ChildPanel::SetVehicleId(int vehicleId)
{
    m_vehicleId = vehicleId;
}

int ChildPanel::GetVehicleId() const
{
    return m_vehicleId;
}

m3d::Class* ChildPanel::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* ChildPanel::CreateObject()
{
    return new ChildPanel();
}

ai::Vehicle* ChildPanel::GetVehicle() const
{
    if (m_vehicleId == -1)
    {
        return nullptr;
    }

    auto* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        return static_cast<ai::Vehicle*>(obj);
    }
    return nullptr;
}

ChildPanel::~ChildPanel() = default;

m3d::Object* ChildPanel::Clone()
{
    return new ChildPanel();
}

void ChildPanel::OnRestoreStyles()
{
}

ChildPanel::ChildPanel()
{
    m_vehicleId = -1;
    m_guiId = -1;
}

ChildPanel::ChildPanel(ChildPanel const&) : ChildPanel()
{
}

int ChildPanel::GameDataClear(bool)
{
    SetVehicleId(-1);
    return 1;
}

int ChildPanel::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    if ((m_style & m3d::ui::WS_REFLECT_NOTIFY_MESSAGES_TO_PARENT) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }

    if (id != 1 || msg != 1)
    {
        return 0;
    }

    OnExit();
    return 1;
}

int ChildPanel::GameDataUpdate(void*, int)
{
    return 1;
}

void ChildPanel::OnExit()
{
    if (IsChildOf(M3D_APP))
    {
        M3D_APP->EnqueueMessage(65673, m_guiId, 0, 0, 0, {}, {});
    }
}
