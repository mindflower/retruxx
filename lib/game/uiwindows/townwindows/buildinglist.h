#pragma once
#include <ui/button.h>

namespace ai
{
    class Building;
    class Town;
}  // namespace ai

namespace m3d
{
    namespace ui
    {
        class ImageWnd;
    }
}  // namespace m3d

class BuildingList;

class BuildingButton : public m3d::ui::ButtonWnd
{
    // BuildingList owns the shared button pattern and drives Load/ClearPattern.
    friend class BuildingList;

public:
    int SetUpForBuilding(ai::Building const* building);
    ai::Building* GetBuilding() const;
    void SetIdx(int idx);
    int GetIdx() const;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_wndIcoName;
        /* 0x0018 */ CStr m_lblNameName;
        /* 0x0024 */ m3d::ui::ButtonWnd* m_wndPattern;
        /* 0x0028 */ m3d::ui::ImageWnd* m_wndPatternIco;
        /* 0x002c */ m3d::ui::Wnd* m_lblPatternName;
        AuxInfo(BuildingButton::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0030 */

private:
    m3d::rend::TexHandle GetIcoForBuilding(ai::Building const* building) const;
    CStr GetNameForBuilding(ai::Building const* building) const;
    int CreateFromPattern();
    int CreateChildren();
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    static void __fastcall ClearPattern();
    /* 0x023c */ int m_buildingId;
    /* 0x0240 */ m3d::ui::ImageWnd* m_wndBuildingIco;
    /* 0x0244 */ m3d::ui::Wnd* m_lblBuildingName;
    /* 0x0248 */ int m_idx;
    static inline BuildingButton::AuxInfo m_aif;

protected:
    BuildingButton();
    BuildingButton(BuildingButton const& rhs);

public:
    virtual ~BuildingButton() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classBuildingButton;
}; /* size: 0x024c */

class BuildingList : public m3d::ui::Wnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    void SetUpForTown(ai::Town* town);

    struct BuildingList::AuxInfo
    {
        /* 0x0000 */ float m_spaceY;
        AuxInfo();
    }; /* size: 0x0004 */

    using ButtonsVector = std::vector<BuildingButton*, std::allocator<BuildingButton*>>;

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnAfterRemoveFromWndStation() override /* 0x74 */;
    void RecalcLayot();
    int AddButton(BuildingButton* btn);
    void PlaceButton(BuildingButton* btn);
    void FullUpdate();
    int CreateItems();
    void ClearItems();
    ai::Town const* GetTown() const;
    /* 0x0220 */ std::vector<BuildingButton*, std::allocator<BuildingButton*>> m_buttons;
    /* 0x0230 */ int m_townId;
    /* 0x0234 */ BuildingList::AuxInfo m_aif;
    BuildingList();
    BuildingList(BuildingList const& rhs);

public:
    virtual ~BuildingList() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classBuildingList;
}; /* size: 0x0238 */
