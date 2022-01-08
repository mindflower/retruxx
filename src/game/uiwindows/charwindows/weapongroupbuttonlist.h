#pragma once

class WeaponGroupButtonList :  public m3d::ui::Wnd
{
public:
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual ~WeaponGroupButtonList();
    int SetupForGunPart(CStr const &,int);
protected:
    class ai::Vehicle const * GetVehicle() const ;
    int GetCurGroupId() const ;
    void UpdateCurGroupId();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    static int __fastcall LoadPattern(class m3d::ui::Wnd *);
    virtual int CreateChildren();
    void OnVehiclePartChanged(void *);
    virtual int CreateFromPattern();
    virtual int GameDataUpdate(void *,int);
    void OnBtnWeaponGroupClick(class m3d::ui::Wnd *,int,class m3d::AIParam const &);
    WeaponGroupButtonList(class WeaponGroupButtonList const &);
    WeaponGroupButtonList();
    bool IsSetForGun() const ;
    void AddWeaponToGroup(int);
    void UpdateButtonsState();
    void OnKeyBindingsChanged();
    class ai::VehiclePart const * GetGun() const ;
    void UpdateButtonsTooltips();
    void OnWeaponGroupChanged();
private:
    std::vector<WeaponGroupButton *> m_weaponGroupButtons;
    CStr m_gunPartName;
    int m_vehicleId;
};

class WeaponGroupButton :  public m3d::ui::ButtonWnd
{
public:
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    int GetGroupId() const ;
    void SetState(enum State);
    virtual ~WeaponGroupButton();
    int SetupForGroup(int);
    enum State GetState() const ;
    static struct m3d::Class * GetBaseClass();
protected:
    void UpdateTextures();
    WeaponGroupButton();
    WeaponGroupButton(class WeaponGroupButton const &);
    void GetTexturesByState(enum State,int,class m3d::rend::TexHandle &,class m3d::rend::TexHandle &,class m3d::rend::TexHandle &) const ;
private:
    int m_groupId;
    WeaponGroupButton::State m_state;
    WeaponGroupButton::AuxInfo m_aif;
};
