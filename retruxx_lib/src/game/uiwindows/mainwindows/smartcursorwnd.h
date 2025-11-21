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
        /* 0x0000 */ PointBase<float> m_sz;
        /* 0x0008 */ float m_angle;
        /* 0x000c */ unsigned int m_color;
        /* 0x0010 */ PointBase<float> m_coord;
        /* 0x0018 */ m3d::rend::TexHandle m_tex;
    }; /* size: 0x001c */

    using AuxDrawInfoVector = std::vector<SmartCursorWnd::AuxDrawInfo*, std::allocator<SmartCursorWnd::AuxDrawInfo*>>;

protected:
    virtual int GameDataUpdate(void* data, int dataType) override /* 0x10c */;
    virtual int OnPaint(const m3d::ui::DrawInfo& di) override /* 0x88 */;
    virtual void OnNewFrame() /* 0x11c */;
    virtual void ClearDrawInfo() /* 0x120 */;
    virtual void DrawTexture(const m3d::ui::DrawInfo& di, const SmartCursorWnd::AuxDrawInfo* texInfo) const /* 0x124 */;
    /* 0x0220 */ std::vector<SmartCursorWnd::AuxDrawInfo*, std::allocator<SmartCursorWnd::AuxDrawInfo*>> m_drawInfo;
    SmartCursorWnd();
    SmartCursorWnd(const SmartCursorWnd& rhs);

public:
    virtual ~SmartCursorWnd() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x04 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x34 */;
    static m3d::Class m_classSmartCursorWnd;
}; /* size: 0x0230 */
