#include "playervehiclewnd.h"

#include <utility>
#include <vector>

#include <core/kernel.h>
#include <core/log.h>
#include <game/m3dgame.h>
#include "game/uimisc/guihelper.h"
#include "game/uimanager/truxxuimanager.h"

#include <game/uiwindows/mainwindows/damageinfownd.h>
#include <game/uiwindows/commonwindows/complexmodelwnd.h>
#include <ui/button.h>

#include <server/objects/player.h>
#include <server/objects/vehicle.h>
#include <server/objects/gadget.h>
#include <server/objects/physicbodies/vehiclepart.h>
#include <server/objects/physicbodies/physicbody.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/resourcemanager.h>
#include <server/geomrepository.h>
#include <server/geomrepositoryitem.h>
#include <server/izvratrepository.h>

RT_CLASS_EXPORTS_BEGIN(PlayerVehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(PlayerVehicleWnd);

PlayerVehicleWnd::PAuxInfo::PAuxInfo()
{
    // RVA 0x65EF0
    m_tabBtnSz.x = 158.0f;
    m_tabBtnSz.y = 32.0f;
    m_tabBtnSpace = 0.0f;

    m_tabButtonNames[0] = "tabBtnVehicleCharacteristics";
    m_tabButtonNames[1] = "tabBtnCBCharacteristics";
    m_tabButtonNames[2] = "tabBtnWeaponGroups";
}

m3d::Class* PlayerVehicleWnd::GetClass() const
{
    // RVA 0x65EB0
    return RT_CLASS_LOCAL(PlayerVehicleWnd);
}

m3d::Class* PlayerVehicleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(VehicleWnd);
}

m3d::Object* PlayerVehicleWnd::CreateObject()
{
    // RVA 0x65EC0
    return new PlayerVehicleWnd;
}

m3d::Object* PlayerVehicleWnd::Clone()
{
    // RVA 0x65E40
    return new PlayerVehicleWnd(*this);
}

PlayerVehicleWnd::PlayerVehicleWnd()
{
    // RVA 0x65FF0
    m_hackedVehicleId = -1;
    m_tabButtons[0] = nullptr;
    m_tabButtons[1] = nullptr;
    m_tabButtons[2] = nullptr;
}

PlayerVehicleWnd::PlayerVehicleWnd(PlayerVehicleWnd const& rhs) : VehicleWnd(rhs)
{
    // RVA 0x66130 - fresh window; m_paif is re-defaulted, m_wndDamageInfo null.
    m_hackedVehicleId = -1;
    m_tabButtons[0] = nullptr;
    m_tabButtons[1] = nullptr;
    m_tabButtons[2] = nullptr;
}

PlayerVehicleWnd::~PlayerVehicleWnd()
{
    // RVA 0x66160 - m_paif and the m_wndDamageInfo ref_ptr release themselves.
}

// ============================================================================
//  Setup / teardown
// ============================================================================

int PlayerVehicleWnd::GameDataSetup()
{
    // RVA 0x661A0
    if (!VehicleWnd::GameDataSetup())
    {
        return 0;
    }

    int res = 1;
    if ((m_gameDataFlags & 2) == 0)
    {
        // The damage-info panel lives in the interface manager; window id 27 for
        // the main char screen, 28 for the workshop variant.
        ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(27 + (m_guiId != 68 ? 1 : 0));
        if (w && w->IsKindOf(&DamageInfoWnd::m_classDamageInfoWnd))
        {
            m_wndDamageInfo = static_cast<DamageInfoWnd*>(w.get());
            AddChild(w.get());
        }
        else
        {
            res = 0;
        }

        // Three tab buttons, pulled by name from the dialog resource.
        for (int i = 0; i < 3; ++i)
        {
            m3d::Object* child = GetChildByName(m_paif.m_tabButtonNames[i]);
            if (child && child->IsKindOf(&m3d::ui::ButtonWnd::m_classButtonWnd))
            {
                m_tabButtons[i] = static_cast<m3d::ui::ButtonWnd*>(child);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + m_paif.m_tabButtonNames[i]
                    + " is not found or incorrect type");
                res = 0;
            }
        }

        if (res)
        {
            ShowTab(TAB_ID_VEHICLE_CHARACTERISTICS);

            // Register the 3d model window (id 74 / 75) and keep it on top.
            int const modelWndId = 74 + (m_guiId != 68 ? 1 : 0);
            res = M3D_APP->m_pInterfaceManager->AddWindowById(m_wndVehicleModel.get(), modelWndId, true, false) & res;
            MoveChildToLastPosition(m_wndVehicleModel.get());

            if (res)
            {
                SetVehicleType(m_vehicleType);
            }
            else
            {
                m_gameDataFlags &= ~1u;
            }
        }
        else
        {
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("PlayerVehicleWnd: error - fail to init because of a bad resource");
    return 0;
}

int PlayerVehicleWnd::OnAfterAddToWndStation()
{
    // RVA 0x66F20
    int const r = m3d::ui::Wnd::OnAfterAddToWndStation();
    if (m_guiId == 69)
    {
        M3D_APP->EnqueueMessage(65691, 0, 0, 0, 0, {}, {});
    }
    return r;
}

// ============================================================================
//  Data flow
// ============================================================================

int PlayerVehicleWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x66500
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }

    if (dataType == 64)
    {
        if (m_vehicleType != VEHICLE_HACKED)
        {
            ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
            if (vehicle)
            {
                SetVehicleId(vehicle->GetId());
                return 1;
            }
            SetVehicleId(-1);
        }
    }
    else if (dataType == 107)
    {
        if (data)
        {
            // NOTE: the shipped build passes the hacked vehicle-part obj id
            // packed directly into the data pointer.
            SetupForHackedVehiclePart(reinterpret_cast<int>(data));
            return 1;
        }
    }
    else
    {
        return VehicleWnd::GameDataUpdate(data, dataType);
    }
    return 1;
}

void PlayerVehicleWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x665A0
    if ((m_gameDataFlags & 1) != 0)
    {
        VehicleWnd::SetVehicleId(vehicleId);
    }
}

void PlayerVehicleWnd::SetVehicleType(VehicleWnd::VehicleType type)
{
    // RVA 0x665C0
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    VehicleWnd::SetVehicleType(type);

    if (m_vehicleType == VEHICLE_NORMAL)
    {
        if (m_wndDamageInfo && !IsDirectChild(m_wndDamageInfo.get()))
        {
            AddChild(m_wndDamageInfo.get());
        }
        MoveChildToLastPosition(m_wndVehicleModel.get());
    }
    else if (m_vehicleType == VEHICLE_HACKED)
    {
        if (m_wndDamageInfo && IsDirectChild(m_wndDamageInfo.get()))
        {
            RemoveChild(m_wndDamageInfo.get());
        }
    }
}

// ============================================================================
//  Tabs
// ============================================================================

int PlayerVehicleWnd::OnWndNotify(m3d::ui::Wnd* from, unsigned id, unsigned msg, m3d::AIParam const& data)
{
    // RVA 0x66CD0
    if ((m_style & 0x100000) != 0)
    {
        ReflectChildNotifyToParent(from, id, msg, data);
    }

    if (id == 1 && msg == 1)
    {
        OnExit();
        return 1;
    }
    if (id == 300351)
    {
        if (msg == 1)
        {
            ShowTab(TAB_ID_VEHICLE_CHARACTERISTICS);
            return 1;
        }
    }
    else if (id == 300352)
    {
        if (msg == 1)
        {
            ShowTab(TAB_ID_CB_CHARACTERISTICS);
            return 1;
        }
    }
    else if (id == 300353 && msg == 1)
    {
        ShowTab(TAB_ID_WEAPON_GROUPS);
        return 1;
    }
    return 0;
}

void PlayerVehicleWnd::ShowTab(VehicleWnd::TabId tabId)
{
    // RVA 0x66D80
    VehicleWnd::TabId const prevTab = m_curTab;
    VehicleWnd::ShowTab(tabId);

    for (int i = TAB_ID_VEHICLE_CHARACTERISTICS; i < TAB_ID_NUM_TAB_IDS; ++i)
    {
        SelectTabButton(static_cast<VehicleWnd::TabId>(i), static_cast<VehicleWnd::TabId>(i) == m_curTab);
    }

    if (prevTab != m_curTab)
    {
        M3D_APP->ImmediateMessage(65690, m_curTab, GetUniqueId(), 0, 0, {}, {});
    }
}

void PlayerVehicleWnd::OnTabBtnVehicleCharacteristicsClick()
{
    // RVA 0x66E60
    ShowTab(TAB_ID_VEHICLE_CHARACTERISTICS);
}

void PlayerVehicleWnd::OnTabBtnCBCharacteristicsClick()
{
    // RVA 0x66E70
    ShowTab(TAB_ID_CB_CHARACTERISTICS);
}

void PlayerVehicleWnd::OnTabBtnWeaponGroupsClick()
{
    // RVA 0x66E80
    ShowTab(TAB_ID_WEAPON_GROUPS);
}

void PlayerVehicleWnd::UpdateTabButtonsState()
{
    // RVA 0x66E90
    for (int i = TAB_ID_VEHICLE_CHARACTERISTICS; i < TAB_ID_NUM_TAB_IDS; ++i)
    {
        SelectTabButton(static_cast<VehicleWnd::TabId>(i), static_cast<VehicleWnd::TabId>(i) == m_curTab);
    }
}

void PlayerVehicleWnd::SelectTabButton(VehicleWnd::TabId tabId, bool bSelect)
{
    // RVA 0x66EB0
    if ((m_gameDataFlags & 1) != 0 && tabId != TAB_ID_NUM_TAB_IDS)
    {
        m3d::rend::TexHandle const tex =
            M3D_APP->m_pInterfaceManager->GetIcoByName(m_paif.m_tabButtonNames[tabId], bSelect ? 1 : 0);
        m_tabButtons[tabId]->SetImaged(tex, {}, {}, {});
    }
}

// ============================================================================
//  Hacked vehicle
// ============================================================================

ai::Vehicle* PlayerVehicleWnd::GetHackedVehicle() const
{
    // RVA 0x66C70
    if (m_hackedVehicleId == -1)
    {
        return nullptr;
    }
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_hackedVehicleId), ai::Vehicle);
}

int PlayerVehicleWnd::CreateHackedVehicle()
{
    // RVA 0x66BE0
    if (m_hackedVehicleId != -1)
    {
        help::DestroyVehicle(m_hackedVehicleId);
        m_hackedVehicleId = -1;
    }

    if (ai::thePlayer)
    {
        if (ai::Vehicle* vehicle = ai::thePlayer->GetVehicle())
        {
            ai::Obj* clone = vehicle->CloneObj();
            if (clone && clone->IsKindOf(&ai::Vehicle::m_classVehicle))
            {
                clone->SetInvisible();
                m_hackedVehicleId = clone->GetId();
            }
        }
    }
    return m_hackedVehicleId != -1;
}

void PlayerVehicleWnd::DestroyHackedVehicle()
{
    // RVA 0x66C50
    if (m_hackedVehicleId != -1)
    {
        help::DestroyVehicle(m_hackedVehicleId);
        m_hackedVehicleId = -1;
    }
}

int PlayerVehicleWnd::SetupForHackedVehiclePart(int vpId)
{
    // RVA 0x66680
    if (vpId == -1)
    {
        // Reset back to the player's real vehicle.
        if (m_hackedVehicleId != -1)
        {
            help::DestroyVehicle(m_hackedVehicleId);
            m_hackedVehicleId = -1;
        }
        ai::Vehicle* vehicle = ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr;
        SetVehicleId(vehicle ? vehicle->GetId() : -1);
        SetVehicleType(VEHICLE_NORMAL);
        return 1;
    }

    if (PrepareHackedVehicleForVehiclePart(vpId))
    {
        SetVehicleId(m_hackedVehicleId);
        SetVehicleType(VEHICLE_HACKED);
        return 1;
    }
    return 0;
}

int PlayerVehicleWnd::PrepareHackedVehicleForVehiclePart(int hackedVpId)
{
    // RVA 0x66710 - clones the player's vehicle, strips its gadgets, repository
    // and guns, then swaps in a clone of the hovered vehicle part so the
    // characteristics tab can preview it.
    if (hackedVpId < 0)
    {
        return 0;
    }

    ai::Obj* vp = ai::theObjects->GetEntityByObjId(hackedVpId);
    if (!vp || !vp->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
    {
        return 0;
    }

    ai::PrototypeInfo const* proto = vp->GetPrototypeInfo();
    if (!proto)
    {
        return 0;
    }

    // Cabin and basket parts are previewed by their own dedicated windows.
    int const cabinResId = ai::theResourceManager->GetResourceId("CABIN");
    int const basketResId = ai::theResourceManager->GetResourceId("BASKET");
    if (ai::theResourceManager->bResourceIsKindOf(proto->m_resourceId, cabinResId)
        || ai::theResourceManager->bResourceIsKindOf(proto->m_resourceId, basketResId))
    {
        return 0;
    }

    CStr const partName = help::GetVehiclePartNameByResourceId(proto->m_resourceId);
    if (partName.empty())
    {
        return 0;
    }

    if (!CreateHackedVehicle())
    {
        return 0;
    }
    ai::Vehicle* hackedVeh = GetHackedVehicle();
    if (!hackedVeh)
    {
        return 0;
    }

    ai::Obj* vpClone = vp->CloneObj();
    if (!vpClone)
    {
        DestroyHackedVehicle();
        return 0;
    }
    if (!vpClone->IsKindOf(&ai::VehiclePart::m_classVehiclePart))
    {
        vpClone->Remove();
        DestroyHackedVehicle();
        return 0;
    }
    auto* newPart = static_cast<ai::VehiclePart*>(vpClone);
    newPart->SetSkin(static_cast<int>(hackedVeh->GetSkin()));

    // Strip gadgets.
    std::vector<ai::Gadget*> gadgets;
    for (auto const& kv : hackedVeh->GetGadgets())
    {
        gadgets.push_back(kv.second);
    }
    for (ai::Gadget* gadget : gadgets)
    {
        hackedVeh->RemoveChild(gadget);
        gadget->Remove();
    }

    // Strip the repository.
    if (ai::IzvratRepository* repo = hackedVeh->GetRepository())
    {
        unsigned int const numItems = repo->GetNumItems();
        for (unsigned int i = 0; i < numItems; ++i)
        {
            ai::GeomRepositoryItem const item = repo->GetItem(static_cast<int>(i));
            if (ai::Obj* obj = item.GetObj())
            {
                obj->Remove();
            }
        }
        repo->Clear(false);
    }

    // Detach the existing guns, remembering them so compatible ones can be
    // re-attached to the previewed configuration afterwards.
    std::vector<std::pair<CStr, ai::VehiclePart*>> guns;
    retruxx::vector<CStr> const attachedParts = hackedVeh->GetAttachedPartNames();
    int const gunResId = ai::theResourceManager->GetResourceId("GUN");
    for (CStr const& pn : attachedParts)
    {
        ai::VehiclePart* part = hackedVeh->GetPartByName(pn);
        if (!part)
        {
            continue;
        }
        ai::PrototypeInfo const* partProto = part->GetPrototypeInfo();
        if (partProto && ai::theResourceManager->bResourceIsKindOf(partProto->m_resourceId, gunResId))
        {
            guns.emplace_back(pn, part);
            hackedVeh->SetPartByName(pn, nullptr, false);
        }
    }

    // Swap in the previewed part.
    if (ai::VehiclePart* existing = hackedVeh->GetPartByName(partName))
    {
        hackedVeh->SetPartByName(partName, nullptr, false);
        existing->Remove();
    }
    hackedVeh->SetPartByName(partName, newPart, false);

    // Re-attach the guns that still fit.
    for (auto const& gun : guns)
    {
        if (hackedVeh->CanPartBeAttached(gun.first))
        {
            hackedVeh->SetPartByName(gun.first, gun.second, false);
        }
        else
        {
            gun.second->Remove();
        }
    }
    return 1;
}
