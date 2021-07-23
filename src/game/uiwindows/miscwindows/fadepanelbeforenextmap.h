#pragma once

class FadePanelBeforeNextMap :  public m3d::ui::Wnd
{
public:
    virtual struct m3d::Class * GetClass() const ;
    virtual ~FadePanelBeforeNextMap();
    static class m3d::Object * __fastcall CreateObject();
    static struct m3d::Class * __fastcall GetBaseClass();
    virtual class m3d::Object * Clone();
protected:
    FadePanelBeforeNextMap(class FadePanelBeforeNextMap const &);
    FadePanelBeforeNextMap();
    unsigned int CalcCurColor() const ;
    void OnPlayerPassToMapFading();
    void UpdateAlpha();
    void StartFading();
    void OnNewFrame();
    virtual int OnPaint(struct m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
private:
    float m_alpha;
    unsigned int m_fadeStartTime;
    float m_fadePeriod;
    unsigned int m_blackColor;
};
