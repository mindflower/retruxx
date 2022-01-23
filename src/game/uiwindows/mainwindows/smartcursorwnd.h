#pragma once
#include <ui/ui.h>

class SmartCursorWnd :  public m3d::ui::Wnd
{
public:
    class AuxDrawInfo
    {
    public:
        AuxDrawInfo(void);
        void SetTexture(m3d::rend::TexHandle);
        m3d::rend::TexHandle GetTexture(void) const;
        ~AuxDrawInfo();

    private:
        PointBase<float> m_sz;
        float m_angle;
        unsigned int m_color;
        PointBase<float> m_coord;
        m3d::rend::TexHandle m_tex;
    };

public:
    virtual m3d::Object * Clone();
    static m3d::Object * CreateObject();
    static m3d::Class * GetBaseClass();
    virtual ~SmartCursorWnd();
    virtual m3d::Class * GetClass() const ;

protected:
    SmartCursorWnd(SmartCursorWnd const &);
    SmartCursorWnd();
    virtual void OnNewFrame();
    virtual int OnPaint(m3d::ui::DrawInfo const &);
    virtual int GameDataUpdate(void *,int);
    virtual void ClearDrawInfo();
    virtual void DrawTexture(m3d::ui::DrawInfo const &, AuxDrawInfo const *) const ;

public:
    RT_CLASS_DECLARE(SmartCursorWnd);

private:
    std::vector<SmartCursorWnd::AuxDrawInfo *> m_drawInfo;
};
