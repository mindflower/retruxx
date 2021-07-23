#pragma once

class QuestDizWnd :  public m3d::ui::Wnd
{
public:
    static class m3d::Object * __fastcall CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    int CreateFromPattern(class m3d::ui::Wnd const *,bool);
    static struct m3d::Class * __fastcall GetBaseClass();
    int SetupForQuest(enum help::QuestType,int);
    virtual ~QuestDizWnd();
    virtual class m3d::Object * Clone();
protected:
    void OnNavPointDeleted(void *);
    int SetupNavPointBtn(enum help::QuestType,int);
    int RemoveNavPoint(int);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    int SetupDataPlaceWnd(class QuestInfo const *);
    void OnNavPointAdded(void *);
    int SetupChildQuestList(enum help::QuestType,int);
    virtual int GameDataClear(bool);
    int SetupDizWnd(class QuestInfo const *);
    void OnNavPoint(void *);
    virtual int GameDataUpdate(void *,int);
    int AddNavPoint(enum NavPoint::NavPointType);
    void OnBtnNavPointClick(class m3d::ui::Wnd *,unsigned int,class m3d::AIParam const &);
    QuestDizWnd(class QuestDizWnd const &);
    QuestDizWnd();
    bool NeedNavPointBtnBeAdded(enum help::QuestType,int) const ;
private:
    NavPointButton *m_btnNavPoint;
    m3d::ui::Wnd *m_wndQuestDiz;
    m3d::ui::Wnd *m_wndDataPlace;
    ref_ptr<QuestItemList> m_lstChildQuests;
    help::QuestType m_questType;
    int m_questId;
    QuestDizWnd::AuxInfo m_aif;
};
