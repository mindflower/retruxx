#include "vehiclewnd.h"

#include "basketcharacteristicswnd.h"
#include "cabincharacteristicswnd.h"
#include "vehiclecharacteristicswnd.h"
#include "weaponslotlist.h"
#include "game/uimisc/guihelper.h"
#include "game/uimanager/uidefs.h"

#include <game/uiwindows/commonwindows/complexmodelwnd.h>
#include <game/m3dgame.h>
#include <core/kernel.h>
#include <core/log.h>
#include <i_event.h>
#include <ui/sgnodearraywnd.h>

#include <server/objects/vehicle.h>
#include <server/objects/cabin.h>
#include <server/objects/basket.h>
#include <server/objects/base/prototypeinfo.h>
#include <server/objects/base/prototypemanager.h>

RT_CLASS_EXPORTS_BEGIN(VehicleWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(VehicleWnd);

VehicleWnd::AuxInfo::AuxInfo()
{
    m_wndTruckPictureName = "wndTruckPicture";
    m_lblNameName = "lblName";
    m_wndCBCharacteristicsName = "wndCBCharacteristics";
    m_redColor = help::Color2Str(0xFF9C2621);
    m_greenColor = help::Color2Str(0xFF23781B);
}

VehicleWnd::VehicleWnd()
{
    m_lblName = nullptr;
    m_vehicleType = VEHICLE_NORMAL;
    m_curTab = TAB_ID_NUM_TAB_IDS;
}

VehicleWnd::VehicleWnd(VehicleWnd const&) : VehicleWnd()
{
    // The shipped copy constructor only zero-inits the ref_ptr members (which
    // default-construct null in retruxx anyway); delegate to the default ctor.
}

VehicleWnd::~VehicleWnd() = default;

m3d::Object* VehicleWnd::Clone()
{
    return new VehicleWnd(*this);
}

m3d::Object* VehicleWnd::CreateObject()
{
    return new VehicleWnd;
}

m3d::Class* VehicleWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ChildPanel);
}

m3d::Class* VehicleWnd::GetClass() const
{
    return RT_CLASS_LOCAL(VehicleWnd);
}

// ---------------------------------------------------------------------------

int VehicleWnd::GameDataSetup()
{
    int res = 1;

    if ((m_gameDataFlags & 2) == 0)
    {
        // Name label.
        m3d::Object* child = GetChildByName(m_aif.m_lblNameName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_lblName = static_cast<m3d::ui::Wnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_lblNameName + " is not found or incorrect type");
            res = 0;
        }

        // The cabin/basket characteristics container pattern - it only serves as
        // an anchor; the real sub-panels are pulled from the interface manager.
        child = GetChildByName(m_aif.m_wndCBCharacteristicsName);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            m_wndCBCharacteristics = static_cast<m3d::ui::Wnd*>(child);
        }
        else
        {
            M3D_LOG_INFO("Get control error: control " + m_aif.m_wndCBCharacteristicsName +
                         " is not found or incorrect type");
            res = 0;
        }

        // The 3d vehicle model is built from the wndTruckPicture pattern node.
        m3d::Object* pattern = GetChildByName(m_aif.m_wndTruckPictureName);
        if (pattern && pattern->IsKindOf(&m3d::ui::SgNodeArrayWnd::m_classSgNodeArrayWnd))
        {
            m_wndVehicleModel = static_cast<ComplexModelWnd*>(M3D_KERNEL->New("ComplexModelWnd"));
            if (!m_wndVehicleModel)
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndTruckPictureName +
                             " - cannot find rtti class ComplexModelWnd");
                res = 0;
            }
            else if (!m_wndVehicleModel->CreateFromPattern(static_cast<m3d::ui::Wnd*>(pattern), true))
            {
                M3D_LOG_INFO("Make control error: cannot create " + m_aif.m_wndTruckPictureName +
                             " from pattern class");
                res = 0;
            }
        }
        else
        {
            M3D_LOG_INFO("Make control error: control " + m_aif.m_wndTruckPictureName +
                         " is not found or incorrect type");
            res = 0;
        }

        // Resolve the interface-manager ids of the four sub panels from our own
        // gui id (left / right character window, or the buy-vehicle window).
        int vehCharId = -1;
        int cabinCharId = -1;
        int basketCharId = -1;
        int weaponGroupsId = -1;
        switch (m_guiId)
        {
        case IW_WND_CHARACTERISTICS_LEFT:
            vehCharId = IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_LEFT;
            cabinCharId = IW_WND_PLAYER_CABIN_CHARACTERISTICS_LEFT;
            basketCharId = IW_WND_PLAYER_BASKET_CHARACTERISTICS_LEFT;
            weaponGroupsId = IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_LEFT;
            break;
        case IW_WND_CHARACTERISTICS_RIGHT:
            vehCharId = IW_WND_PLAYER_VEHICLE_CHARACTERISTICS_RIGHT;
            cabinCharId = IW_WND_PLAYER_CABIN_CHARACTERISTICS_RIGHT;
            basketCharId = IW_WND_PLAYER_BASKET_CHARACTERISTICS_RIGHT;
            weaponGroupsId = IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_RIGHT;
            break;
        case IW_WND_BUY_VEHICLE:
            vehCharId = IW_WND_WORKSHOP_VEHICLE_CHARACTERISTICS;
            cabinCharId = IW_WND_WORKSHOP_CABIN_CHARACTERISTICS;
            basketCharId = IW_WND_WORKSHOP_BASKET_CHARACTERISTICS;
            weaponGroupsId = IW_WND_WEAPON_INFO_LIST_IN_CHAR_WND_WORKSHOP;
            break;
        default:
            break;
        }

        {
            ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(vehCharId);
            if (w && w->IsKindOf(&VehicleCharacteristicsWnd::m_classVehicleCharacteristicsWnd))
            {
                m_wndVehicleCharacteristics = static_cast<VehicleCharacteristicsWnd*>(w.get());
            }
            else
            {
                res = 0;
            }
        }
        {
            ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(cabinCharId);
            if (w && w->IsKindOf(&CabinCharacteristicsWnd::m_classCabinCharacteristicsWnd))
            {
                m_wndCabinCharacteristics = static_cast<CabinCharacteristicsWnd*>(w.get());
            }
            else
            {
                res = 0;
            }
        }
        {
            ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(basketCharId);
            if (w && w->IsKindOf(&BasketCharacteristicsWnd::m_classBasketCharacteristicsWnd))
            {
                m_wndBasketCharacteristics = static_cast<BasketCharacteristicsWnd*>(w.get());
            }
            else
            {
                res = 0;
            }
        }
        {
            ref_ptr<m3d::ui::Wnd> w = M3D_APP->m_pInterfaceManager->GetWindow(weaponGroupsId);
            if (w && w->IsKindOf(&WeaponSlotList::m_classWeaponSlotList))
            {
                m_wndWeaponGroups = static_cast<WeaponSlotList*>(w.get());
            }
            else
            {
                res = 0;
            }
        }

        if (res)
        {
            // Move the two CB sub-panels under the container and register the
            // three tab pages.
            RemoveChild(m_wndCBCharacteristics.get());
            m_wndCBCharacteristics->AddChild(m_wndCabinCharacteristics.get());
            m_wndCBCharacteristics->AddChild(m_wndBasketCharacteristics.get());

            m_tabItems[TAB_ID_VEHICLE_CHARACTERISTICS] = m_wndVehicleCharacteristics.get();
            m_tabItems[TAB_ID_CB_CHARACTERISTICS] = m_wndCBCharacteristics.get();
            m_tabItems[TAB_ID_WEAPON_GROUPS] = m_wndWeaponGroups.get();

            m_gameDataFlags |= 1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("VehicleWnd: error - fail to init because of a bad resource");
    return 0;
}

int VehicleWnd::GameDataUpdate(void* data, int dataType)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 55)
    {
        OnCharacteristicTabSelChanged(data);
    }
    else if (dataType == 65)
    {
        OnVehiclePartChanged(data);
        return 1;
    }
    return 1;
}

void VehicleWnd::SetVehicleId(int vehicleId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    ChildPanel::SetVehicleId(vehicleId);
    const int newId = GetVehicleId();

    ai::Vehicle* vehicle = GetVehicle();
    const ai::Cabin* cabin = vehicle ? vehicle->GetCabin() : nullptr;
    const ai::Basket* basket = vehicle ? vehicle->GetBasket() : nullptr;

    m_wndVehicleModel->SetObjId(newId);

    VehicleCharacteristicsWnd* vehChar = m_wndVehicleCharacteristics.get();
    vehChar->m_vehicleId = newId;
    if (!vehChar->GetVehicle())
    {
        vehChar->ClearCharacteristics();
        if (vehChar->m_vehicleId != -1)
        {
            vehChar->m_vehicleId = -1;
        }
    }

    m_wndCabinCharacteristics->SetupForCabin(cabin ? cabin->GetId() : -1);
    m_wndBasketCharacteristics->SetupForBasket(basket ? basket->GetId() : -1);
    m_wndWeaponGroups->SetupForVehicle(newId);

    UpdateVehicleName();
}

void VehicleWnd::SetVehicleType(VehicleWnd::VehicleType type)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    m_vehicleType = type;
    m_wndVehicleCharacteristics->SetVehicleType(type);
    m_wndCabinCharacteristics->SetVehicleType(type);
    m_wndBasketCharacteristics->SetVehicleType(type);
}

void VehicleWnd::UpdateVehicleName()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    ai::Vehicle* vehicle = GetVehicle();
    const ai::VehiclePrototypeInfo* proto = vehicle ? vehicle->GetPrototypeInfo() : nullptr;
    if (proto)
    {
        m_lblName->SetText(ai::thePrototypeManager->GetPrototypeFullName(proto->m_prototypeName));
    }
    else
    {
        // The shipped build copies the global "errormsg" placeholder here; it is
        // a default-constructed (empty) CStr with no writer anywhere in the exe.
        m_lblName->SetText(CStr());
    }
}

void VehicleWnd::OnVehiclePartChanged(void* data)
{
    if ((m_gameDataFlags & 1) == 0 || !data)
    {
        return;
    }

    const auto* evt = static_cast<const m3d::Event*>(data);
    if (GetVehicleId() != evt->m_intEv[0])
    {
        return;
    }

    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    const char* changedPart = evt->m_strEv.c_str();
    if (CStr::my_strcmp(changedPart, "CABIN") == 0)
    {
        const ai::Cabin* cabin = vehicle->GetCabin();
        m_wndCabinCharacteristics->SetupForCabin(cabin ? cabin->GetId() : -1);
    }
    else if (CStr::my_strcmp(changedPart, "BASKET") == 0)
    {
        const ai::Basket* basket = vehicle->GetBasket();
        m_wndBasketCharacteristics->SetupForBasket(basket ? basket->GetId() : -1);
    }
}

void VehicleWnd::ShowTab(VehicleWnd::TabId tabId)
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }

    m_curTab = tabId;
    for (unsigned int i = 0; i < TAB_ID_NUM_TAB_IDS; ++i)
    {
        m3d::ui::Wnd* item = m_tabItems[i].get();
        if (i == static_cast<unsigned int>(tabId))
        {
            if (item && !item->GetParent())
            {
                AddChild(item);
            }
        }
        else if (item && IsDirectChild(item))
        {
            RemoveChild(item);
        }
    }
}

void VehicleWnd::OnCharacteristicTabSelChanged(void* data)
{
    if ((m_gameDataFlags & 1) == 0 || !data)
    {
        return;
    }

    const auto* evt = static_cast<const m3d::Event*>(data);
    if (evt->m_intEv[1] == GetUniqueId())
    {
        return;
    }

    unsigned int tab = static_cast<unsigned int>(evt->m_intEv[0]);
    if (tab > TAB_ID_WEAPON_GROUPS)
    {
        tab = TAB_ID_NUM_TAB_IDS;
    }
    ShowTab(static_cast<TabId>(tab));
}
