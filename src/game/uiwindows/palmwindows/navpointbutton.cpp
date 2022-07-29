#include "navpointbutton.h"

RT_CLASS_EXPORTS_BEGIN(NavPointButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(NavPointButton);

int NavPointButton::SetupForQuest(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* NavPointButton::Clone()
{
    throw std::logic_error("Not implemented");
}

NavPointButton::~NavPointButton()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* NavPointButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* NavPointButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int NavPointButton::Create(CStr const&, unsigned, BoundsBase<float> const&, unsigned)
{
    throw std::logic_error("Not implemented");
}

int NavPointButton::GetNavPointId() const
{
    throw std::logic_error("Not implemented");
}

NavPoint::NavPointType NavPointButton::GetNavPointType() const
{
    throw std::logic_error("Not implemented");
}

CStr NavPointButton::NavPointType2Str(NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* NavPointButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

NavPointButton::NavPointButton()
{
    throw std::logic_error("Not implemented");
}

NavPointButton::NavPointButton(NavPointButton const&)
{
    throw std::logic_error("Not implemented");
}

void NavPointButton::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

NavPoint const* NavPointButton::GetNavPoint() const
{
    throw std::logic_error("Not implemented");
}

void NavPointButton::UpdateNpTexture()
{
    throw std::logic_error("Not implemented");
}

NavPoint::NavPointType NavPointButton::GetNavPointTypeByQuest(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

int NavPointButton::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

void NavPointButton::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    throw std::logic_error("Not implemented");
}

void NavPointButton::UpdateTooltip()
{
    throw std::logic_error("Not implemented");
}
