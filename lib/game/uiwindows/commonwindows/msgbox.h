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
        CStr m_wndTitleName = "wndTitle";
        CStr m_wndMsgName = "wndMsg";
        CStr m_wndUpLineName = "wndUpLine";
        CStr m_wndDownLineName = "wndDownLine";
        CStr m_buttonName = "button_";
        CStr m_idioticEmbossName = "buttonEmboss_";
        CStr m_wndFrameName = "wndFrame";
        CStr m_wndBgName = "wndBg";
        AuxInfo(MsgBox::AuxInfo const&);
        AuxInfo();
    };

protected:
    virtual int GameDataSetup() override;
    virtual int CloseModal(int ret) override;
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

    m3d::ui::Wnd* m_wndTitle = nullptr;
    m3d::ui::Wnd* m_wndMsg = nullptr;
    m3d::ui::LineWnd* m_wndUpLine = nullptr;
    m3d::ui::LineWnd* m_wndDownLine = nullptr;
    m3d::ui::ButtonWnd* m_buttons[3] = {};
    m3d::ui::Wnd* m_idioticEmbosses[3] = {};
    m3d::ui::Wnd* m_wndFrame = nullptr;
    m3d::ui::Wnd* m_wndBg = nullptr;

    static m3d::ui::Wnd* m_pattern;
    static int m_ref;
    
    unsigned int m_msgBoxFlags = 0;
    CStr m_msg;
    CStr m_title;
    MsgBox::AuxInfo m_aif;
    MsgBox();
    MsgBox(MsgBox const& rhs);

public:
    virtual ~MsgBox() override;
    virtual m3d::Object* Clone() override;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override;
    static m3d::Class m_classMsgBox;
};
