#include "characteristicswnd.h"

#include "game/uimisc/guihelper.h"

#include "server/objects/base/objcontainer.h"
#include "server/objects/vehicle.h"

RT_CLASS_EXPORTS_BEGIN(CharacteristicsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CharacteristicsWnd);

CharacteristicsWnd::CAuxInfo::CAuxInfo()
{
    m_redColor = help::Color2Str(0x0FF904040);
    m_greenColor = help::Color2Str(0x0FF23961B);
}

CharacteristicsWnd::CharacteristicsWnd()
{
    m_vehicleType = VehicleWnd::VEHICLE_NORMAL;
    m_vehicleId = -1;
}

CharacteristicsWnd::~CharacteristicsWnd()
{
    // RVA 0x42120 - m_caif's CStr members and the Wnd base clean up automatically.
}

m3d::Class* CharacteristicsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Class* CharacteristicsWnd::GetRtClass() const
{
    return RT_CLASS_LOCAL(CharacteristicsWnd);
}

ai::Vehicle const* CharacteristicsWnd::GetVehicle() const
{
    // RVA 0x42210
    if (m_vehicleId == -1)
    {
        return nullptr;
    }
    ai::Obj* obj = ai::theObjects->GetEntityByObjId(m_vehicleId);
    return (obj && obj->IsKindOf(&ai::Vehicle::m_classVehicle)) ? static_cast<ai::Vehicle*>(obj) : nullptr;
}

void CharacteristicsWnd::SetVehicleType(VehicleWnd::VehicleType vehicleType)
{
    m_vehicleType = vehicleType;
}

void CharacteristicsWnd::SetupForVehicle(int vehicleId)
{
    // RVA 0x421E0
    m_vehicleId = vehicleId;
    if (!GetVehicle())
    {
        ClearCharacteristics();
        m_vehicleId = -1;
    }
}

int CharacteristicsWnd::GameDataClear(bool)
{
    // RVA 0x42190
    // NOTE: the shipped build resets m_vehicleId to -1 before calling
    // GetVehicle() here, so the early-return branch is unreachable in
    // practice (GetVehicle() always sees -1 and returns null); preserved as
    // shipped rather than simplified away.
    m_vehicleId = -1;
    if (GetVehicle())
    {
        return 1;
    }
    ClearCharacteristics();
    m_vehicleId = -1;
    return 1;
}

int CharacteristicsWnd::GameDataUpdate(void*, int dataType)
{
    // RVA 0x42280
    if ((m_gameDataFlags & 1) == 0)
    {
        return 0;
    }
    if (dataType == 89)
    {
        OnNewFrame();
    }
    return 1;
}

void CharacteristicsWnd::OnNewFrame()
{
    // RVA 0x422B0
    UpdateCharacteristics();
}

CStr CharacteristicsWnd::GetColorByVal(int val, int baseVal, bool bInverted) const
{
    // RVA 0x422C0
    if (val > baseVal)
    {
        return bInverted ? m_caif.m_redColor : m_caif.m_greenColor;
    }
    if (val < baseVal)
    {
        return bInverted ? m_caif.m_greenColor : m_caif.m_redColor;
    }
    return CStr();
}

CStr CharacteristicsWnd::GetDiffValPrefix(int diff, bool bColorInverted) const
{
    // RVA 0x42320
    if (diff > 0)
    {
        return (bColorInverted ? m_caif.m_redColor : m_caif.m_greenColor) + "+";
    }
    if (diff < 0)
    {
        return bColorInverted ? m_caif.m_greenColor : m_caif.m_redColor;
    }
    return CStr();
}

void CharacteristicsWnd::UpdateValue(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted)
{
    // RVA 0x423B0
    if (m_vehicleType != VehicleWnd::VEHICLE_NORMAL)
    {
        UpdateValueHacked(ctrl, val, baseVal, bColorInverted);
    }
    else
    {
        UpdateValueNormal(ctrl, val, baseVal, bColorInverted);
    }
}

void CharacteristicsWnd::UpdateValueNormal(m3d::ui::Wnd* ctrl, int val, int baseVal, bool)
{
    // RVA 0x423E0
    // NOTE: the shipped build never reads bColorInverted here - it always
    // uses the non-inverted color rule, regardless of what the caller passes.
    if (!ctrl)
    {
        return;
    }
    ctrl->SetText(GetColorByVal(val, baseVal, false) + CStr(val));
}

void CharacteristicsWnd::UpdateValueHacked(m3d::ui::Wnd* ctrl, int val, int baseVal, bool bColorInverted)
{
    // RVA 0x424B0
    if (!ctrl)
    {
        return;
    }

    int const diff = val - baseVal;
    CStr suffix;
    if (diff != 0)
    {
        // The diff number is drawn in its own color, then the control's own
        // text color is appended right after it so the closing ")" isn't tinted.
        CStr const ownColor = help::Color2Str(ctrl->GetTextColor());
        suffix = CStr("(") + GetDiffValPrefix(diff, bColorInverted) + CStr(diff) + ownColor + ")";
    }
    ctrl->SetText(CStr(val) + suffix);
}
