#pragma once
#include <core/ref_ptr.h>
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/ui.h>

class NavPointButton;
class QuestItemList;

class QuestDizWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
        friend class QuestDizWnd;

    public:
        AuxInfo();

    private:
        float m_spaceX;
        float m_spaceY;
        PointBase<float> m_btnNavPointSz;
    };

public:
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    int CreateFromPattern(m3d::ui::Wnd const *,bool);
    static m3d::Class * GetBaseClass();
    int SetupForQuest(help::QuestType,int);
    virtual ~QuestDizWnd();
    virtual m3d::Object * Clone();

protected:
    void OnNavPointDeleted(void *);
    int SetupNavPointBtn(help::QuestType,int);
    int RemoveNavPoint(int);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    int SetupDataPlaceWnd(class QuestInfo const *);
    void OnNavPointAdded(void *);
    int SetupChildQuestList(help::QuestType,int);
    virtual int GameDataClear(bool);
    int SetupDizWnd(QuestInfo const *);
    void OnNavPoint(void *);
    virtual int GameDataUpdate(void *,int);
    int AddNavPoint(NavPoint::NavPointType);
    void OnBtnNavPointClick(m3d::ui::Wnd *,unsigned int, m3d::AIParam const &);
    QuestDizWnd(QuestDizWnd const &);
    QuestDizWnd();
    bool NeedNavPointBtnBeAdded(help::QuestType,int) const ;

public:
    RT_CLASS_DECLARE(QuestDizWnd);

private:
    NavPointButton *m_btnNavPoint;
    m3d::ui::Wnd *m_wndQuestDiz;
    m3d::ui::Wnd *m_wndDataPlace;
    ref_ptr<QuestItemList> m_lstChildQuests;
    help::QuestType m_questType;
    int m_questId;
    QuestDizWnd::AuxInfo m_aif;
};
