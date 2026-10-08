#include "cabincharacteristicswnd.h"

#include <algorithm>

#include "core/log.h"
#include "game/uimisc/guihelper.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/cabin.h"
#include "server/objects/player.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(CabinCharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinCharacteristicsWnd);

CabinCharacteristicsWnd::CabinAuxInfo::CabinAuxInfo()
{
    m_wndMaxSpeedValName = "wndMaxSpeedVal";
    m_wndTorqueValName = "wndTorqueVal";
    m_wndControlValName = "wndControlVal";
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd()
{
    m_wndMaxSpeedVal = 0;
    m_wndTorqueVal = 0;
    m_wndControlVal = 0;
}

CabinCharacteristicsWnd::CabinCharacteristicsWnd(CabinCharacteristicsWnd const&) : CabinCharacteristicsWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and AuxInfo but
    // leaves every Wnd* member uninitialised; delegating to the default ctor
    // avoids reading uninitialised pointers.
}

CabinCharacteristicsWnd::~CabinCharacteristicsWnd()
{
    // m_cabinAif's CStr members and the CBCharacteristicsWnd base clean up
    // automatically.
}

m3d::Object* CabinCharacteristicsWnd::Clone()
{
    return new CabinCharacteristicsWnd(*this);
}

m3d::Object* CabinCharacteristicsWnd::CreateObject()
{
    return new CabinCharacteristicsWnd;
}

m3d::Class* CabinCharacteristicsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(CabinCharacteristicsWnd);
}

m3d::Class* CabinCharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBCharacteristicsWnd);
}

int CabinCharacteristicsWnd::GameDataSetup()
{
    if (!CBCharacteristicsWnd::GameDataSetup())
    {
        return 0;
    }

    bool ok = true;
    if ((m_gameDataFlags & 2) == 0)
    {
        auto find = [this, &ok](CStr const& name, m3d::ui::Wnd*& out)
        {
            auto* child = GetChildByName(name);
            if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
            {
                out = static_cast<m3d::ui::Wnd*>(child);
            }
            else
            {
                M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
                ok = false;
            }
        };

        find(m_cabinAif.m_wndMaxSpeedValName, m_wndMaxSpeedVal);
        find(m_cabinAif.m_wndTorqueValName, m_wndTorqueVal);
        find(m_cabinAif.m_wndControlValName, m_wndControlVal);

        if (!ok)
        {
            m_gameDataFlags &= ~1u;
        }
    }

    if ((m_gameDataFlags & 1) != 0)
    {
        return 1;
    }
    M3D_LOG_INFO("CabinCharacteristicsWnd: error - fail to init because of a bad resource");
    return 0;
}

void CabinCharacteristicsWnd::SetupForCB(int cbId)
{
    CBCharacteristicsWnd::SetupForCB(cbId);
}

void CabinCharacteristicsWnd::SetupForCabin(int cabinId)
{
    if (cabinId != -1)
    {
        ai::Obj* obj = ai::theObjects->GetEntityByObjId(cabinId);
        if (!obj || !obj->IsKindOf(&ai::Cabin::m_classCabin))
        {
            cabinId = -1;
        }
    }
    SetupForCB(cabinId);
}

ai::Cabin const* CabinCharacteristicsWnd::GetCabin() const
{
    if (m_cbId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    return (obj && obj->IsKindOf(&ai::Cabin::m_classCabin)) ? static_cast<ai::Cabin*>(obj) : nullptr;
}

void CabinCharacteristicsWnd::ClearCharacteristics()
{
    if ((m_gameDataFlags & 1) != 0)
    {
        CBCharacteristicsWnd::ClearCharacteristics();
        m_wndMaxSpeedVal->SetText(CStr());
        m_wndTorqueVal->SetText(CStr());
        m_wndControlVal->SetText(CStr());
    }
}

void CabinCharacteristicsWnd::UpdateCharacteristics()
{
    if ((m_gameDataFlags & 1) == 0)
    {
        return;
    }
    CBCharacteristicsWnd::UpdateCharacteristics();

    ai::Cabin const* cabin = GetCabin();
    if (!cabin)
    {
        return;
    }
    ai::Vehicle const* vehicle = GetVehicle();
    if (!vehicle)
    {
        return;
    }

    ai::Vehicle* baseVehicle = m_vehicleType != VehicleWnd::VEHICLE_NORMAL
                                   ? (ai::thePlayer ? ai::thePlayer->GetVehicle() : nullptr)
                                   : const_cast<ai::Vehicle*>(vehicle);
    if (!baseVehicle)
    {
        return;
    }

    ai::VehiclePart* basePart = baseVehicle->GetPartByName(cabin->GetPartName());
    if (!basePart || !basePart->IsKindOf(&ai::Cabin::m_classCabin))
    {
        return;
    }

    int const maxSpeed = static_cast<int>(cabin->GetMaxSpeed() * 3.6f);
    int const torque = static_cast<int>(cabin->GetMaxTorque());
    int const control = std::clamp(static_cast<int>(cabin->GetControl()), 0, 100);

    // Property ids as verified from the shipped binary; the calls also
    // reverse any installed gadget modification on baseVehicle.
    int const baseMaxSpeed = static_cast<int>(help::GetBasePropertyValFromObj(basePart, baseVehicle, 23) * 3.6f);
    int const baseTorque = static_cast<int>(help::GetBasePropertyValFromObj(basePart, baseVehicle, 22));
    int const baseControl =
        std::clamp(static_cast<int>(help::GetBasePropertyValFromObj(basePart, baseVehicle, 25)), 0, 100);

    UpdateValue(m_wndMaxSpeedVal, maxSpeed, baseMaxSpeed, false);
    UpdateValue(m_wndTorqueVal, torque, baseTorque, false);
    UpdateValue(m_wndControlVal, control, baseControl, false);
}
