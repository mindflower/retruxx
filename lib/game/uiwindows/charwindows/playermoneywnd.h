#pragma once
#include <ui/ui.h>

class PlayerMoneyWnd : public m3d::ui::Wnd
{
public:
    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndValueName;
        /* 0x000c */ CStr m_strIdMoney;
        AuxInfo(PlayerMoneyWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0018 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    void OnPlayerMoneyChanged();
    void OnStartLevel();
    void UpdateValue();
    /* 0x0220 */ m3d::ui::Wnd* m_wndValue;
    /* 0x0224 */ PlayerMoneyWnd::AuxInfo m_aif;
    /* 0x023c */ CStr m_strMoney;
    PlayerMoneyWnd();
    PlayerMoneyWnd(PlayerMoneyWnd const& rhs);

public:
    virtual ~PlayerMoneyWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classPlayerMoneyWnd;
}; /* size: 0x0248 */
