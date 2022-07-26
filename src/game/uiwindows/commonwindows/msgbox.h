#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class LineWnd;
    }
}

class MsgBox :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    public:
        CStr m_wndTitleName = "wndTitle";
        CStr m_wndMsgName = "wndMsg";
        CStr m_wndUpLineName = "wndUpLine";
        CStr m_wndDownLineName = "wndDownLine";
        CStr m_buttonName = "button_";
        CStr m_idioticEmbossName = "buttonEmboss_";
        CStr m_wndFrameName = "wndFrame";
        CStr m_wndBgName = "wndBg";
    };

public:
    int CreateMsgBox(CStr const &,CStr const &,unsigned int);
    virtual m3d::Class * GetClass() const ;
    virtual m3d::Object * Clone();
    virtual ~MsgBox();
    static m3d::Class * GetBaseClass();
    static m3d::Object * CreateObject();

protected:
    void SetIdioticEmbossesBounds(PointBase<float> const &);
    MsgBox(MsgBox const &);
    MsgBox();
    void AddTitle();
    void SetMsgBounds(PointBase<float> const &);
    void AddMsg();
    void AddLines();
    void SetDownLineBounds();
    PointBase<float> CalcSummaryButtonsSize() const ;
    void Clear();
    PointBase<float> CalcTitleSize() const ;
    PointBase<float> CalcSummaryIdioticEmbossesSize() const ;
    void SetUpLineBounds();
    void AddMiscFignya();
    void SetTitleBounds(PointBase<float> const &);
    virtual int CloseModal(int);
    PointBase<float> CalcMsgSize() const ;
    void ClearPattern();
    void RecalcLayot();
    void CenterOnScreen();
    void HackedExpandToScreen();
    void CalcSelfWidthByChildrensMaxWidth(float);
    void AddButtonsAndIdioticEmbosses();
    void CalcSelfHeight();
    int LoadPattern();
    void SetButtonsBounds(PointBase<float> const &);
    virtual int GameDataSetup();
    int CreateFromPattern();

public:
    RT_CLASS_DECLARE(MsgBox);
    static int m_ref;
    static Wnd* m_pattern;

private:
    m3d::ui::Wnd *m_wndTitle = nullptr;
    m3d::ui::Wnd *m_wndMsg = nullptr;
    m3d::ui::LineWnd *m_wndUpLine = nullptr;
    m3d::ui::LineWnd *m_wndDownLine = nullptr;
    m3d::ui::ButtonWnd *m_buttons[3] = {nullptr};
    m3d::ui::Wnd *m_idioticEmbosses[3] = {nullptr};
    m3d::ui::Wnd *m_wndFrame = nullptr;
    m3d::ui::Wnd *m_wndBg = nullptr;
    unsigned int m_msgBoxFlags = 0;
    CStr m_msg;
    CStr m_title;
    MsgBox::AuxInfo m_aif;
};
