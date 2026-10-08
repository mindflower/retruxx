#pragma once
#include <ui/button.h>

namespace ai
{
    class Vehicle;
    class VehiclePart;
}

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
        /* 0x0000 */ CStr m_texIdSelected;
        /* 0x000c */ CStr m_texIdUnselectedOut;
        /* 0x0018 */ CStr m_texIdUnselectedIn;
        /* 0x0024 */ CStr m_texIdDisabled;
        AuxInfo(WeaponGroupButton::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0030 */

protected:
    void GetTexturesByState(
        WeaponGroupButton::State state,
        int groupId,
        m3d::rend::TexHandle& texOut,
        m3d::rend::TexHandle& texIn,
        m3d::rend::TexHandle& texDown) const;
    void UpdateTextures();

    /* 0x023c */ int m_groupId;
    /* 0x0240 */ WeaponGroupButton::State m_state;
    /* 0x0244 */ WeaponGroupButton::AuxInfo m_aif;

    WeaponGroupButton();
    WeaponGroupButton(WeaponGroupButton const& rhs);

public:
    virtual ~WeaponGroupButton() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WeaponGroupButton);
}; /* size: 0x0274 */

class WeaponGroupButtonList : public m3d::ui::Wnd
{
public:
    int SetupForGunPart(CStr const& gunPartName, int vehicleId);

    using WeaponGroupButtonVector = std::vector<WeaponGroupButton*, std::allocator<WeaponGroupButton*>>;
    using ButtonVector = std::vector<m3d::ui::ButtonWnd*, std::allocator<m3d::ui::ButtonWnd*>>;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndPatternName;
        /* 0x000c */ CStr m_patternWeaponGroupButtonName;
        /* 0x0018 */ CStr m_wndPatternWeaponSlotName;
        AuxInfo(WeaponGroupButtonList::AuxInfo const& rhs);
        AuxInfo();
    }; /* size: 0x0024 */

    struct Pattern
    {
        Pattern(WeaponGroupButtonList::Pattern const& rhs);
        Pattern();
        ~Pattern();
        void IncRef();
        void DecRef();
        int GetRef() const;

        /* 0x0000 */ m3d::ui::Wnd* m_wndPattern;
        /* 0x0004 */ ButtonVector m_patternWeaponGroupButtons;
        void Clear();
        /* 0x0014 */ int m_ref;
    }; /* size: 0x0018 */

protected:
    virtual int CreateFromPattern() /* 0x11c */;
    virtual int CreateChildren() /* 0x120 */;
    static int __fastcall LoadPattern(m3d::ui::Wnd* pattern);
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int id, unsigned int msg, m3d::AIParam const& data) override
        /* 0xc8 */;
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

    /* 0x0220 */ WeaponGroupButtonVector m_weaponGroupButtons;
    /* 0x0230 */ CStr m_gunPartName;
    /* 0x023c */ int m_vehicleId;
    static inline WeaponGroupButtonList::Pattern m_pattern;
    static inline WeaponGroupButtonList::AuxInfo m_aif;

    WeaponGroupButtonList();
    WeaponGroupButtonList(WeaponGroupButtonList const& rhs);

public:
    virtual ~WeaponGroupButtonList() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(WeaponGroupButtonList);

    // WeaponSlotList owns the shared pattern's lifetime, holding a reference
    // across GameDataSetup / destruction.
    friend class WeaponSlotList;
}; /* size: 0x0240 */
