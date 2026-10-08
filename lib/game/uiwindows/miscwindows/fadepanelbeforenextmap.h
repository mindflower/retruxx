#pragma once
#include <ui/ui.h>

class FadePanelBeforeNextMap : public m3d::ui::Wnd
{
protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnPaint(m3d::ui::DrawInfo const& di) override /* 0x88 */;
    void OnPlayerPassToMapFading();
    void OnNewFrame();
    void UpdateAlpha();
    void StartFading();
    unsigned int CalcCurColor() const;

    /* 0x0220 */ float m_alpha;
    /* 0x0224 */ unsigned int m_fadeStartTime;
    /* 0x0228 */ float m_fadePeriod;
    /* 0x022c */ unsigned int m_blackColor;

    FadePanelBeforeNextMap();
    FadePanelBeforeNextMap(FadePanelBeforeNextMap const& rhs);

public:
    virtual ~FadePanelBeforeNextMap() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    RT_CLASS_DECLARE(FadePanelBeforeNextMap);
}; /* size: 0x0230 */
