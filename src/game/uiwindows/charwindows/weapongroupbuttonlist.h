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

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_texIdSelected;
        CStr m_texIdUnselectedOut;
        CStr m_texIdUnselectedIn;
        CStr m_texIdDisabled;
    };

public:
    static m3d::Object* CreateObject();
    virtual m3d::Class* GetClass() const;
    virtual m3d::Object* Clone();
    int GetGroupId() const;
    void SetState(State);
    virtual ~WeaponGroupButton();
    int SetupForGroup(int);
    State GetState() const;
    static m3d::Class* GetBaseClass();
protected:
    void UpdateTextures();
    WeaponGroupButton();
    WeaponGroupButton(WeaponGroupButton const&);
    void GetTexturesByState(State, int, m3d::rend::TexHandle&, m3d::rend::TexHandle&, m3d::rend::TexHandle&) const;

public:
    RT_CLASS_DECLARE(WeaponGroupButton);

private:
    int m_groupId;
    WeaponGroupButton::State m_state;
    WeaponGroupButton::AuxInfo m_aif;
};


class WeaponGroupButtonList :  public m3d::ui::Wnd
{
public:
    virtual m3d::Object * Clone();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~WeaponGroupButtonList();
    int SetupForGunPart(CStr const &,int);

protected:
    ai::Vehicle const * GetVehicle() const ;
    int GetCurGroupId() const ;
    void UpdateCurGroupId();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    static int __fastcall LoadPattern(m3d::ui::Wnd *);
    virtual int CreateChildren();
    void OnVehiclePartChanged(void *);
    virtual int CreateFromPattern();
    virtual int GameDataUpdate(void *,int);
    void OnBtnWeaponGroupClick(m3d::ui::Wnd *,int, m3d::AIParam const &);
    WeaponGroupButtonList(WeaponGroupButtonList const &);
    WeaponGroupButtonList();
    bool IsSetForGun() const ;
    void AddWeaponToGroup(int);
    void UpdateButtonsState();
    void OnKeyBindingsChanged();
    ai::VehiclePart const * GetGun() const ;
    void UpdateButtonsTooltips();
    void OnWeaponGroupChanged();

public:
    RT_CLASS_DECLARE(WeaponGroupButtonList);

private:
    std::vector<WeaponGroupButton *> m_weaponGroupButtons;
    CStr m_gunPartName;
    int m_vehicleId;
};
