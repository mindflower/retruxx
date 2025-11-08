#include "navpointbutton.h"

RT_CLASS_EXPORTS_BEGIN(NavPointButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NavPointButton);

int NavPointButton::SetupForQuest(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NavPointButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointButton::~NavPointButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* NavPointButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NavPointButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

int NavPointButton::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointButton::GetNavPointId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPoint::NavPointType NavPointButton::GetNavPointType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr NavPointButton::NavPointType2Str(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* NavPointButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointButton::NavPointButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPointButton::NavPointButton(NavPointButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NavPointButton::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPoint const* NavPointButton::GetNavPoint() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NavPointButton::UpdateNpTexture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

NavPoint::NavPointType NavPointButton::GetNavPointTypeByQuest(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int NavPointButton::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NavPointButton::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void NavPointButton::UpdateTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}
