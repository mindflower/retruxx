#pragma once
#include <ui/ui.h>

namespace ai
{
    class Vehicle;
}

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
    }
}

class SkinSwitcher
{
public:
    static int __fastcall ApplySkin(int, ai::Vehicle*);
    SkinSwitcher(void);
    void ShowPrevSkin(void);
    static int __fastcall GetMaxSkinCount(ai::Vehicle*);
    void ShowNextSkin(void);
    static bool __fastcall IsSkinApplicable(int, ai::Vehicle*);
    ~SkinSwitcher(void);
    static int __fastcall GetVehicleSkin(ai::Vehicle*);
    void Reset(void);
    int GetCurSkin(void) const;
    int SetUp(int, m3d::ui::ButtonWnd*, m3d::ui::ButtonWnd*);

private:
    ai::Vehicle* GetVehicle(void) const;
    int GetPrevSkin(void) const;
    int GetNextSkin(void) const;
    int SetCurSkin(int);
    bool IsValid(void) const;
    void UpdateSkinButtonsStates(void);
    int m_vehicleId;
    m3d::ui::ButtonWnd* m_btnSkinNext;
    m3d::ui::ButtonWnd* m_btnSkinPrev;
    int m_curSkin;
};

class SkinsWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_btnPrevName;
        CStr m_btnNextName;
        CStr m_btnBuyName;
        CStr m_wndPriceName;
    };

public:
    int SetupForVehicle(int);
    virtual ~SkinsWnd();
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual m3d::Object * Clone();

protected:
    SkinsWnd(SkinsWnd const &);
    SkinsWnd();
    void FullUpdate();
    void ShowPrevSkin();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int, m3d::AIParam const &);
    ai::Vehicle * GetVehicle() const ;
    void OnChangeSkin();
    void ShowNextSkin();
    void BuySkin();
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    int GetSkinPrice(int,int) const ;

public:
    RT_CLASS_DECLARE(SkinsWnd);

private:
    SkinSwitcher *m_skinSwitcher;
    m3d::ui::ButtonWnd *m_btnPrev;
    m3d::ui::ButtonWnd *m_btnNext;
    m3d::ui::ButtonWnd *m_btnBuy;
    m3d::ui::Wnd *m_wndPrice;
    SkinsWnd::AuxInfo m_aif;
    int m_vehicleId;
    int m_savedSkinId;
};
