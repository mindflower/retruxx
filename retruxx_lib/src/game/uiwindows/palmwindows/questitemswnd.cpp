#include "questitemswnd.h"

RT_CLASS_EXPORTS_BEGIN(QuestItemsWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(QuestItemsWnd);

QuestItemsWnd::AuxInfo::AuxInfo()
{
    m_wndPictureName = "wndPicture";
    m_wndDizName = "wndDiz";
    m_wndItemsListName = "wndItemsList";
}

m3d::Class* QuestItemsWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(Wnd);
}

m3d::Object* QuestItemsWnd::CreateObject()
{
    return new QuestItemsWnd;
}

m3d::Class* QuestItemsWnd::GetClass() const
{
    return RT_CLASS_LOCAL(QuestItemsWnd);
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
    // TODO: QuestItemsWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
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
    m_wndPicture = 0;
    m_wndDiz = 0;
    m_wndItemsList = 0;
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
