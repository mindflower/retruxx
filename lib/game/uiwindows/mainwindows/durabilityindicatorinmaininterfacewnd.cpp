#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"
#include <core/log.h>
#include <game/m3dgame.h>
#include <i_event.h>
#include "server/objects/base/complexphysicobj.h"
#include "server/objects/physicbodies/vehiclepart.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(DurabilityIndicatorInMainInterfaceWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(DurabilityIndicatorInMainInterfaceWnd);

m3d::Class* DurabilityIndicatorInMainInterfaceWnd::GetClass() const
{
    return RT_CLASS_LOCAL(DurabilityIndicatorInMainInterfaceWnd);
}

m3d::Object* DurabilityIndicatorInMainInterfaceWnd::CreateObject()
{
    return new DurabilityIndicatorInMainInterfaceWnd;
}

int DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc)
{
    // RVA 0x11C2E0
    using namespace m3d::ui;

    if (!patternWnd || !patternWnd->IsKindOf(&ImageWnd::m_classImageWnd))
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd: error to create - invalid pattern wnd");
        return 0;
    }
    auto* parent = patternWnd->GetParent();
    if (!parent || !IS_KIND_OF(parent, Wnd))
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - null parent for paternWnd");
        return 0;
    }

    auto res = Wnd::Create(patternWnd->GetText(), patternWnd->GetStyle(), patternWnd->GetBounds(), patternWnd->GetId());
    if (res == 0)
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - cannot create window");
        return 0;
    }

    SetStyle(patternWnd->GetStyle());
    SetText(patternWnd->GetText());
    SetId(patternWnd->GetId());
    SetName(patternWnd->GetName());
    SetBounds(patternWnd->GetBounds(), true);
    SetDefaultFont(patternWnd->GetDefaultFont());
    SetWrapMode(patternWnd->GetWrapMode());

    SetFormatMode(patternWnd->GetFormatMode());
    SetColor(patternWnd->GetColor());
    SetTextColor(patternWnd->GetColor());
    SetTextColorDisabled(patternWnd->GetTextColorDisabled());
    SetClientEdges(patternWnd->GetClientEdges());
    SetPane(patternWnd->GetPaneName());
    SetPaneFlags(patternWnd->GetPaneFlags());
    SetScrollPane(patternWnd->GetScrollPaneName());
    SetBackground(patternWnd->GetBackground());

    CStr tooltip;
    patternWnd->GetProperty(PROP_WND_TOOLTIP, &tooltip);
    SetProperty(PROP_WND_TOOLTIP, &tooltip);

    SetOnShowAnimation(patternWnd->GetOnShowAnimation());
    SetOnHideAnimation(patternWnd->GetOnHideAnimation());

    auto* imageWnd = (ImageWnd*)patternWnd;
    SetImage(imageWnd->GetImage());

    parent->AddChild(this);
    if (deleteSrc)
    {
        parent->RemoveChild(patternWnd);
        // TODO: check this obj delete
        patternWnd->DecRef();
    }

    m_strCabinDurability =
        M3D_APP->GetStringByStringId0(DurabilityIndicatorInMainInterfaceWnd::m_aif.m_strCabinDurabilityId);
    m_strBasketDurability =
        M3D_APP->GetStringByStringId0(DurabilityIndicatorInMainInterfaceWnd::m_aif.m_strBasketDurabilityId);

    m_gameDataFlags |= 1u;
    return 1;
}

void DurabilityIndicatorInMainInterfaceWnd::SetType(Type type, CStr const& partName)
{
    // RVA 0x11CE30
    m_partName = partName;
    m_type = type;

    char const* namePtr = m_partName.c_str();
    if ((CStr::my_strcmp(namePtr, "CABIN") == 0 || CStr::my_strcmp(namePtr, "BASKET") == 0) && type < TYPE_NUM_TYPES)
    {
        FullUpdate(true);
    }
}

m3d::Object* DurabilityIndicatorInMainInterfaceWnd::Clone()
{
    // RVA 0x11BFE0
    return new DurabilityIndicatorInMainInterfaceWnd(*this);
}

void DurabilityIndicatorInMainInterfaceWnd::SetVehicleId(int vehicleId)
{
    m_vehiclePartId = -1;
    m_vehicleId = vehicleId;

    if (auto const* vehicle = GetVehicle())
    {
        if (auto const* partByName = vehicle->GetPartByName(m_partName))
        {
            m_vehiclePartId = partByName->GetId();
        }
    }

    float curValue = 0.0;
    float maxValue = 0.0;
    if (auto const* vehiclePart = GetVehiclePart())
    {
        curValue = vehiclePart->Durability().value().get();
        maxValue = vehiclePart->Durability().maxValue().get();
    }

    UpdateColor(curValue, maxValue);
    if (m_type == TYPE_IN_CHARACTERISTIC_WND)
    {
        UpdateTooltip(curValue, maxValue);
    }

    m_prevCurVal = curValue;
    m_prevMaxVal = maxValue;
}

DurabilityIndicatorInMainInterfaceWnd::~DurabilityIndicatorInMainInterfaceWnd()
{
    // RVA 0x11C240 - m_partName/m_strCabinDurability/m_strBasketDurability
    // and the ImageWnd base clean up automatically.
}

m3d::Class* DurabilityIndicatorInMainInterfaceWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ImageWnd);
}

unsigned DurabilityIndicatorInMainInterfaceWnd::GetColorByValue(float curVal, float maxVal) const
{
    if (maxVal == 0.0)
        return 0xFF666666;

    float const val = curVal / maxVal;
    if (val < 0.001)
        return 0xFF666666;
    if (val <= 0.1)
        return 0xFFFF3800;
    if (val <= 0.2)
        return 0xFFFF5200;
    if (val <= 0.30000001)
        return 0xFFFF6E00;
    if (val <= 0.40000001)
        return 0xFFFF8F00;
    if (val <= 0.5)
        return 0xFFFFA100;
    if (val <= 0.60000002)
        return 0xFFF2B200;
    if (val <= 0.69999999)
        return 0xFFDDC000;
    if (val <= 0.80000001)
        return 0xFFC3CC00;
    if (val <= 0.89999998)
        return 0xFFA9D700;
    if (val > 1.0)
        return -1;

    return 0xFF87E400;
}

int DurabilityIndicatorInMainInterfaceWnd::GameDataUpdate(void* data, int dataType)
{
    // RVA 0x11C6B0
    if (dataType == 65)
    {
        OnVehiclePartChanged(data);
    }
    else if (dataType == 89 && m_vehicleId != -1)
    {
        FullUpdate(false);
        return 1;
    }
    return 1;
}

int DurabilityIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    // RVA 0x11C690
    SetVehicleId(-1);
    return 1;
}

ai::Vehicle const* DurabilityIndicatorInMainInterfaceWnd::GetVehicle() const
{
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehicleId), ai::Vehicle const);
}

DurabilityIndicatorInMainInterfaceWnd::DurabilityIndicatorInMainInterfaceWnd()
{
    m_vehicleId = -1;
    m_vehiclePartId = -1;
    m_type = TYPE_NUM_TYPES;
    m_prevCurVal = 0.0;
    m_prevMaxVal = 0.0;
}

DurabilityIndicatorInMainInterfaceWnd::DurabilityIndicatorInMainInterfaceWnd(
    DurabilityIndicatorInMainInterfaceWnd const&) :
    DurabilityIndicatorInMainInterfaceWnd()
{
    // NOTE: the shipped copy ctor (RVA 0x11C1F0) default-constructs the base
    // and resets m_partName/m_strCabinDurability/m_strBasketDurability to
    // empty, but leaves m_vehiclePartId/m_vehicleId/m_type/m_prevCurVal/
    // m_prevMaxVal uninitialized; delegating to the default ctor here avoids
    // reading uninitialized ints while still copying nothing from the source.
}

void DurabilityIndicatorInMainInterfaceWnd::UpdateTooltip(float val, float maxVal)
{
    // RVA 0x11C840
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

void DurabilityIndicatorInMainInterfaceWnd::OnVehiclePartChanged(void* data)
{
    // RVA 0x11C710
    if (!data)
    {
        return;
    }
    auto const* evt = static_cast<m3d::Event const*>(data);
    if (m_vehicleId == evt->m_intEv[0] && CStr::my_strcmp(m_partName.c_str(), evt->m_strEv.c_str()) == 0)
    {
        if (ai::Vehicle const* vehicle = GetVehicle())
        {
            ai::VehiclePart const* part = vehicle->GetPartByName(m_partName);
            m_vehiclePartId = part ? part->GetId() : -1;
            FullUpdate(true);
        }
    }
}

void DurabilityIndicatorInMainInterfaceWnd::GetValue(float& curVal, float& maxVal) const
{
    // RVA 0x11CE80
    curVal = 0.0f;
    maxVal = 0.0f;
    if (ai::VehiclePart const* part = GetVehiclePart())
    {
        curVal = part->Durability().value().get();
        maxVal = part->Durability().maxValue().get();
    }
}

void DurabilityIndicatorInMainInterfaceWnd::FullUpdate(bool bForce)
{
    float curVal = 0.0;
    float maxVal = 0.0;

    if (auto const* vehiclePart = GetVehiclePart())
    {
        curVal = vehiclePart->Durability().value().get();
        maxVal = vehiclePart->Durability().maxValue().get();
    }

    if (bForce || curVal != m_prevCurVal || maxVal != m_prevMaxVal)
    {
        UpdateColor(curVal, maxVal);
        if (m_type == TYPE_IN_CHARACTERISTIC_WND)
        {
            UpdateTooltip(curVal, maxVal);
        }
    }

    m_prevCurVal = curVal;
    m_prevMaxVal = maxVal;
}

void DurabilityIndicatorInMainInterfaceWnd::OnNewFrame()
{
    // RVA 0x11C6F0
    if (m_vehicleId != -1)
    {
        FullUpdate(false);
    }
}

void DurabilityIndicatorInMainInterfaceWnd::UpdateColor(float curVal, float maxVal)
{
    unsigned const colorByValue = GetColorByValue(curVal, maxVal);
    SetColor(colorByValue);
}

ai::VehiclePart const* DurabilityIndicatorInMainInterfaceWnd::GetVehiclePart() const
{
    return RT_DYNCAST(ai::theObjects->GetEntityByObjId(m_vehiclePartId), ai::VehiclePart const);
}
