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
        CStr m_wndPatternName;
        CStr m_wndIcoName;
        CStr m_lblNameName;
        m3d::ui::ButtonWnd* m_wndPattern;
        m3d::ui::ImageWnd* m_wndPatternIco;
        m3d::ui::Wnd* m_lblPatternName;
        AuxInfo(BuildingButton::AuxInfo const&);
        AuxInfo();
    };

private:
    m3d::rend::TexHandle GetIcoForBuilding(ai::Building const* building) const;
    CStr GetNameForBuilding(ai::Building const* building) const;
    int CreateFromPattern();
    int CreateChildren();
    static int LoadPattern(m3d::ui::Wnd* pattern);
    static void ClearPattern();
    int m_buildingId;
    m3d::ui::ImageWnd* m_wndBuildingIco;
    m3d::ui::Wnd* m_lblBuildingName;
    int m_idx;
    static inline BuildingButton::AuxInfo m_aif;

protected:
    BuildingButton();
    BuildingButton(BuildingButton const& rhs);

public:
    virtual ~BuildingButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBuildingButton;
};

class BuildingList : public m3d::ui::Wnd
{
public:
    int CreateFromPattern(m3d::ui::Wnd* patternWnd, bool deleteSrc);
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    void SetUpForTown(ai::Town* town);

    struct BuildingList::AuxInfo
    {
        float m_spaceY;
        AuxInfo();
    };

    using ButtonsVector = std::vector<BuildingButton*, std::allocator<BuildingButton*>>;

protected:
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnAfterRemoveFromWndStation() override;
    void RecalcLayot();
    int AddButton(BuildingButton* btn);
    void PlaceButton(BuildingButton* btn);
    void FullUpdate();
    int CreateItems();
    void ClearItems();
    ai::Town const* GetTown() const;
    std::vector<BuildingButton*, std::allocator<BuildingButton*>> m_buttons;
    int m_townId;
    BuildingList::AuxInfo m_aif;
    BuildingList();
    BuildingList(BuildingList const& rhs);

public:
    virtual ~BuildingList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classBuildingList;
};
