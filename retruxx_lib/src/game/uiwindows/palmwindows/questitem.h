#pragma once
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/button.h>
#include <ui/textbox.h>
#include <ui/ui.h>

class NavPointButton;
class QuestLabel;
class QuestStatusBtn;

class QuestItem :  public m3d::ui::Wnd
{
public:
    void UpdateOnQuestNavPointChangedChildIncluded();
    static m3d::Class * GetBaseClass();
    int FullUpdate();
    int GetQuestId() const ;
    virtual m3d::Object * Clone();
    int CreateQuestItem(PointBase<float>,float, help::QuestType,int,int);
    help::QuestType GetQuestType() const ;
    virtual ~QuestItem();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    void SetSelected(bool);

protected:
    QuestItem();
    QuestItem(QuestItem const &);
    int RemoveNavPoint(int);
    int AddNavPoint(NavPoint::NavPointType);
    CStr GetQuestDizById(help::QuestType,int) const ;
    bool NeedNavPointBtnBeAdded(help::QuestType,int) const ;
    void UpdateQuestLabelWnd();
    void OnBtnNavPointClick(m3d::ui::Wnd *,unsigned int, m3d::AIParam const &);
    void UpdateQuestStatusBtn();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    int SetupNavPointBtn(help::QuestType,int);

public:
    RT_CLASS_DECLARE(QuestItem);

private:
    int m_questId;
    help::QuestType m_questType;
    QuestStatusBtn *m_btnQuestStatus;
    QuestLabel *m_wndQuestLabel;
    NavPointButton *m_btnNavPoint;
};

class QuestStatusBtn :  public m3d::ui::ButtonWnd
{
public:
    virtual m3d::Class * GetClass() const ;
    virtual int CreateQuestStatusBtn(PointBase<float> const &,unsigned int);
    void SetNavPointType(NavPoint::NavPointType);
    bool IsSelected() const ;
    void SetQuestStatus(help::UnifyQuestStatus);
    help::UnifyQuestStatus GetQuestStatus() const ;
    void SetSelected(bool);
    virtual ~QuestStatusBtn();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();
    virtual m3d::Object * Clone();

protected:
    QuestStatusBtn(QuestStatusBtn const &);
    QuestStatusBtn();
    virtual void OnNcPaint(m3d::ui::DrawInfo const &,unsigned int);
    void UpdateNpTexture();
    void UpdateIcon();

public:
    RT_CLASS_DECLARE(QuestStatusBtn);

private:
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
    NavPoint::NavPointType m_npType;
    m3d::rend::TexHandle m_npTex;
};

class QuestLabel : public m3d::ui::TextBoxWnd
{
public:
    BoundsBase<float> GetLastStringBounds() const ;
    virtual ~QuestLabel();
    virtual m3d::Class * GetClass() const ;
    virtual int CreateQuestLabel(CStr const &, PointBase<float> const &,float,unsigned int);
    bool IsSelected() const ;
    void SetSelected(bool);
    void SetQuestStatus(help::UnifyQuestStatus);
    help::UnifyQuestStatus GetQuestStatus() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();

protected:
    QuestLabel(QuestLabel const &);
    QuestLabel();
    void UpdateTextColor();
    virtual int OnPaint(m3d::ui::DrawInfo const &);

public:
    RT_CLASS_DECLARE(QuestLabel);

private:
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
};
