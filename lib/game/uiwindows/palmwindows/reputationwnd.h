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
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndClanIcoName;
        /* 0x0018 */ CStr m_lblClanNameName;
        /* 0x0024 */ CStr m_wndToleranceNameName;
        /* 0x0030 */ CStr m_strIdPeaceAvailable;
        /* 0x003c */ unsigned int m_peaceAvailableColor;
        /* 0x0040 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0044 */ m3d::ui::ImageWnd* m_wndPatternClanIco;
        /* 0x0048 */ m3d::ui::Wnd* m_lblPatternClanName;
        /* 0x004c */ m3d::ui::Wnd* m_wndPatternToleranceName;
        /* 0x0050 */ unsigned int m_friendToleranceColor;
        /* 0x0054 */ unsigned int m_enemyToleranceColor;
        /* 0x0058 */ CStr m_strIdToleranceFriend1;
        /* 0x0064 */ CStr m_strIdToleranceEnemy1;
        /* 0x0070 */ CStr m_strIdToleranceFriend2;
        /* 0x007c */ CStr m_strIdToleranceEnemy2;
        AuxInfo(const ReputationButton::AuxInfo&);
        AuxInfo();
    }; /* size: 0x0088 */

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
    /* 0x023c */ int m_clanBelong;
    /* 0x0240 */ m3d::ui::Wnd* m_lblClanName;
    /* 0x0244 */ m3d::ui::ImageWnd* m_wndClanIco;
    /* 0x0248 */ m3d::ui::Wnd* m_wndToleranceName;
    static ReputationButton::AuxInfo m_aif;
    ReputationButton();
    ReputationButton(ReputationButton const& rhs);

public:
    virtual ~ReputationButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(ReputationButton);

    // ReputationList lays the buttons out across its panels and needs to build
    // them from the shared pattern.
    friend class ReputationList;
}; /* size: 0x024c */

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
    /* 0x0000 */ ReputationButton* m_btns[2];
}; /* size: 0x0008 */

class ReputationList : public m3d::ui::Wnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    ReputationButton* AddButtonByClanBelong(int clanBelong);
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    void UpdateClans();

    struct AuxInfo
    {
        /* 0x0000 */ float m_spaceX;
        /* 0x0004 */ float m_spaceY;
        AuxInfo();
    }; /* size: 0x0008 */

    using AuxRPanelVector = std::vector<AuxRPanel*, std::allocator<AuxRPanel*>>;

protected:
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    int GetPanelIdFitToPlaceButton();
    ReputationButton* GetButtonByClanBelong(int clanBelong) const;
    void ClearClans();
    PointBase<int> GetButtonPanelCoord(ReputationButton const* btn) const;
    void PlaceButton(ReputationButton* btn);
    /* 0x0220 */ ReputationList::AuxInfo m_aif;
    /* 0x0228 */ std::vector<AuxRPanel*, std::allocator<AuxRPanel*>> m_items;
    ReputationList();
    ReputationList(ReputationList const& rhs);

public:
    virtual ~ReputationList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(ReputationList);
}; /* size: 0x0238 */

class ReputationWnd : public ScreenWnd
{
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_reputationListName;
        AuxInfo(const ReputationWnd::AuxInfo&);
        AuxInfo();
    }; /* size: 0x000c */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x00 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x00 */;
    virtual int OnAfterAddToWndStation() override /* 0x00 */;
    void OnKnownClansChanged();
    /* 0x0224 */ ReputationList* m_reputationList;
    /* 0x0228 */ ReputationWnd::AuxInfo m_aif;
    ReputationWnd();
    ReputationWnd(ReputationWnd const& rhs);

public:
    virtual ~ReputationWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(ReputationWnd);
}; /* size: 0x0234 */
