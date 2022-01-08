#pragma once

class WeaponInfoWnd :  public m3d::ui::Wnd
{
public:
    int GetGroupId() const ;
    class ai::Obj const * GetGun() const ;
    static class m3d::Object * CreateObject();
    virtual struct m3d::Class * GetClass() const ;
    static struct m3d::Class * GetBaseClass();
    virtual ~WeaponInfoWnd();
    int SetupForGun(int,int,enum WeaponInfoList::Type);
    virtual class m3d::Object * Clone();
protected:
    WeaponInfoWnd();
    WeaponInfoWnd(class WeaponInfoWnd const &);
    class m3d::rend::TexHandle GetIco() const ;
    int GetAmmo() const ;
    void UpdateAmmo();
    void UpdateCanShotBg();
    void UpdateOnNewFrame();
    int CreateFromPattern(enum WeaponInfoList::Type);
    class m3d::rend::TexHandle GetBarTexture(bool) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    enum ai::DamageType GetWeaponTypeForGun(class ai::Obj const *) const ;
    void UpdateCharge();
    static void __fastcall ClearPattern(enum WeaponInfoList::Type);
    static int __fastcall LoadPattern(class m3d::ui::Wnd *,enum WeaponInfoList::Type);
    void UpdateIco();
    bool IsGunReady() const ;
    int CreateChildren(enum WeaponInfoList::Type);
private:
    GetBaseClass();
    int m_gunId;
    m3d::ui::ProgressBarWnd *m_wndCharge;
    m3d::ui::ImageWnd *m_wndIco;
    m3d::ui::Wnd *m_wndName;
    m3d::ui::Wnd *m_wndAmmo;
    m3d::ui::Wnd *m_wndFrame0;
    m3d::ui::Wnd *m_wndFrame1;
    m3d::ui::ImageWnd *m_wndBg;
    m3d::ui::ImageWnd *m_wndCanShotBg;
    m3d::ui::Wnd *m_wndGroup;
    int m_groupId;
    WeaponInfoList::Type m_type;
};

class WeaponInfoList :  public m3d::ui::Wnd
{
public:
    static struct m3d::Class * GetBaseClass();
    static class m3d::Object * CreateObject();
    int CreateFromPattern(class m3d::ui::Wnd *,bool);
    virtual class m3d::Object * Clone();
    virtual struct m3d::Class * GetClass() const ;
    virtual ~WeaponInfoList();
protected:
    void UpdateOnKeyBindingsChanged();
    int RecalcLayotInMainInterface();
    virtual int GameDataSetup();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    WeaponInfoList();
    WeaponInfoList(class WeaponInfoList const &);
    void GetItemsByGroup(int,class std::vector<class ref_ptr<class WeaponInfoWnd>,class std::allocator<class ref_ptr<class WeaponInfoWnd> > > &) const ;
    class ai::Vehicle * GetVehicle() const ;
    int GetNumOfGroups() const ;
    int CreateItems();
    int RecalcLayot();
    virtual int GameDataUpdate(void *,int);
    int AddGroupButton(int,struct BoundsBase<float> const &,class WeaponInfoWnd const *,int);
    bool CanGunShotToTarget(class ai::Obj const *) const ;
    int UpdateOnVehiclePartChanged(void *);
    void UpdateOnWeaponGroupChanged();
    class m3d::rend::TexHandle GetGroupTextureForWeaponBtn(class WeaponInfoWnd const *,int);
    int RecalcLayotInCharacteristicWnd();
    int GetGroupByVehiclePartName(CStr const &) const ;
    virtual int OnBeforeAddToWndStation();
    int UpdateOnPlayerVehicleChanged();
    int AddItem(int,int);
    void InitGroupTextures();
    void ReleaseGroupTextures();
    void SetTooltipForGroupButton(class m3d::ui::Wnd *);
    virtual int GameDataClear(bool);
private:
    std::vector<ref_ptr<WeaponInfoWnd>> m_items;
    std::vector<m3d::ui::Wnd *> m_groupButtons;
    int m_vehicleId;
    WeaponInfoList::AuxInfo m_aif;
    WeaponInfoList::Type m_type;
    std::vector<std::pair<m3d::rend::TexHandle,m3d::rend::TexHandle>> m_groupTextures;
    m3d::ui::ImageWnd *m_wndDecorTop;
    m3d::ui::ImageWnd *m_wndDecorBottom;
};

class WeaponGroupChoiceDlg :  public m3d::ui::ModalWnd
{
public:
    virtual class m3d::Object * Clone();
    static CStr __fastcall CreateTooltipForWeaponGroup(int);
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual ~WeaponGroupChoiceDlg();
    virtual struct m3d::Class * GetClass() const ;
protected:
    WeaponGroupChoiceDlg();
    WeaponGroupChoiceDlg(class WeaponGroupChoiceDlg const &);
    virtual int GameDataSetup();
    void SelectButton(int);
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void OnKeyBindingsChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
private:
    std::vector<m3d::ui::ButtonWnd *> m_groupButons;
    WeaponGroupChoiceDlg::AuxInfo m_aif;
};
