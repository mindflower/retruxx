#pragma once
#include <ui/ui.h>

class PlayerMoneyWnd :  public m3d::ui::Wnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_wndValueName;
        CStr m_strIdMoney;
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~PlayerMoneyWnd();
    virtual m3d::Object * Clone();

protected:
    void OnStartLevel();
    PlayerMoneyWnd();
    PlayerMoneyWnd(PlayerMoneyWnd const &);
    void UpdateValue();
    virtual int GameDataSetup();
    virtual int GameDataUpdate(void *,int);
    void OnPlayerMoneyChanged();

public:
    RT_CLASS_DECLARE(PlayerMoneyWnd);

private:
    m3d::ui::Wnd *m_wndValue;
    PlayerMoneyWnd::AuxInfo m_aif;
    CStr m_strMoney;
};
