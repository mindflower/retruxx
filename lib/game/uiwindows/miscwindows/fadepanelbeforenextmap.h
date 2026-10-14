#pragma once
#include <ui/ui.h>

class FadePanelBeforeNextMap : public m3d::ui::Wnd
{
protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override;
    void OnPlayerPassToMapFading();
    void OnNewFrame();
    void UpdateAlpha();
    void StartFading();
    unsigned int CalcCurColor() const;

    float m_alpha;
    unsigned int m_fadeStartTime;
    float m_fadePeriod;
    unsigned int m_blackColor;

    FadePanelBeforeNextMap();
    FadePanelBeforeNextMap(FadePanelBeforeNextMap const& rhs);

public:
    virtual ~FadePanelBeforeNextMap() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    RT_CLASS_DECLARE(FadePanelBeforeNextMap);
};
