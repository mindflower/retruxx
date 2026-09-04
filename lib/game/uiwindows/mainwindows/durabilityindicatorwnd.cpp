#include "durabilityindicatorwnd.h"

#include <i_event.h>

#include "core/kernel.h"
#include "core/log.h"
#include "game/m3dgame.h"
#include "game/uimanager/truxxuimanager.h"

#include "server/objects/base/complexphysicobj.h"
#include "server/objects/base/objcontainer.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(DurabilityIndicatorWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DurabilityIndicatorWnd);

DurabilityIndicatorWnd::AuxInfo::AuxInfo()
{
    // RVA 0x11D000
    static char const* const suffixes[TYPE_NUM_TYPES] = {"Main", "Char", "Fignya"};
    static char const* const states[4] = {"Green", "Yellow", "Red", "Dead"};
    for (int type = 0; type < TYPE_NUM_TYPES; ++type)
    {
        for (int state = 0; state < 4; ++state)
        {
            m_cabinDurTexNames[type][state] = CStr("CabinDur") + states[state] + suffixes[type];
            m_basketDurTexNames[type][state] = CStr("BasketDur") + states[state] + suffixes[type];
        }
    }
    m_strCabinDurabilityId = "CabinDurability";
    m_strBasketDurabilityId = "BasketDurability";
}

m3d::Class* DurabilityIndicatorWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* DurabilityIndicatorWnd::CreateObject()
{
    // RVA 0x11CF00
    return new DurabilityIndicatorWnd;
}

int DurabilityIndicatorWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x11D7D0
    // NOTE: unlike the other indicator windows, the shipped code does not
    // reject a pattern of the wrong class here - it only checks for null.
    if (!patternWnd)
    {
        return 0;
    }

    auto* parent = static_cast<m3d::ui::Wnd*>(patternWnd->GetParent());
    if (!parent)
    {
        return 0;
    }

    Create({}, patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        patternWnd->DecRef();
    }

    m_strCabinDurability = M3D_APP->GetStringByStringId0(m_aif.m_strCabinDurabilityId);
    m_strBasketDurability = M3D_APP->GetStringByStringId0(m_aif.m_strBasketDurabilityId);
    return 1;
}

m3d::Object* DurabilityIndicatorWnd::Clone()
{
    // RVA 0x11CEB0
    return new DurabilityIndicatorWnd(*this);
}

void DurabilityIndicatorWnd::SetType(Type type, CStr const& partName)
{
    // RVA 0x11E060
    m_partName = partName;
    m_type = type;
    if ((m_partName == "CABIN" || m_partName == "BASKET") && type < TYPE_NUM_TYPES)
    {
        bool const isCabin = (m_partName == "CABIN");
        CStr const& green = isCabin ? m_aif.m_cabinDurTexNames[type][0] : m_aif.m_basketDurTexNames[type][0];
        CStr const& yellow = isCabin ? m_aif.m_cabinDurTexNames[type][1] : m_aif.m_basketDurTexNames[type][1];
        CStr const& red = isCabin ? m_aif.m_cabinDurTexNames[type][2] : m_aif.m_basketDurTexNames[type][2];
        CStr const& dead = isCabin ? m_aif.m_cabinDurTexNames[type][3] : m_aif.m_basketDurTexNames[type][3];
        SetDurTextures(green, yellow, red, dead);
        UpdateValue(true);
    }
}

m3d::Class* DurabilityIndicatorWnd::GetClass() const
{
    // RVA 0x11CEF0
    return RT_CLASS_LOCAL(DurabilityIndicatorWnd);
}

void DurabilityIndicatorWnd::SetVehicleId(int vehicleId)
{
    // RVA 0x11DF50
    m_vehicleId = vehicleId;
    m_vehiclePartId = -1;
    if (ai::Vehicle* vehicle = GetVehicle())
    {
        if (ai::VehiclePart* part = vehicle->GetPartByName(m_partName))
        {
            m_vehiclePartId = part->GetId();
        }
    }
    UpdateValue(false);
}

DurabilityIndicatorWnd::~DurabilityIndicatorWnd()
{
    // RVA 0x11D710 - the CStr members and the Wnd base clean up automatically.
    for (auto& tex : m_durTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
}

m3d::rend::TexHandle DurabilityIndicatorWnd::GetImageByDurabilityRelValue(float relValue) const
{
    // RVA 0x11DFA0
    int const pct = static_cast<int>(relValue * 100.0f);
    if (pct > 0)
    {
        if (pct >= 66)
        {
            return m_durTextures[0];
        }
        if (pct >= 33)
        {
            return m_durTextures[1];
        }
        return m_durTextures[2];
    }
    return m_durTextures[3];
}

int DurabilityIndicatorWnd::UpdateOnNewFrame()
{
    // RVA 0x11D9A0
    UpdateValue(false);
    return 1;
}

DurabilityIndicatorWnd::DurabilityIndicatorWnd()
{
    // RVA 0x11D5B0
    m_vehiclePartId = -1;
    m_vehicleId = -1;
    m_type = TYPE_IN_MAIN_INTERFACE;
    m_prevCurVal = 0.0f;
    m_prevMaxVal = 0.0f;
    UpdateValue(true);
}

DurabilityIndicatorWnd::DurabilityIndicatorWnd(DurabilityIndicatorWnd const&) : DurabilityIndicatorWnd()
{
    // NOTE: the shipped copy ctor default-constructs the base and resets
    // m_partName/m_durTextures/m_strCabinDurability/m_strBasketDurability, but
    // leaves m_vehiclePartId/m_vehicleId/m_type/m_prevCurVal/m_prevMaxVal
    // uninitialized; delegating to the default ctor here avoids reading
    // uninitialized ints while still copying nothing from the source.
}

BoundsBase<float> DurabilityIndicatorWnd::GetRect() const
{
    // RVA 0x11DE50
    return GetClientBounds();
}

ai::Vehicle* DurabilityIndicatorWnd::GetVehicle() const
{
    // RVA 0x11DE90
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

void DurabilityIndicatorWnd::SetDurTextures(
    CStr const& texGreenName,
    CStr const& texYellowName,
    CStr const& texRedName,
    CStr const& texDeadName)
{
    // RVA 0x11E170
    for (auto& tex : m_durTextures)
    {
        M3D_RENDERER->ReleaseTexture(tex);
    }
    m_durTextures[0] = M3D_APP->m_pInterfaceManager->GetIcoByName(texGreenName, 0);
    m_durTextures[1] = M3D_APP->m_pInterfaceManager->GetIcoByName(texYellowName, 0);
    m_durTextures[2] = M3D_APP->m_pInterfaceManager->GetIcoByName(texRedName, 0);
    m_durTextures[3] = M3D_APP->m_pInterfaceManager->GetIcoByName(texDeadName, 0);
    for (auto& tex : m_durTextures)
    {
        M3D_RENDERER->ReferenceTexture(tex);
    }
}

int DurabilityIndicatorWnd::OnPaint(m3d::ui::DrawInfo const& di)
{
    // RVA 0x11DE70
    m3d::ui::Wnd::OnPaint(di);
    return 1;
}

int DurabilityIndicatorWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x11D940
    if (dataType == 65)
    {
        if (data)
        {
            auto const* evt = static_cast<m3d::Event const*>(data);
            if (m_vehicleId == evt->m_intEv[0] && CStr::my_strcmp(m_partName.c_str(), evt->m_strEv.c_str()) == 0)
            {
                UpdateOnVehiclePartChanged();
            }
        }
    }
    else if (dataType == 89)
    {
        UpdateOnNewFrame();
        return 1;
    }
    return 1;
}

int DurabilityIndicatorWnd::UpdateOnVehiclePartChanged()
{
    // RVA 0x11D9B0
    ai::Vehicle* vehicle = GetVehicle();
    if (!vehicle)
    {
        return 0;
    }
    ai::VehiclePart* part = vehicle->GetPartByName(m_partName);
    m_vehiclePartId = part ? part->GetId() : -1;
    UpdateValue(false);
    return 1;
}

void DurabilityIndicatorWnd::UpdateTooltip(float val, float maxVal)
{
    // RVA 0x11DAF0
    if (m_partName != "CABIN" && m_partName != "BASKET")
    {
        CStr empty;
        SetProperty(PROP_WND_TOOLTIP, &empty);
        return;
    }

    CStr const& label = (m_partName == "CABIN") ? m_strCabinDurability : m_strBasketDurability;
    CStr text = label + ": " + CStr(static_cast<int>(val)) + "/" + CStr(static_cast<int>(maxVal));
    SetProperty(PROP_WND_TOOLTIP, &text);
}

ai::VehiclePart* DurabilityIndicatorWnd::GetVehiclePart() const
{
    // RVA 0x11DEF0
    if (m_vehiclePartId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehiclePartId);
    return (obj && obj->IsKindOf(&ai::VehiclePart::m_classVehiclePart)) ? static_cast<ai::VehiclePart*>(obj) : nullptr;
}

void DurabilityIndicatorWnd::UpdateVehiclePartImage()
{
    // RVA 0x11E000
    float durRelVal = 0.0f;
    if (ai::VehiclePart* part = GetVehiclePart())
    {
        durRelVal = part->Durability().value().get() / part->Durability().maxValue().get();
    }
    SetBackground(GetImageByDurabilityRelValue(durRelVal));
}

void DurabilityIndicatorWnd::UpdateValue(bool bForce)
{
    // RVA 0x11DA00
    float curVal = 0.0f;
    float maxVal = 0.0f;
    if (ai::VehiclePart* part = GetVehiclePart())
    {
        curVal = part->Durability().value().get();
        maxVal = part->Durability().maxValue().get();
    }
    if (bForce || curVal != m_prevCurVal || maxVal != m_prevMaxVal)
    {
        UpdateVehiclePartImage();
        if (m_type == TYPE_IN_CHARACTERISTIC_WND)
        {
            UpdateTooltip(curVal, maxVal);
        }
    }
    m_prevCurVal = curVal;
    m_prevMaxVal = maxVal;
}
