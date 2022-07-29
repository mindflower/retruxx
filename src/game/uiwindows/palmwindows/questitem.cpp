#include "questitem.h"

RT_CLASS_EXPORTS_BEGIN(QuestItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItem);

void QuestItem::UpdateOnQuestNavPointChangedChildIncluded()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestItem::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

int QuestItem::FullUpdate()
{
    throw std::logic_error("Not implemented");
}

int QuestItem::GetQuestId() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestItem::Clone()
{
    throw std::logic_error("Not implemented");
}

int QuestItem::CreateQuestItem(PointBase<float>, float, help::QuestType, int, int)
{
    throw std::logic_error("Not implemented");
}

help::QuestType QuestItem::GetQuestType() const
{
    throw std::logic_error("Not implemented");
}

QuestItem::~QuestItem()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestItem::GetClass() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestItem::CreateObject()
{
    throw std::logic_error("Not implemented");
}

void QuestItem::SetSelected(bool)
{
    throw std::logic_error("Not implemented");
}

QuestItem::QuestItem()
{
    throw std::logic_error("Not implemented");
}

QuestItem::QuestItem(QuestItem const&)
{
    throw std::logic_error("Not implemented");
}

int QuestItem::RemoveNavPoint(int)
{
    throw std::logic_error("Not implemented");
}

int QuestItem::AddNavPoint(NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

CStr QuestItem::GetQuestDizById(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

bool QuestItem::NeedNavPointBtnBeAdded(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}

void QuestItem::UpdateQuestLabelWnd()
{
    throw std::logic_error("Not implemented");
}

void QuestItem::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

void QuestItem::UpdateQuestStatusBtn()
{
    throw std::logic_error("Not implemented");
}

int QuestItem::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int QuestItem::SetupNavPointBtn(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(QuestStatusBtn)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestStatusBtn);

m3d::Class* QuestStatusBtn::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int QuestStatusBtn::CreateQuestStatusBtn(PointBase<float> const&, unsigned)
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::SetNavPointType(NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

bool QuestStatusBtn::IsSelected() const
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::SetQuestStatus(help::UnifyQuestStatus)
{
    throw std::logic_error("Not implemented");
}

help::UnifyQuestStatus QuestStatusBtn::GetQuestStatus() const
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::SetSelected(bool)
{
    throw std::logic_error("Not implemented");
}

QuestStatusBtn::~QuestStatusBtn()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestStatusBtn::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestStatusBtn::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestStatusBtn::Clone()
{
    throw std::logic_error("Not implemented");
}

QuestStatusBtn::QuestStatusBtn(QuestStatusBtn const&)
{
    throw std::logic_error("Not implemented");
}

QuestStatusBtn::QuestStatusBtn()
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::UpdateNpTexture()
{
    throw std::logic_error("Not implemented");
}

void QuestStatusBtn::UpdateIcon()
{
    throw std::logic_error("Not implemented");
}

RT_CLASS_EXPORTS_BEGIN(QuestLabel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestLabel);

BoundsBase<float> QuestLabel::GetLastStringBounds() const
{
    throw std::logic_error("Not implemented");
}

QuestLabel::~QuestLabel()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestLabel::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int QuestLabel::CreateQuestLabel(CStr const&, PointBase<float> const&, float, unsigned)
{
    throw std::logic_error("Not implemented");
}

bool QuestLabel::IsSelected() const
{
    throw std::logic_error("Not implemented");
}

void QuestLabel::SetSelected(bool)
{
    throw std::logic_error("Not implemented");
}

void QuestLabel::SetQuestStatus(help::UnifyQuestStatus)
{
    throw std::logic_error("Not implemented");
}

help::UnifyQuestStatus QuestLabel::GetQuestStatus() const
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestLabel::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestLabel::GetBaseClass()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestLabel::Clone()
{
    throw std::logic_error("Not implemented");
}

QuestLabel::QuestLabel(QuestLabel const&)
{
    throw std::logic_error("Not implemented");
}

QuestLabel::QuestLabel()
{
    throw std::logic_error("Not implemented");
}

void QuestLabel::UpdateTextColor()
{
    throw std::logic_error("Not implemented");
}

int QuestLabel::OnPaint(m3d::ui::DrawInfo const&)
{
    throw std::logic_error("Not implemented");
}
