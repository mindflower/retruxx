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
    struct TsqlAuxInfo
    {
        /* 0x0000 */ CStr m_cbMapsName;
        /* 0x000c */ CStr m_lstQuestsName;
        /* 0x0018 */ CStr m_wndQuestDizName;
        /* 0x0024 */ CStr m_btnFilterName;
        /* 0x0030 */ CStr m_btnShowQuestItemsName;
        /* 0x003c */ CStr m_icoIdFilterShowAll_out;
        /* 0x0048 */ CStr m_icoIdFilterShowAll_in;
        /* 0x0054 */ CStr m_icoIdFilterShowAll_down;
        /* 0x0060 */ CStr m_icoIdFilterShowNoncompleted_out;
        /* 0x006c */ CStr m_icoIdFilterShowNoncompleted_in;
        /* 0x0078 */ CStr m_icoIdFilterShowNoncompleted_down;
        /* 0x0084 */ CStr m_strIdFilterShowAll;
        /* 0x0090 */ CStr m_strIdFilterShowNoncompleted;
        /* 0x009c */ CStr m_strIdShowQuestItems;
        /* 0x00a8 */ CStr m_strIdShowQuests;
        /* 0x00b4 */ CStr m_strIdQuestItems;
        /* 0x00c0 */ CStr m_strIdQuests;
        /* 0x00cc */ CStr m_wndLstQuestsFrameName;
        /* 0x00d8 */ CStr m_wndEmbossCbMapsName;
        /* 0x00e4 */ CStr m_wndEmbossBtnFilterName;
        TsqlAuxInfo(const TakSebeQuestLogWnd::TsqlAuxInfo&);
        TsqlAuxInfo();
    }; /* size: 0x00f0 */

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
    // Declared in the shipped headers but fully inlined away - the binary
    // carries no standalone code for it.
    int ShowQuestInfoForMap(CStr const&);
    int MakeFilterFromParts(short,short);
    void UpdateQuestItemsButtonState();
    void UpdateQuestItemsButtonTooltip();
    short GetMapFilterPart(long) const ;
    int SetMapFilterPart(short);
    int OnChangeSelectedQuest();

public:
    RT_CLASS_DECLARE(TakSebeQuestLogWnd);

private:
    /* 0x0224 */ TakSebeQuestLogWnd::TsqlAuxInfo m_tsqlAif;
    /* 0x0314 */ ref_ptr<MapComboBox> m_cbMaps;
    /* 0x0318 */ long m_filter;
    /* 0x031c */ ref_ptr<QuestItemList> m_lstQuests;
    /* 0x0320 */ ref_ptr<QuestDizWnd> m_wndQuestDiz;
    /* 0x0324 */ ref_ptr<QuestItemsWnd> m_wndQuestItems;
    /* 0x0328 */ m3d::ui::ButtonWnd* m_btnFilter;
    /* 0x032c */ m3d::ui::ButtonWnd* m_btnShowQuestItems;
    /* 0x0330 */ m3d::ui::Wnd* m_wndLstQuestsFrame;
    /* 0x0334 */ m3d::ui::Wnd* m_wndEmbossCbMaps;
    /* 0x0338 */ m3d::ui::Wnd* m_wndEmbossBtnFilter;
}; /* size: 0x033c */
