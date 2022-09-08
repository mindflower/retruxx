#include "questdizwnd.h"
#include "questitemlist.h"

RT_CLASS_EXPORTS_BEGIN(QuestDizWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestDizWnd);

QuestDizWnd::AuxInfo::AuxInfo()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestDizWnd::CreateObject()
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestDizWnd::GetClass() const
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::CreateFromPattern(m3d::ui::Wnd const*, bool)
{
    throw std::logic_error("Not implemented");
}

m3d::Class* QuestDizWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

int QuestDizWnd::SetupForQuest(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

QuestDizWnd::~QuestDizWnd()
{
    throw std::logic_error("Not implemented");
}

m3d::Object* QuestDizWnd::Clone()
{
    throw std::logic_error("Not implemented");
}

void QuestDizWnd::OnNavPointDeleted(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::SetupNavPointBtn(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::RemoveNavPoint(int)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::SetupDataPlaceWnd(QuestInfo const*)
{
    throw std::logic_error("Not implemented");
}

void QuestDizWnd::OnNavPointAdded(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::SetupChildQuestList(help::QuestType, int)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::GameDataClear(bool)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::SetupDizWnd(QuestInfo const*)
{
    throw std::logic_error("Not implemented");
}

void QuestDizWnd::OnNavPoint(void*)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::GameDataUpdate(void*, int)
{
    throw std::logic_error("Not implemented");
}

int QuestDizWnd::AddNavPoint(NavPoint::NavPointType)
{
    throw std::logic_error("Not implemented");
}

void QuestDizWnd::OnBtnNavPointClick(m3d::ui::Wnd*, unsigned, m3d::AIParam const&)
{
    throw std::logic_error("Not implemented");
}

QuestDizWnd::QuestDizWnd(QuestDizWnd const&)
{
    throw std::logic_error("Not implemented");
}

QuestDizWnd::QuestDizWnd()
{
    throw std::logic_error("Not implemented");
}

bool QuestDizWnd::NeedNavPointBtnBeAdded(help::QuestType, int) const
{
    throw std::logic_error("Not implemented");
}
