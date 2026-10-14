#pragma once
#include <game/uimisc/guihelper.h>
#include <game/uimisc/navpoint.h>
#include <ui/button.h>
#include <ui/textbox.h>
#include <ui/ui.h>

class NavPointButton;
class QuestLabel;
class QuestStatusBtn;

class QuestItem : public m3d::ui::Wnd
{
public:
    int CreateQuestItem(PointBase<float> origin, float maxWidth, help::QuestType questType, int questId, int id);
    int FullUpdate();
    help::QuestType GetQuestType() const;
    int GetQuestId() const;
    void SetSelected(bool isSelected);
    void UpdateOnQuestNavPointChangedChildIncluded();

    struct AuxInfo
    {
        float m_xSpace;
        float m_ySpace0;
        float m_ySpace1;
        PointBase<float> m_navPointBtnSz;
        AuxInfo(QuestItem::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override;
    CStr GetQuestDizById(help::QuestType questType, int questId) const;
    int SetupNavPointBtn(help::QuestType questType, int questId);
    bool NeedNavPointBtnBeAdded(help::QuestType questType, int questId) const;
    void OnBtnNavPointClick(m3d::ui::Wnd* wndFrom, unsigned int idFrom, m3d::AIParam const& data);
    int AddNavPoint(NavPoint::NavPointType npType);
    int RemoveNavPoint(int npId);
    void UpdateQuestStatusBtn();
    void UpdateQuestLabelWnd();
    int m_questId;
    help::QuestType m_questType;
    QuestStatusBtn* m_btnQuestStatus;
    QuestLabel* m_wndQuestLabel;
    NavPointButton* m_btnNavPoint;
    static QuestItem::AuxInfo m_aif;
    QuestItem();
    QuestItem(QuestItem const& rhs);

public:
    virtual ~QuestItem() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(QuestItem);
};

class QuestStatusBtn : public m3d::ui::ButtonWnd
{
    // QuestItem owns the button and refreshes its icon / nav-point marker
    // directly whenever the quest it stands for changes.
    friend class QuestItem;

public:
    void SetSelected(bool isSelected);
    bool IsSelected() const;
    void SetQuestStatus(help::UnifyQuestStatus questStatus);
    help::UnifyQuestStatus GetQuestStatus() const;
    virtual int CreateQuestStatusBtn(PointBase<float> const& origin, unsigned int id);
    void SetNavPointType(NavPoint::NavPointType npType);

    struct AuxInfo
    {
        CStr m_bgTextures[3];
        PointBase<float> m_size;
        PointBase<float> m_npTexSz;
        CStr m_strNpButton;
        unsigned int m_npTexColor;
        AuxInfo(QuestStatusBtn::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) override;
    void UpdateIcon();
    void UpdateNpTexture();
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
    static QuestStatusBtn::AuxInfo m_aif;
    NavPoint::NavPointType m_npType;
    m3d::rend::TexHandle m_npTex;
    QuestStatusBtn();
    QuestStatusBtn(QuestStatusBtn const& rhs);

public:
    virtual ~QuestStatusBtn() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(QuestStatusBtn);
};

class QuestLabel : public m3d::ui::TextBoxWnd
{
    // QuestItem owns the label and re-tints it when the quest status changes.
    friend class QuestItem;

public:
    void SetSelected(bool isSelected);
    bool IsSelected() const;
    void SetQuestStatus(help::UnifyQuestStatus questStatus);
    help::UnifyQuestStatus GetQuestStatus() const;
    virtual int CreateQuestLabel(CStr const& caption, PointBase<float> const& origin, float maxWidth, unsigned int id);
    BoundsBase<float> GetLastStringBounds() const;

    struct AuxInfo
    {
        unsigned int m_selColor;
        unsigned int m_unselColor;
        unsigned int m_selFailColor;
        unsigned int m_unselFailColor;
        CStr m_crossTexName;
        float m_crossLineH;
        AuxInfo(QuestLabel::AuxInfo const&);
        AuxInfo();
    };

protected:
    void UpdateTextColor();
    virtual int OnPaint(m3d::ui::DrawInfo const& clipToIt) override;
    bool m_isSelected;
    help::UnifyQuestStatus m_questStatus;
    static QuestLabel::AuxInfo m_aif;
    QuestLabel();
    QuestLabel(QuestLabel const& rhs);

public:
    virtual ~QuestLabel() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(QuestLabel);
};
