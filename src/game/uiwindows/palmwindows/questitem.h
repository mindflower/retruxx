#pragma once

class QuestItem :  public m3d::ui::Wnd
{
public:
    void UpdateOnQuestNavPointChangedChildIncluded();
    static struct m3d::Class * GetBaseClass();
    int FullUpdate();
    int GetQuestId() const ;
    virtual class m3d::Object * Clone();
    int CreateQuestItem(class PointBase<float>,float,enum help::QuestType,int,int);
    enum help::QuestType GetQuestType() const ;
    virtual ~QuestItem();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    void SetSelected(bool);
protected:
    QuestItem();
    QuestItem(class QuestItem const &);
    int RemoveNavPoint(int);
    int AddNavPoint(enum NavPoint::NavPointType);
    CStr GetQuestDizById(enum help::QuestType,int) const ;
    bool NeedNavPointBtnBeAdded(enum help::QuestType,int) const ;
    void UpdateQuestLabelWnd();
    void OnBtnNavPointClick(class m3d::ui::Wnd *,unsigned int,class m3d::AIParam const &);
    void UpdateQuestStatusBtn();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    int SetupNavPointBtn(enum help::QuestType,int);
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
    virtual struct m3d::Class * GetClass() const ;
    virtual int CreateQuestStatusBtn(class PointBase<float> const &,unsigned int);
    void SetNavPointType(enum NavPoint::NavPointType);
    bool IsSelected() const ;
    void SetQuestStatus(enum help::UnifyQuestStatus);
    enum help::UnifyQuestStatus GetQuestStatus() const ;
    void SetSelected(bool);
    virtual ~QuestStatusBtn();
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    virtual class m3d::Object * Clone();
protected:
    QuestStatusBtn(class QuestStatusBtn const &);
    QuestStatusBtn();
    virtual void OnNcPaint(struct m3d::ui::DrawInfo const &,unsigned int);
    void UpdateNpTexture();
    void UpdateIcon();
private:
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
    NavPoint::NavPointType m_npType;
    m3d::rend::TexHandle m_npTex;
};

class QuestLabel :  public m3d::ui::TextBoxWnd
{
public:
    struct BoundsBase<float> GetLastStringBounds() const ;
    virtual ~QuestLabel();
    virtual struct m3d::Class * GetClass() const ;
    virtual int CreateQuestLabel(CStr const &,class PointBase<float> const &,float,unsigned int);
    bool IsSelected() const ;
    void SetSelected(bool);
    void SetQuestStatus(enum help::UnifyQuestStatus);
    enum help::UnifyQuestStatus GetQuestStatus() const ;
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
protected:
    QuestLabel(class QuestLabel const &);
    QuestLabel();
    void UpdateTextColor();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
private:
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
};
