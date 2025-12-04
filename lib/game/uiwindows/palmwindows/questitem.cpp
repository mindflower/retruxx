#include "questitem.h"

RT_CLASS_EXPORTS_BEGIN(QuestItem)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItem);

void QuestItem::UpdateOnQuestNavPointChangedChildIncluded()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItem::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int QuestItem::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::GetQuestId() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestItem::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::CreateQuestItem(PointBase<float>, float, help::QuestType, int, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

help::QuestType QuestItem::GetQuestType() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem::~QuestItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItem::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestItem::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItem::SetSelected(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem::QuestItem()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItem::QuestItem(QuestItem const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::RemoveNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::AddNavPoint(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

CStr QuestItem::GetQuestDizById(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestItem::NeedNavPointBtnBeAdded(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItem::UpdateQuestLabelWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItem::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItem::UpdateQuestStatusBtn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItem::SetupNavPointBtn(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(QuestStatusBtn)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestStatusBtn);

m3d::Class* QuestStatusBtn::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestStatusBtn::CreateQuestStatusBtn(PointBase<float> const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::SetNavPointType(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestStatusBtn::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::SetQuestStatus(help::UnifyQuestStatus)
{
    RETRUXX_NOT_IMPLEMENTED;
}

help::UnifyQuestStatus QuestStatusBtn::GetQuestStatus() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::SetSelected(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestStatusBtn::~QuestStatusBtn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestStatusBtn::GetBaseClass()
{
    return RT_CLASS_LOCAL(ButtonWnd);
}

m3d::Object* QuestStatusBtn::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestStatusBtn::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestStatusBtn::QuestStatusBtn(QuestStatusBtn const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestStatusBtn::QuestStatusBtn()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::OnNcPaint(m3d::ui::DrawInfo const&, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::UpdateNpTexture()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestStatusBtn::UpdateIcon()
{
    RETRUXX_NOT_IMPLEMENTED;
}

RT_CLASS_EXPORTS_BEGIN(QuestLabel)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestLabel);

BoundsBase<float> QuestLabel::GetLastStringBounds() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestLabel::~QuestLabel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestLabel::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestLabel::CreateQuestLabel(CStr const&, PointBase<float> const&, float, unsigned)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestLabel::IsSelected() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestLabel::SetSelected(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestLabel::SetQuestStatus(help::UnifyQuestStatus)
{
    RETRUXX_NOT_IMPLEMENTED;
}

help::UnifyQuestStatus QuestLabel::GetQuestStatus() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestLabel::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestLabel::GetBaseClass()
{
    return RT_CLASS_LOCAL(TextBoxWnd);
}

m3d::Object* QuestLabel::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestLabel::QuestLabel(QuestLabel const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestLabel::QuestLabel()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestLabel::UpdateTextColor()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestLabel::OnPaint(m3d::ui::DrawInfo const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}
