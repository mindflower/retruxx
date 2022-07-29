#include "checklist.h"

RT_CLASS_EXPORTS_BEGIN(CheckList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckList);

int CheckList::SelectButtonByName(CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CheckList::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CheckList::CreateObject()
{
    throw std::logic_error("Not implemented");
}

int CheckList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CheckList::Clone()
{
    throw std::logic_error("Not implemented");
}

int CheckList::AddButtonByName(CStr const&, CStr const&)
{
    throw std::logic_error("Not implemented");
}

void CheckList::SetCurSel(int)
{
    throw std::logic_error("Not implemented");
}

CheckList::~CheckList()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CheckList::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int CheckList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}

void CheckList::SelectButton(CheckButton*)
{
    throw std::logic_error("Not implemented");
}

int CheckList::MeasureItem(int, BoundsBase<float>&) const
{
    throw std::logic_error("Not implemented");
}

int CheckList::DeleteItem(int)
{
    throw std::logic_error("Not implemented");
}

CheckList::CheckList()
{
    throw std::logic_error("Not implemented");
}

CheckList::CheckList(CheckList const&)
{
    throw std::logic_error("Not implemented");
}

int CheckList::CompareItem(int, int)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(CheckButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckButton);

m3d::Class* CheckButton::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CheckButton::CreateObject()
{
    throw std::logic_error("Not implemented");
}

BoundsBase<float> const& CheckButton::GetVirtualBounds() const
{
    throw std::logic_error("Not implemented");
}

int CheckButton::SetUp(CStr const&, PointBase<float> const&, float, CStr const&)
{
    throw std::logic_error("Not implemented");
}

m3d::Object* CheckButton::Clone()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* CheckButton::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

void CheckButton::Select(bool)
{
    throw std::logic_error("Not implemented");
}

CStr const& CheckButton::GetName() const
{
    throw std::logic_error("Not implemented");
}

CStr const& CheckButton::GetFullName() const
{
    throw std::logic_error("Not implemented");
}

void CheckButton::SetBounds(BoundsBase<float> const&, BoundsBase<float> const&)
{
    throw std::logic_error("Not implemented");
}

CheckButton::~CheckButton()
{
    throw std::logic_error("Not implemented");
}

CheckButton::CheckButton()
{
    throw std::logic_error("Not implemented");
}

CheckButton::CheckButton(CheckButton const&)
{
    throw std::logic_error("Not implemented");
}

CStr CheckButton::CalcFullName() const
{
    throw std::logic_error("Not implemented");
}

void CheckButton::SetBounds(BoundsBase<float> const&, bool)
{
    throw std::logic_error("Not implemented");
}
