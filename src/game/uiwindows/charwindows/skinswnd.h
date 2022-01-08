#pragma once

class SkinsWnd :  public m3d::ui::Wnd
{
public:
    int SetupForVehicle(int);
    virtual ~SkinsWnd();
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * CreateObject();
    static struct m3d::Class * GetBaseClass();
    virtual class m3d::Object * Clone();
protected:
    SkinsWnd(class SkinsWnd const &);
    SkinsWnd();
    void FullUpdate();
    void ShowPrevSkin();
    virtual int OnWndNotify(class m3d::ui::Wnd *,unsigned int,unsigned int,class m3d::AIParam const &);
    class ai::Vehicle * GetVehicle() const ;
    void OnChangeSkin();
    void ShowNextSkin();
    void BuySkin();
    virtual int GameDataSetup();
    virtual int OnBeforeAddToWndStation();
    virtual int OnAfterRemoveFromWndStation();
    int GetSkinPrice(int,int) const ;
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
