#pragma once
#include <ui/ui.h>

class IgrokaMochatWnd : public m3d::ui::Wnd
{
protected:
    static int const NUM_BORDERS;
    static unsigned int const SHOW_TIME;
    static unsigned int const FADE_TIME;

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_boderNames[4];
        AuxInfo(IgrokaMochatWnd::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0030 */

protected:
    virtual int GameDataSetup() override /* 0x104 */;
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int GameDataClear(bool beforeContinuousLevel) override /* 0x108 */;
    virtual int OnBeforeAddToWndStation() override /* 0x68 */;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    void OnNewFrame();
    void OnPlayerVehicleDamaged(void* data);
    void UpdateAlpha();
    void UpdateStartTimes();
    void ShowBorder(int borderId, bool bShow);
    void HideBorders();
    std::vector<int, std::allocator<int>> GetBordersByAttackerId(int attackerId) const;
    /* 0x0220 */ m3d::rend::TexHandle m_borderTextures[4];
    /* 0x0230 */ BoundsBase<float> m_borderBounds[4];
    /* 0x0270 */ unsigned char m_borderAlpha[4];
    /* 0x0274 */ unsigned int m_borderStartTime[4];
    /* 0x0284 */ IgrokaMochatWnd::AuxInfo m_aif;
    /* 0x02b4 */ float m_integratedDamage;
    IgrokaMochatWnd();
    IgrokaMochatWnd(IgrokaMochatWnd const& rhs);

public:
    virtual ~IgrokaMochatWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classIgrokaMochatWnd;
}; /* size: 0x02b8 */
