#pragma once
#include <ui/ui.h>

class PlayerMoneyWnd : public m3d::ui::Wnd
{
public:
    struct AuxInfo
    {
        CStr m_wndValueName;
        CStr m_strIdMoney;
        AuxInfo(PlayerMoneyWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    void OnPlayerMoneyChanged();
    void OnStartLevel();
    void UpdateValue();
    m3d::ui::Wnd* m_wndValue;
    PlayerMoneyWnd::AuxInfo m_aif;
    CStr m_strMoney;
    PlayerMoneyWnd();
    PlayerMoneyWnd(PlayerMoneyWnd const& rhs);

public:
    virtual ~PlayerMoneyWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classPlayerMoneyWnd;
};
