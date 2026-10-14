#pragma once
#include <ui/ui.h>

class SmartCursorWnd : public m3d::ui::Wnd
{
public:
    struct AuxDrawInfo
    {
        AuxDrawInfo(const SmartCursorWnd::AuxDrawInfo&);
        AuxDrawInfo();
        ~AuxDrawInfo();
        void SetTexture(m3d::rend::TexHandle tex);
        m3d::rend::TexHandle GetTexture() const;
        PointBase<float> m_sz;
        float m_angle;
        unsigned int m_color;
        PointBase<float> m_coord;
        m3d::rend::TexHandle m_tex;
    };

    using AuxDrawInfoVector = std::vector<SmartCursorWnd::AuxDrawInfo*, std::allocator<SmartCursorWnd::AuxDrawInfo*>>;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override;
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override;
    virtual void OnNewFrame();
    virtual void ClearDrawInfo();
    virtual void DrawTexture(const m3d::ui::DrawInfo& di, const SmartCursorWnd::AuxDrawInfo* texInfo) const;
    std::vector<SmartCursorWnd::AuxDrawInfo*, std::allocator<SmartCursorWnd::AuxDrawInfo*>> m_drawInfo;
    SmartCursorWnd();
    SmartCursorWnd(const SmartCursorWnd& rhs);

public:
    virtual ~SmartCursorWnd() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classSmartCursorWnd;
};
