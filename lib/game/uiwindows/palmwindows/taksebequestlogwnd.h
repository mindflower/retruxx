#pragma once
#include "screenwnd.h"
#include <core/ref_ptr.h>
#include <game/uimisc/guihelper.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class QuestItemsWnd;
class QuestDizWnd;
class QuestItemList;
class MapComboBox;

class TakSebeQuestLogWnd :  public ScreenWnd
{
public:
    class TsqlAuxInfo
    {
        friend class TakSebeQuestLogWnd;

    public:
        TsqlAuxInfo();

    private:
        CStr m_cbMapsName;
        CStr m_lstQuestsName;
        CStr m_wndQuestDizName;
        CStr m_btnFilterName;
        CStr m_btnShowQuestItemsName;
        CStr m_icoIdFilterShowAll_out;
        CStr m_icoIdFilterShowAll_in;
        CStr m_icoIdFilterShowAll_down;
        CStr m_icoIdFilterShowNoncompleted_out;
        CStr m_icoIdFilterShowNoncompleted_in;
        CStr m_icoIdFilterShowNoncompleted_down;
        CStr m_strIdFilterShowAll;
        CStr m_strIdFilterShowNoncompleted;
        CStr m_strIdShowQuestItems;
        CStr m_strIdShowQuests;
        CStr m_strIdQuestItems;
        CStr m_strIdQuests;
        CStr m_wndLstQuestsFrameName;
        CStr m_wndEmbossCbMapsName;
        CStr m_wndEmbossBtnFilterName;
    };

public:
    virtual ~TakSebeQuestLogWnd();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    bool CanQuestBeShown(help::QuestType,int) const ;

protected:
    virtual int GameDataSetup();
    bool IsInQuestItemMode();
    void UpdateFilterButtonTooltip();
    void UpdateFilterButtonState();
    TakSebeQuestLogWnd(TakSebeQuestLogWnd const &);
    TakSebeQuestLogWnd();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    void ShowQuestItemsWnd(bool);
    void OnCurProfileParamChanged(void *);
    void OnCurProfileChanged();
    virtual int GameDataUpdate(void *,int);
    void ToggleQuestItemsWnd();
    void OnCompleteFilterPartChanged();
    int OnChangeFilter();
    void ToggleFilter();
    int ShowAll();
    int SetFilter0(long);
    int SetFilter(long);
    short GetCompleteFilterPart(long) const ;
    int SetCompleteFilterPart(short);
    bool IsCompleteStatusFit(help::QuestType,int,short) const ;
    virtual int OnBeforeAddToWndStation();
    bool IsMapFit(help::QuestType,int,short) const ;
    int ShowNoncompleted();
    int ChangeMap();
    int MakeFilterFromParts(short,short);
    void UpdateQuestItemsButtonState();
    void UpdateQuestItemsButtonTooltip();
    short GetMapFilterPart(long) const ;
    int SetMapFilterPart(short);
    int OnChangeSelectedQuest();

public:
    RT_CLASS_DECLARE(TakSebeQuestLogWnd);

private:
    TakSebeQuestLogWnd::TsqlAuxInfo m_tsqlAif;
    ref_ptr<MapComboBox> m_cbMaps;
    int m_filter;
    ref_ptr<QuestItemList> m_lstQuests;
    ref_ptr<QuestDizWnd> m_wndQuestDiz;
    ref_ptr<QuestItemsWnd> m_wndQuestItems;
    m3d::ui::ButtonWnd *m_btnFilter;
    m3d::ui::ButtonWnd *m_btnShowQuestItems;
    m3d::ui::Wnd *m_wndLstQuestsFrame;
    m3d::ui::Wnd *m_wndEmbossCbMaps;
    m3d::ui::Wnd *m_wndEmbossBtnFilter;
};
