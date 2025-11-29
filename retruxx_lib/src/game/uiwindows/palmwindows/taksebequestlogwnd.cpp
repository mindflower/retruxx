#include "taksebequestlogwnd.h"
#include "questitemswnd.h"
#include "questdizwnd.h"
#include "questitemlist.h"
#include "mapcombobox.h"

RT_CLASS_EXPORTS_BEGIN(TakSebeQuestLogWnd)
RT_CLASS_EXPORTS_END;
RT_CLASS_DEFINE(TakSebeQuestLogWnd);

TakSebeQuestLogWnd::TsqlAuxInfo::TsqlAuxInfo()
{
    m_cbMapsName = "cbMaps";
    m_lstQuestsName = "lstQuests";
    m_wndQuestDizName = "wndQuestDiz";
    m_btnFilterName = "btnFilter";
    m_btnShowQuestItemsName = "btnShowQuestItemsList";
    m_icoIdFilterShowAll_out = "QuestFilterShowAll_out";
    m_icoIdFilterShowAll_in = "QuestFilterShowAll_int";
    m_icoIdFilterShowAll_down = "QuestFilterShowAll_down";
    m_icoIdFilterShowNoncompleted_out = "QuestFilterShowNoncompleted_out";
    m_icoIdFilterShowNoncompleted_in = "QuestFilterShowNoncompleted_in";
    m_icoIdFilterShowNoncompleted_down = "QuestFilterShowNoncompleted_down";
    m_strIdFilterShowAll = "ShowAllQuests";
    m_strIdFilterShowNoncompleted = "ShowNoncompletedQuests";
    m_strIdShowQuestItems = "ShowQuestItems";
    m_strIdShowQuests = "ShowQuests";
    m_strIdQuestItems = "QuestItems";
    m_strIdQuests = "Quests";
    m_wndLstQuestsFrameName = "lstQuestsFrame";
    m_wndEmbossCbMapsName = "emboss_cbMaps";
    m_wndEmbossBtnFilterName = "emboss_btnFilter";
}

TakSebeQuestLogWnd::~TakSebeQuestLogWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TakSebeQuestLogWnd::GetBaseClass()
{
    return RT_CLASS_LOCAL(ScreenWnd);
}

m3d::Object* TakSebeQuestLogWnd::CreateObject()
{
    return new TakSebeQuestLogWnd;
}

m3d::Object* TakSebeQuestLogWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TakSebeQuestLogWnd::GetClass() const
{
    return RT_CLASS_LOCAL(TakSebeQuestLogWnd);
}

bool TakSebeQuestLogWnd::CanQuestBeShown(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::GameDataSetup()
{
    // TODO: implement TakSebeQuestLogWnd::GameDataSetup
    // RETRUXX_NOT_IMPLEMENTED;
    return 1;
}

bool TakSebeQuestLogWnd::IsInQuestItemMode()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::UpdateFilterButtonTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::UpdateFilterButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

TakSebeQuestLogWnd::TakSebeQuestLogWnd(TakSebeQuestLogWnd const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

TakSebeQuestLogWnd::TakSebeQuestLogWnd()
{
    m_btnShowQuestItems = 0;
    m_wndLstQuestsFrame = 0;
    m_wndEmbossCbMaps = 0;
    m_wndEmbossBtnFilter = 0;
}

int TakSebeQuestLogWnd::OnWndNotify(m3d::ui::Wnd*, unsigned, unsigned, m3d::AIParam const&)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::ShowQuestItemsWnd(bool)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::OnCurProfileParamChanged(void*)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::OnCurProfileChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::GameDataUpdate(void*, int)
{
    // TODO: implement TakSebeQuestLogWnd::GameDataUpdate
    // RETRUXX_NOT_IMPLEMENTED;
    return 0;
}

void TakSebeQuestLogWnd::ToggleQuestItemsWnd()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::OnCompleteFilterPartChanged()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::OnChangeFilter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::ToggleFilter()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::ShowAll()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::SetFilter0(long)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::SetFilter(long)
{
    RETRUXX_NOT_IMPLEMENTED;
}

short TakSebeQuestLogWnd::GetCompleteFilterPart(long) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::SetCompleteFilterPart(short)
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TakSebeQuestLogWnd::IsCompleteStatusFit(help::QuestType, int, short) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::OnBeforeAddToWndStation()
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TakSebeQuestLogWnd::IsMapFit(help::QuestType, int, short) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::ShowNoncompleted()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::ChangeMap()
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::MakeFilterFromParts(short, short)
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::UpdateQuestItemsButtonState()
{
    RETRUXX_NOT_IMPLEMENTED;
}

void TakSebeQuestLogWnd::UpdateQuestItemsButtonTooltip()
{
    RETRUXX_NOT_IMPLEMENTED;
}

short TakSebeQuestLogWnd::GetMapFilterPart(long) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::SetMapFilterPart(short)
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::OnChangeSelectedQuest()
{
    RETRUXX_NOT_IMPLEMENTED;
}
