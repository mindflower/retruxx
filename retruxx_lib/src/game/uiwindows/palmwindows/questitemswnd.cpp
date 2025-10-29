#include "questitemswnd.h"

RT_CLASS_EXPORTS_BEGIN(QuestItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemsWnd);

QuestItemsWnd::AuxInfo::AuxInfo()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItemsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* QuestItemsWnd::CreateObject()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* QuestItemsWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemsWnd::~QuestItemsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* QuestItemsWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemsWnd::ClearItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemsWnd::CreateItems()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::OnAfterAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemsWnd::FullUpdate()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemsWnd::OnQuestItemsChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::GameDataUpdate(void*, int)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::GameDataClear(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemsWnd::QuestItemsWnd(QuestItemsWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

QuestItemsWnd::QuestItemsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void QuestItemsWnd::OnItemsListSelChanged(m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int QuestItemsWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}
