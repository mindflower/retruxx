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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Object* TakSebeQuestLogWnd::Clone()
{
    RETRUXX_NOT_IMPLEMENTED;
}

m3d::Class* TakSebeQuestLogWnd::GetClass() const
{
    RETRUXX_NOT_IMPLEMENTED;
}

bool TakSebeQuestLogWnd::CanQuestBeShown(help::QuestType, int) const
{
    RETRUXX_NOT_IMPLEMENTED;
}

int TakSebeQuestLogWnd::GameDataSetup()
{
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
    RETRUXX_NOT_IMPLEMENTED;
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
