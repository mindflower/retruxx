#pragma once

class MsgBox :  public m3d::ui::ModalWnd
{
public:
    int CreateMsgBox(class CStr const &,class CStr const &,unsigned int);
    virtual struct m3d::Class * GetClass() const ;
    virtual class m3d::Object * Clone();
    virtual ~MsgBox();
    static struct m3d::Class * __fastcall GetBaseClass();
    static class m3d::Object * __fastcall CreateObject();
protected:
    void SetIdioticEmbossesBounds(class PointBase<float> const &);
    MsgBox(class MsgBox const &);
    MsgBox();
    void AddTitle();
    void SetMsgBounds(class PointBase<float> const &);
    void AddMsg();
    void AddLines();
    void SetDownLineBounds();
    class PointBase<float> CalcSummaryButtonsSize() const ;
    void Clear();
    class PointBase<float> CalcTitleSize() const ;
    class PointBase<float> CalcSummaryIdioticEmbossesSize() const ;
    void SetUpLineBounds();
    void AddMiscFignya();
    void SetTitleBounds(class PointBase<float> const &);
    virtual int CloseModal(int);
    class PointBase<float> CalcMsgSize() const ;
    void ClearPattern();
    void RecalcLayot();
    void CenterOnScreen();
    void HackedExpandToScreen();
    void CalcSelfWidthByChildrensMaxWidth(float);
    void AddButtonsAndIdioticEmbosses();
    void CalcSelfHeight();
    int LoadPattern();
    void SetButtonsBounds(class PointBase<float> const &);
    virtual int GameDataSetup();
    int CreateFromPattern();
private:
    m3d::ui::Wnd *m_wndTitle;
    m3d::ui::Wnd *m_wndMsg;
    m3d::ui::LineWnd *m_wndUpLine;
    m3d::ui::LineWnd *m_wndDownLine;
    m3d::ui::ButtonWnd *m_buttons[3];
    m3d::ui::Wnd *m_idioticEmbosses[3];
    m3d::ui::Wnd *m_wndFrame;
    m3d::ui::Wnd *m_wndBg;
    unsigned int m_msgBoxFlags;
    CStr m_msg;
    CStr m_title;
    MsgBox::AuxInfo m_aif;
};
