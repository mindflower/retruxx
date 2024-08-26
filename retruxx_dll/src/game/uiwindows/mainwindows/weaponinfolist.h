#pragma once
#include <core/ref_ptr.h>
#include <server/damageinfo.h>
#include <ui/ui.h>
#include <ui/wnd.h>

namespace ai
{
    class Obj;
    class Vehicle;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class ImageWnd;
        class ProgressBarWnd;
    }
}

class WeaponInfoList : public m3d::ui::Wnd
{
public:
    enum Type
    {
        TYPE_IN_MAIN_INTERFACE = 0,
        TYPE_IN_CHARACTERISTIC_WND = 1,
        TYPE_NUM_TYPES = 2,
    };

    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        float m_smallSpace;
        float m_bigSpace;
        float m_bottom;
        float m_groupInMainUIW;
        float m_groupInCharWndW;
        float m_groupInFignyaWndW;
        float m_groupInCharWndSpace;
        CStr m_groupTexInMainUi;
        CStr m_groupTexInCharWnd;
        CStr m_groupTexInFignyaWnd;
        CStr m_wndDecorTopName;
        CStr m_wndDecorBottomName;
    };

public:
    static m3d::Class* GetBaseClass();
    static m3d::Object* CreateObject();
    int CreateFromPattern(m3d::ui::Wnd*, bool);
    virtual m3d::Object* Clone();
    virtual m3d::Class* GetClass() const;
    virtual ~WeaponInfoList();

protected:
    void UpdateOnKeyBindingsChanged();
    int RecalcLayotInMainInterface();
    virtual int GameDataSetup();
    virtual int OnWndNotify(m3d::ui::Wnd*, unsigned int, unsigned int, m3d::AIParam const&);
    WeaponInfoList();
    WeaponInfoList(WeaponInfoList const&);
    void GetItemsByGroup(int, std::vector<ref_ptr<class WeaponInfoWnd>>&) const;
    ai::Vehicle* GetVehicle() const;
    int GetNumOfGroups() const;
    int CreateItems();
    int RecalcLayot();
    virtual int GameDataUpdate(void*, int);
    int AddGroupButton(int, BoundsBase<float> const&, WeaponInfoWnd const*, int);
    bool CanGunShotToTarget(ai::Obj const*) const;
    int UpdateOnVehiclePartChanged(void*);
    void UpdateOnWeaponGroupChanged();
    m3d::rend::TexHandle GetGroupTextureForWeaponBtn(WeaponInfoWnd const*, int);
    int RecalcLayotInCharacteristicWnd();
    int GetGroupByVehiclePartName(CStr const&) const;
    virtual int OnBeforeAddToWndStation();
    int UpdateOnPlayerVehicleChanged();
    int AddItem(int, int);
    void InitGroupTextures();
    void ReleaseGroupTextures();
    void SetTooltipForGroupButton(m3d::ui::Wnd*);
    virtual int GameDataClear(bool);

public:
    RT_CLASS_DECLARE(WeaponInfoList);

private:
    std::vector<ref_ptr<WeaponInfoWnd>> m_items;
    std::vector<m3d::ui::Wnd*> m_groupButtons;
    int m_vehicleId;
    WeaponInfoList::AuxInfo m_aif;
    WeaponInfoList::Type m_type;
    std::vector<std::pair<m3d::rend::TexHandle, m3d::rend::TexHandle>> m_groupTextures;
    m3d::ui::ImageWnd* m_wndDecorTop;
    m3d::ui::ImageWnd* m_wndDecorBottom;
};

class WeaponInfoWnd :  public m3d::ui::Wnd
{
public:
    int GetGroupId() const ;
    ai::Obj const * GetGun() const ;
    static m3d::Object * CreateObject();
    virtual m3d::Class * GetClass() const ;
    static m3d::Class * GetBaseClass();
    virtual ~WeaponInfoWnd();
    int SetupForGun(int,int,WeaponInfoList::Type);
    virtual m3d::Object * Clone();

protected:
    WeaponInfoWnd();
    WeaponInfoWnd(WeaponInfoWnd const &);
    m3d::rend::TexHandle GetIco() const ;
    int GetAmmo() const ;
    void UpdateAmmo();
    void UpdateCanShotBg();
    void UpdateOnNewFrame();
    int CreateFromPattern(WeaponInfoList::Type);
    m3d::rend::TexHandle GetBarTexture(bool) const ;
    virtual int GameDataUpdate(void *,int);
    virtual int GameDataClear(bool);
    ai::DamageType GetWeaponTypeForGun(class ai::Obj const *) const ;
    void UpdateCharge();
    static void  ClearPattern(WeaponInfoList::Type);
    static int LoadPattern(m3d::ui::Wnd *,WeaponInfoList::Type);
    void UpdateIco();
    bool IsGunReady() const ;
    int CreateChildren(WeaponInfoList::Type);

public:
    RT_CLASS_DECLARE(WeaponInfoWnd);

private:
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

class WeaponGroupChoiceDlg :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_groupButtonName;
        unsigned int m_selTextColor;
    };

public:
    virtual m3d::Object * Clone();
    static CStr CreateTooltipForWeaponGroup(int);
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~WeaponGroupChoiceDlg();
    virtual m3d::Class * GetClass() const ;

protected:
    WeaponGroupChoiceDlg();
    WeaponGroupChoiceDlg(WeaponGroupChoiceDlg const &);
    virtual int GameDataSetup();
    void SelectButton(int);
    virtual int OnKey(unsigned short,unsigned char,unsigned int);
    void OnKeyBindingsChanged();
    virtual int GameDataUpdate(void *,int);
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);

public:
    RT_CLASS_DECLARE(WeaponGroupChoiceDlg);

private:
    std::vector<m3d::ui::ButtonWnd *> m_groupButons;
    WeaponGroupChoiceDlg::AuxInfo m_aif;
};
