#include "questdizwnd.h"
#include "questitemlist.h"

RT_CLASS_EXPORTS_BEGIN(QuestDizWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestDizWnd);

QuestDizWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestDizWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestDizWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestDizWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int QuestDizWnd::SetupForQuest(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestDizWnd::~QuestDizWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestDizWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestDizWnd::OnNavPointDeleted(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::SetupNavPointBtn(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::RemoveNavPoint(int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::SetupDataPlaceWnd(QuestInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestDizWnd::OnNavPointAdded(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::SetupChildQuestList(help::QuestType, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::SetupDizWnd(QuestInfo const*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestDizWnd::OnNavPoint(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestDizWnd::AddNavPoint(NavPoint::NavPointType)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestDizWnd::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestDizWnd::QuestDizWnd(QuestDizWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestDizWnd::QuestDizWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool QuestDizWnd::NeedNavPointBtnBeAdded(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}
