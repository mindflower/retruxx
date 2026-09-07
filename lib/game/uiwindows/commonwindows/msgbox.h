#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class LineWnd;
    }  // namespace ui
}  // namespace m3d

class MsgBox : public m3d::ui::ModalWnd
{
public:
    int CreateMsgBox(CStr const& msg, CStr const& title, unsigned int flags);

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_wndTitleName;
        /* 0x000c */ CStr m_wndMsgName;
        /* 0x0018 */ CStr m_wndUpLineName;
        /* 0x0024 */ CStr m_wndDownLineName;
        /* 0x0030 */ CStr m_buttonName;
        /* 0x003c */ CStr m_idioticEmbossName;
        /* 0x0048 */ CStr m_wndFrameName;
        /* 0x0054 */ CStr m_wndBgName;
        AuxInfo(MsgBox::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x0060 */

protected:
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int CloseModal(int ret) override /* 0x124 */;
    int LoadPattern();
    void ClearPattern();
    int CreateFromPattern();
    void Clear();
    void RecalcLayot();
    void AddTitle();
    void AddMsg();
    void AddButtonsAndIdioticEmbosses();
    void AddLines();
    void AddMiscFignya();
    PointBase<float> CalcTitleSize() const;
    PointBase<float> CalcMsgSize() const;
    PointBase<float> CalcSummaryButtonsSize() const;
    PointBase<float> CalcSummaryIdioticEmbossesSize() const;
    void CalcSelfWidthByChildrensMaxWidth(float maxChildControlsWidth);
    void CalcSelfHeight();
    void SetTitleBounds(PointBase<float> const& titleSz);
    void SetUpLineBounds();
    void SetMsgBounds(PointBase<float> const& msgSz);
    void SetDownLineBounds();
    void SetButtonsBounds(PointBase<float> const& buttonsSz);
    void SetIdioticEmbossesBounds(PointBase<float> const& idioticEmbossesSz);
    void CenterOnScreen();
    void HackedExpandToScreen();

    enum Btn
    {
        BTN_YES = 0,
        BTN_NO = 1,
        BTN_CANCEL = 2,
        MAX_NUM_BUTTONS = 3,
    };

    /* 0x0224 */ m3d::ui::Wnd* m_wndTitle = nullptr;
    /* 0x0228 */ m3d::ui::Wnd* m_wndMsg = nullptr;
    /* 0x022c */ m3d::ui::LineWnd* m_wndUpLine = nullptr;
    /* 0x0230 */ m3d::ui::LineWnd* m_wndDownLine = nullptr;
    /* 0x0234 */ m3d::ui::ButtonWnd* m_buttons[3] = {};
    /* 0x0240 */ m3d::ui::Wnd* m_idioticEmbosses[3] = {};
    /* 0x024c */ m3d::ui::Wnd* m_wndFrame = nullptr;
    /* 0x0250 */ m3d::ui::Wnd* m_wndBg = nullptr;

    static m3d::ui::Wnd* m_pattern;
    static int m_ref;
    
    /* 0x0254 */ unsigned int m_msgBoxFlags = 0;
    /* 0x0258 */ CStr m_msg;
    /* 0x0264 */ CStr m_title;
    /* 0x0270 */ MsgBox::AuxInfo m_aif;
    MsgBox();
    MsgBox(MsgBox const& rhs);

public:
    virtual ~MsgBox() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    static m3d::Class m_classMsgBox;
}; /* size: 0x02d0 */
