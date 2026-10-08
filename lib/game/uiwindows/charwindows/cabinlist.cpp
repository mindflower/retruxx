#include "cabinlist.h"

#include <core/kernel.h>
#include <core/log.h>
#include <server/objects/base/objcontainer.h>
#include <server/objects/cabin.h>
#include <server/resourcemanager.h>
#include <server/server.h>

RT_CLASS_EXPORTS_BEGIN(CabinButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinButton);

RT_CLASS_EXPORTS_BEGIN(CabinList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CabinList);

CabinButton::CabinAuxInfo CabinButton::m_caif;

// ===========================================================================
//  CabinButton
// ===========================================================================

CabinButton::CabinAuxInfo::CabinAuxInfo()
{
    // RVA 0x438100
    m_wndSpeedName = "wndSpeedVal";
    m_wndTorqueName = "wndTorqueVal";
    m_wndControlName = "wndControlVal";
}

CabinButton::CabinAuxInfo::CabinAuxInfo(CabinButton::CabinAuxInfo const& rhs) :
    m_wndSpeedName(rhs.m_wndSpeedName),
    m_wndTorqueName(rhs.m_wndTorqueName),
    m_wndControlName(rhs.m_wndControlName)
{
}

CabinButton::CabinButton() : m_wndSpeed(nullptr), m_wndTorque(nullptr), m_wndControl(nullptr)
{
    // RVA 0x4381E0
    m_type = TYPE_CABIN;
}

CabinButton::CabinButton(CabinButton const&) : CabinButton()
{
    // NOTE: the shipped copy ctor (RVA 0x438260) runs only the CBButton default
    // ctor and swaps the vtable, so it leaves m_type at NUM_TYPES and the three
    // value windows uninitialised. Delegating gives a usable object instead
    // while still copying nothing.
}

CabinButton::~CabinButton() = default;

m3d::Object* CabinButton::Clone()
{
    // RVA 0x437FC0
    return new CabinButton(*this);
}

m3d::Object* CabinButton::CreateObject()
{
    return new CabinButton;
}

m3d::Class* CabinButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBButton);
}

m3d::Class* CabinButton::GetClass() const
{
    return RT_CLASS_LOCAL(CabinButton);
}

int CabinButton::CreateChildren()
{
    // RVA 0x4382B0 - the cabin row adds speed / torque / control on top of the
    // shared columns.
    if (!CBButton::CreateChildren())
    {
        return 0;
    }

    int res = 1;
    auto const find = [&](CStr const& name, m3d::ui::Wnd*& out)
    {
        m3d::Object* child = GetChildByName(name);
        if (child && child->IsKindOf(&m3d::ui::Wnd::m_classWnd))
        {
            out = static_cast<m3d::ui::Wnd*>(child);
            return;
        }
        M3D_LOG_INFO("Get control error: control " + name + " is not found or incorrect type");
        res = 0;
    };

    find(m_caif.m_wndSpeedName, m_wndSpeed);
    find(m_caif.m_wndTorqueName, m_wndTorque);
    find(m_caif.m_wndControlName, m_wndControl);
    return res;
}

int CabinButton::FullUpdate()
{
    // RVA 0x438770
    if (!CBButton::FullUpdate())
    {
        return 0;
    }

    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_cbId);
    if (!obj || !obj->IsKindOf(&ai::Cabin::m_classCabin))
    {
        return 0;
    }
    auto const* cabin = static_cast<ai::Cabin const*>(obj);

    // Max speed is stored in m/s and shown in km/h.
    m_wndSpeed->SetText(CStr(static_cast<int>(cabin->GetMaxSpeed() * 3.5999999f)));
    m_wndTorque->SetText(CStr(static_cast<int>(cabin->GetMaxTorque())));

    float control = cabin->GetControl();
    if (control < 0.0f)
    {
        control = 0.0f;
    }
    else if (control > 100.0f)
    {
        control = 100.0f;
    }
    m_wndControl->SetText(CStr(static_cast<int>(control)));
    return 1;
}

// ===========================================================================
//  CabinList
// ===========================================================================

CabinList::CabinList()
{
    // RVA 0x438950
    m_type = CBButton::TYPE_CABIN;
}

CabinList::CabinList(CabinList const&) : CabinList()
{
    // NOTE: the shipped copy ctor (RVA 0x4389A0) runs only the CBList default
    // ctor and swaps the vtable, leaving m_type at NUM_TYPES; delegating keeps
    // the list usable while still copying nothing.
}

CabinList::~CabinList() = default;

m3d::Object* CabinList::Clone()
{
    // RVA 0x437FF0
    return new CabinList(*this);
}

m3d::Object* CabinList::CreateObject()
{
    return new CabinList;
}

m3d::Class* CabinList::GetBaseClass()
{
    return RT_CLASS_LOCAL(CBList);
}

m3d::Class* CabinList::GetClass() const
{
    return RT_CLASS_LOCAL(CabinList);
}

int CabinList::GetCBResourceId() const
{
    // RVA 0x4389D0
    return ai::theResourceManager->GetResourceId(CStr("CABIN"));
}

CBButton* CabinList::CreateItem() const
{
    // RVA 0x438A20
    return static_cast<CBButton*>(M3D_KERNEL->New("CabinButton"));
}

void CabinList::PostTriggerEventOnBuyCb()
{
    // RVA 0x438A40
    ai::pServer->PostPlayerEvent(ai::GE_TUTORIAL_CABIN_SELECT);
}
