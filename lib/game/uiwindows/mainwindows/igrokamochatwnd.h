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
        CStr m_boderNames[4];
        AuxInfo(IgrokaMochatWnd::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int GameDataClear(bool beforeContinuousLevel) override;
    virtual int OnBeforeAddToWndStation() override;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    void OnNewFrame();
    void OnPlayerVehicleDamaged(void* data);
    void UpdateAlpha();
    void UpdateStartTimes();
    void ShowBorder(int borderId, bool bShow);
    void HideBorders();
    std::vector<int, std::allocator<int>> GetBordersByAttackerId(int attackerId) const;
    m3d::rend::TexHandle m_borderTextures[4];
    BoundsBase<float> m_borderBounds[4];
    unsigned char m_borderAlpha[4];
    unsigned int m_borderStartTime[4];
    IgrokaMochatWnd::AuxInfo m_aif;
    float m_integratedDamage;
    IgrokaMochatWnd();
    IgrokaMochatWnd(IgrokaMochatWnd const& rhs);

public:
    virtual ~IgrokaMochatWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* CreateObject();
    static m3d::Class* GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classIgrokaMochatWnd;
};
