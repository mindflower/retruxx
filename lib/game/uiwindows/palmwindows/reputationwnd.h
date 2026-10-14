#pragma once
#include "screenwnd.h"
#include <game/uimisc/guihelper.h>
#include <ui/button.h>
#include <vector>

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class AuxRPanel;

class ReputationButton : public m3d::ui::ButtonWnd
{
public:
    int SetUpForClan(int clanBelong);
    int GetClanBelong() const;

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_wndClanIcoName;
        CStr m_lblClanNameName;
        CStr m_wndToleranceNameName;
        CStr m_strIdPeaceAvailable;
        unsigned int m_peaceAvailableColor;
        m3d::ui::Wnd* m_wndPattern;
        m3d::ui::ImageWnd* m_wndPatternClanIco;
        m3d::ui::Wnd* m_lblPatternClanName;
        m3d::ui::Wnd* m_wndPatternToleranceName;
        unsigned int m_friendToleranceColor;
        unsigned int m_enemyToleranceColor;
        CStr m_strIdToleranceFriend1;
        CStr m_strIdToleranceEnemy1;
        CStr m_strIdToleranceFriend2;
        CStr m_strIdToleranceEnemy2;
        AuxInfo(const ReputationButton::AuxInfo&);
        AuxInfo();
    };

protected:
    int CreateFromPattern();
    int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    void UpdateTolerance();
    void UpdateToleranceIndicator(help::BlackWightTolerance bwTolerance);
    unsigned int BWTolerance2Color(help::BlackWightTolerance bwTolerance) const;
    static CStr __fastcall BWTolerance2StrId1(help::BlackWightTolerance bwTolerance);
    CStr BWTolerance2StrId2(help::BlackWightTolerance bwTolerance) const;
    void UpdateTooltip(help::BlackWightTolerance bwTolerance);
    int m_clanBelong;
    m3d::ui::Wnd* m_lblClanName;
    m3d::ui::ImageWnd* m_wndClanIco;
    m3d::ui::Wnd* m_wndToleranceName;
    static ReputationButton::AuxInfo m_aif;
    ReputationButton();
    ReputationButton(ReputationButton const& rhs);

public:
    virtual ~ReputationButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ReputationButton);

    // ReputationList lays the buttons out across its panels and needs to build
    // them from the shared pattern.
    friend class ReputationList;
};

class AuxRPanel
{
public:
    // NOTE: the shipped declaration keeps Slot private; it is public here because
    // ReputationList names the slots when it walks a panel's two halves.
    enum Slot
    {
        SLOT_LEFT = 0,
        SLOT_RIGHT = 1,
        NUM_SLOTS = 2,
    };

public:
    AuxRPanel();
    ~AuxRPanel();
    ReputationButton* AddButton(int clanBelong, m3d::ui::Wnd* parentWnd);
    bool HasEmptySlot() const;
    ReputationButton* GetButtonBySlot(AuxRPanel::Slot slotId) const;

private:
    AuxRPanel::Slot GetEmptySlotId() const;
    ReputationButton* m_btns[2];
};

class ReputationList : public m3d::ui::Wnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    ReputationButton* AddButtonByClanBelong(int clanBelong);
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    void UpdateClans();

    struct AuxInfo
    {
        float m_spaceX;
        float m_spaceY;
        AuxInfo();
    };

    using AuxRPanelVector = std::vector<AuxRPanel*, std::allocator<AuxRPanel*>>;

protected:
    virtual int OnBeforeAddToWndStation() override;
    int GetPanelIdFitToPlaceButton();
    ReputationButton* GetButtonByClanBelong(int clanBelong) const;
    void ClearClans();
    PointBase<int> GetButtonPanelCoord(ReputationButton const* btn) const;
    void PlaceButton(ReputationButton* btn);
    ReputationList::AuxInfo m_aif;
    std::vector<AuxRPanel*, std::allocator<AuxRPanel*>> m_items;
    ReputationList();
    ReputationList(ReputationList const& rhs);

public:
    virtual ~ReputationList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ReputationList);
};

class ReputationWnd : public ScreenWnd
{
    struct AuxInfo
    {
        CStr m_reputationListName;
        AuxInfo(const ReputationWnd::AuxInfo&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnAfterAddToWndStation() override;
    void OnKnownClansChanged();
    ReputationList* m_reputationList;
    ReputationWnd::AuxInfo m_aif;
    ReputationWnd();
    ReputationWnd(ReputationWnd const& rhs);

public:
    virtual ~ReputationWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(ReputationWnd);
};
