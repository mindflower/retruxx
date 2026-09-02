#include "weaponslotlist.h"
#include "weaponslotwnd.h"

#include <game/m3dgame.h>
#include <server/objects/vehicle.h>
#include <server/objects/base/objcontainer.h>

RT_CLASS_EXPORTS_BEGIN(WeaponSlotList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(WeaponSlotList);

WeaponSlotList::AuxInfo::AuxInfo()
{
    m_space = -2.0;
}

m3d::Class* WeaponSlotList::GetClass() const
{
    return RT_CLASS_LOCAL(WeaponSlotList);
}

WeaponSlotList::~WeaponSlotList()
{
    ClearItems();
    // m_items (vector of ref_ptr) releases its remaining references itself.
    // TODO: the binary also drops the shared WeaponSlotWnd / WeaponGroupButtonList
    // pattern refs here.
}

int WeaponSlotList::SetupForVehicle(int vehicleId)
{
    m_vehicleId = vehicleId;
    int result = 1;
    if (vehicleId != -1 && !GetVehicle())
    {
        m_vehicleId = -1;
        result = 0;
    }
    // TODO: rebuild the slot list (CreateItems()) when already parented to the station.
    return result;
}

m3d::Class* WeaponSlotList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* WeaponSlotList::CreateObject()
{
    return new WeaponSlotList;
}

m3d::Object* WeaponSlotList::Clone()
{
    return new WeaponSlotList(*this);
}

void WeaponSlotList::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

WeaponSlotList::WeaponSlotList()
{
    m_vehicleId = -1;
}

WeaponSlotList::WeaponSlotList(WeaponSlotList const&) : WeaponSlotList()
{
}

void WeaponSlotList::GetGunPartNames(int, std::vector<CStr, std::allocator<CStr>>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataClear(bool)
{
    m_vehicleId = -1;
    // TODO: rebuild the slot list (CreateItems()) when parented to the station.
    return 1;
}

int WeaponSlotList::OnAfterRemoveFromWndStation()
{
    int const result = m3d::ui::Wnd::OnAfterRemoveFromWndStation();
    ClearItems();
    return result;
}

void WeaponSlotList::ClearItems()
{
    for (auto& item : m_items)
    {
        if (item)
        {
            M3D_APP->m_pInterfaceManager->RemoveWindow(item->GetGuiId());
        }
    }
    m_items.clear();
}

int WeaponSlotList::AddItem(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void WeaponSlotList::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* WeaponSlotList::GetVehicle() const
{
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    if (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle))
    {
        return static_cast<ai::Vehicle const*>(obj);
    }
    return nullptr;
}

void WeaponSlotList::SortGunPartNames(std::vector<CStr, std::allocator<CStr>>&, ai::Vehicle const*) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataSetup()
{
    // TODO: implement WeaponSlotList::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

void WeaponSlotList::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int WeaponSlotList::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 65)
    {
        OnVehiclePartChanged(data);
    }
    return 1;
}

void WeaponSlotList::RecalcLayot()
{
    RETRUXX_NOT_IMPLEMENTED;
}
