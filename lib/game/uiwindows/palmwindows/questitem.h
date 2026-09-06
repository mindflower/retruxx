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
        /* 0x0000 */ float m_xSpace;
        /* 0x0004 */ float m_ySpace0;
        /* 0x0008 */ float m_ySpace1;
        /* 0x000c */ PointBase<float> m_navPointBtnSz;
        AuxInfo(const QuestItem::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0014 */

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0xc8 */;
    CStr GetQuestDizById(help::QuestType questType, int questId) const;
    int SetupNavPointBtn(help::QuestType questType, int questId);
    bool NeedNavPointBtnBeAdded(help::QuestType questType, int questId) const;
    void OnBtnNavPointClick(m3d::ui::Wnd* wndFrom, unsigned int idFrom, m3d::AIParam const& data);
    int AddNavPoint(NavPoint::NavPointType npType);
    int RemoveNavPoint(int npId);
    void UpdateQuestStatusBtn();
    void UpdateQuestLabelWnd();
    /* 0x0220 */ int m_questId;
    /* 0x0224 */ help::QuestType m_questType;
    /* 0x0228 */ QuestStatusBtn* m_btnQuestStatus;
    /* 0x022c */ QuestLabel* m_wndQuestLabel;
    /* 0x0230 */ NavPointButton* m_btnNavPoint;
    static QuestItem::AuxInfo m_aif;
    QuestItem();
    QuestItem(QuestItem const& rhs);

public:
    virtual ~QuestItem() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(QuestItem);
}; /* size: 0x0234 */

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
    virtual int CreateQuestStatusBtn(PointBase<float> const& origin, unsigned int id) /* 0x11c */;
    void SetNavPointType(NavPoint::NavPointType npType);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_bgTextures[3];
        /* 0x0024 */ PointBase<float> m_size;
        /* 0x002c */ PointBase<float> m_npTexSz;
        /* 0x0034 */ CStr m_strNpButton;
        /* 0x0040 */ unsigned int m_npTexColor;
        AuxInfo(const QuestStatusBtn::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0044 */

protected:
    virtual void OnNcPaint(m3d::ui::DrawInfo const& di, unsigned int clr) override /* 0x00 */;
    void UpdateIcon();
    void UpdateNpTexture();
    /* 0x023c */ bool m_isSelected;
    /* 0x023d */ char Padding_56[3];
    /* 0x0240 */ help::UnifyQuestStatus m_questStatus;
    static QuestStatusBtn::AuxInfo m_aif;
    /* 0x0244 */ NavPoint::NavPointType m_npType;
    /* 0x0248 */ m3d::rend::TexHandle m_npTex;
    QuestStatusBtn();
    QuestStatusBtn(QuestStatusBtn const& rhs);

public:
    virtual ~QuestStatusBtn() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(QuestStatusBtn);
}; /* size: 0x024c */

class QuestLabel : public m3d::ui::TextBoxWnd
{
    // QuestItem owns the label and re-tints it when the quest status changes.
    friend class QuestItem;

public:
    void SetSelected(bool isSelected);
    bool IsSelected() const;
    void SetQuestStatus(help::UnifyQuestStatus questStatus);
    help::UnifyQuestStatus GetQuestStatus() const;
    virtual int CreateQuestLabel(CStr const& caption, PointBase<float> const& origin, float maxWidth, unsigned int id)
        /* 0x144 */;
    BoundsBase<float> GetLastStringBounds() const;

    struct AuxInfo
    {
        /* 0x0000 */ unsigned int m_selColor;
        /* 0x0004 */ unsigned int m_unselColor;
        /* 0x0008 */ unsigned int m_selFailColor;
        /* 0x000c */ unsigned int m_unselFailColor;
        /* 0x0010 */ CStr m_crossTexName;
        /* 0x001c */ float m_crossLineH;
        AuxInfo(const QuestLabel::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0020 */

protected:
    void UpdateTextColor();
    virtual int OnPaint(m3d::ui::DrawInfo const& clipToIt) override /* 0x00 */;
    /* 0x0238 */ bool m_isSelected;
    /* 0x0239 */ char Padding_270[3];
    /* 0x023c */ help::UnifyQuestStatus m_questStatus;
    static QuestLabel::AuxInfo m_aif;
    QuestLabel();
    QuestLabel(QuestLabel const& rhs);

public:
    virtual ~QuestLabel() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(QuestLabel);
}; /* size: 0x0240 */
