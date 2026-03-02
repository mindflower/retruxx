#include "durabilityindicatorinmaininterfacewnd.h"
#include "healthindicatorinmaininterfacewnd.h"
#include <core/log.h>
#include <game/m3dgame.h>
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
    // TODO: check this!!!
    using namespace m3d::ui;

    if (!patternWnd)
    {
        M3D_LOG_INFO("DurabilityIndicatorInMainInterfaceWnd::CreateFromPattern error - null patternWnd");
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
    // TODO: generated code DurabilityIndicatorInMainInterfaceWnd::SetType
    m_partName = partName;
    m_type = type;

    char const* namePtr = m_partName.c_str();  // Assuming c_str() method exists
    if ((CStr::my_strcmp(namePtr, "CABIN") == 0 || CStr::my_strcmp(namePtr, "BASKET") == 0) && type < TYPE_NUM_TYPES)
    {
        FullUpdate(true);  // Assuming the '1' parameter is a bool for some update flag
    }
}

m3d::Object* DurabilityIndicatorInMainInterfaceWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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

int DurabilityIndicatorInMainInterfaceWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

int DurabilityIndicatorInMainInterfaceWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
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
    DurabilityIndicatorInMainInterfaceWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::UpdateTooltip(float, float)
{
    // TODO: implement DurabilityIndicatorInMainInterfaceWnd::UpdateTooltip
}

void DurabilityIndicatorInMainInterfaceWnd::OnVehiclePartChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void DurabilityIndicatorInMainInterfaceWnd::GetValue(float&, float&) const
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
