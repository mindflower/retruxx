#include "checklist.h"

RT_CLASS_EXPORTS_BEGIN(CheckList)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckList);

int CheckList::SelectButtonByName(CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CheckList::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CheckList::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckList::CreateFromPattern(m3d::ui::Wnd*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CheckList::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckList::AddButtonByName(CStr const&, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CheckList::SetCurSel(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckList::~CheckList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CheckList::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int CheckList::RenderItem(int, PointBase<float> const&, m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CheckList::SelectButton(CheckButton*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckList::MeasureItem(int, BoundsBase<float>&) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckList::DeleteItem(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckList::CheckList()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckList::CheckList(CheckList const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckList::CompareItem(int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(CheckButton)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(CheckButton);

m3d::Class* CheckButton::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CheckButton::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

BoundsBase<float> const& CheckButton::GetVirtualBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int CheckButton::SetUp(CStr const&, PointBase<float> const&, float, CStr const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* CheckButton::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* CheckButton::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

void CheckButton::Select(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& CheckButton::GetName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr const& CheckButton::GetFullName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CheckButton::SetBounds(BoundsBase<float> const&, BoundsBase<float> const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckButton::~CheckButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckButton::CheckButton()
{
    RETRUXX_NOT_IMPLEMENTED;
}

CheckButton::CheckButton(CheckButton const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr CheckButton::CalcFullName() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void CheckButton::SetBounds(BoundsBase<float> const&, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}
