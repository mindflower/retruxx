#pragma once

class PlayerMoneyWnd :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual ~PlayerMoneyWnd();
    virtual class m3d::Object * Clone();
protected:
    void OnStartLevel();
    PlayerMoneyWnd();
    PlayerMoneyWnd(class PlayerMoneyWnd const &);
    void UpdateValue();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    void OnPlayerMoneyChanged();
private:
    m3d::ui::Wnd *m_wndValue;
    PlayerMoneyWnd::AuxInfo m_aif;
    CStr m_strMoney;
};
