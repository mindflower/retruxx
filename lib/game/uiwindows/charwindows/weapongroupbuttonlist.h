#pragma once
#include <ui/button.h>

namespace ai
{
    class Vehicle;
    class VehiclePart;
}  // namespace ai

class WeaponGroupButton : public m3d::ui::ButtonWnd
{
public:
    enum State
    {
        STATE_UNSELECTED = 0x0,
        STATE_SELECTED = 0x1,
        STATE_DISABLED = 0x2,
    };

public:
    int SetupForGroup(int groupId);
    int GetGroupId() const;
    void SetState(WeaponGroupButton::State state);
    WeaponGroupButton::State GetState() const;

    struct AuxInfo
    {
        CStr m_texIdSelected;
        CStr m_texIdUnselectedOut;
        CStr m_texIdUnselectedIn;
        CStr m_texIdDisabled;
        AuxInfo(WeaponGroupButton::AuxInfo const& rhs);
        AuxInfo();
    };

protected:
    void GetTexturesByState(
        WeaponGroupButton::State state,
        int groupId,
        m3d::rend::TexHandle& texOut,
        m3d::rend::TexHandle& texIn,
        m3d::rend::TexHandle& texDown) const;
    void UpdateTextures();

    int m_groupId;
    WeaponGroupButton::State m_state;
    WeaponGroupButton::AuxInfo m_aif;

    WeaponGroupButton();
    WeaponGroupButton(WeaponGroupButton const& rhs);

public:
    virtual ~WeaponGroupButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WeaponGroupButton);
};

class WeaponGroupButtonList : public m3d::ui::Wnd
{
public:
    int SetupForGunPart(CStr const& gunPartName, int vehicleId);

    using WeaponGroupButtonVector = std::vector<WeaponGroupButton*, std::allocator<WeaponGroupButton*>>;
    using ButtonVector = std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>>;

    struct AuxInfo
    {
        CStr m_wndPatternName;
        CStr m_patternWeaponGroupButtonName;
        CStr m_wndPatternWeaponSlotName;
        AuxInfo(WeaponGroupButtonList::AuxInfo const& rhs);
        AuxInfo();
    };

    struct Pattern
    {
        Pattern(WeaponGroupButtonList::Pattern const& rhs);
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        m3d::ui::Wnd* m_wndPattern;
        ButtonVector m_patternWeaponGroupButtons;
        void Clear();
        int m_ref;
    };

protected:
    virtual int CreateFromPattern();
    virtual int CreateChildren();
    static int LoadPattern(m3d::ui::Wnd* pattern);
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override;
    void OnBtnWeaponGroupClick(m3d::ui::Wnd* wndFrom, int wndFromId, m3d::AIParam const& data);
    void OnWeaponGroupChanged();
    void OnVehiclePartChanged(void* data);
    int GetCurGroupId() const;
    void UpdateCurGroupId();
    bool IsSetForGun() const;
    void UpdateButtonsState();
    void AddWeaponToGroup(int groupId);
    ai::Vehicle const* GetVehicle() const;
    ai::VehiclePart const* GetGun() const;
    void OnKeyBindingsChanged();
    void UpdateButtonsTooltips();

    WeaponGroupButtonVector m_weaponGroupButtons;
    CStr m_gunPartName;
    int m_vehicleId;
    static inline WeaponGroupButtonList::Pattern m_pattern;
    static inline WeaponGroupButtonList::AuxInfo m_aif;

    WeaponGroupButtonList();
    WeaponGroupButtonList(WeaponGroupButtonList const& rhs);

public:
    virtual ~WeaponGroupButtonList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WeaponGroupButtonList);

    // WeaponSlotList owns the shared pattern's lifetime, holding a reference
    // across GameDataSetup / destruction.
    friend class WeaponSlotList;
};
