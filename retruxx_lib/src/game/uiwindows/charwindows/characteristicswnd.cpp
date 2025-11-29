#include "characteristicswnd.h"

#include "game/uimisc/guihelper.h"

RT_CLASS_EXPORTS_BEGIN(CharacteristicsWnd)
    RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CharacteristicsWnd);

CharacteristicsWnd::CAuxInfo::CAuxInfo()
{
    m_redColor = help::Color2Str(0x0FF904040);
    m_greenColor = help::Color2Str(0x0FF23961B);
}

CharacteristicsWnd::~CharacteristicsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CharacteristicsWnd::GetRtClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CharacteristicsWnd::SetVehicleType(VehicleWnd::VehicleType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr CharacteristicsWnd::GetDiffValPrefix(int, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CharacteristicsWnd::CharacteristicsWnd()
{
    m_vehicleType = VehicleWnd::VEHICLE_NORMAL;
    m_vehicleId = -1;
}

void CharacteristicsWnd::UpdateValue(m3d::ui::Wnd*, int, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CharacteristicsWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr CharacteristicsWnd::GetColorByVal(int, int, bool) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CharacteristicsWnd::GameDataUpdate(void*, int)
{
    // TODO: implement GameDataUpdate
    //  RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void CharacteristicsWnd::OnNewFrame()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CharacteristicsWnd::UpdateValueHacked(m3d::ui::Wnd*, int, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CharacteristicsWnd::UpdateValueNormal(m3d::ui::Wnd*, int, int, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

ai::Vehicle const* CharacteristicsWnd::GetVehicle() const
{
    RETRUXX_NOT_IMPLEMENTED;
}
