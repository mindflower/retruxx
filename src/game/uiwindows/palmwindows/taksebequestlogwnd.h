#pragma once

class TakSebeQuestLogWnd :  public ScreenWnd
{
public:
    virtual ~TakSebeQuestLogWnd();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    bool CanQuestBeShown(enum help::QuestType,int) const ;
protected:
    virtual int GameDataSetup();
    bool IsInQuestItemMode();
    void UpdateFilterButtonTooltip();
    void UpdateFilterButtonState();
    TakSebeQuestLogWnd(class TakSebeQuestLogWnd const &);
    TakSebeQuestLogWnd();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
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
    bool IsCompleteStatusFit(enum help::QuestType,int,short) const ;
    virtual int OnBeforeAddToWndStation();
    bool IsMapFit(enum help::QuestType,int,short) const ;
    int ShowNoncompleted();
    int ChangeMap();
    int MakeFilterFromParts(short,short);
    void UpdateQuestItemsButtonState();
    void UpdateQuestItemsButtonTooltip();
    short GetMapFilterPart(long) const ;
    int SetMapFilterPart(short);
    int OnChangeSelectedQuest();
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
