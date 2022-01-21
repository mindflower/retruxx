#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class SliderWnd;
    }
}

class DevideDlg :  public m3d::ui::ModalWnd
{
public:
    class AuxInfo
    {
    public:
        AuxInfo();

    private:
        CStr m_lblOriginalAmountName;
        CStr m_lblDragAmountName;
        CStr m_sliderName;
        CStr m_btnPlusName;
        CStr m_btnMinusName;
    };

public:
    virtual m3d::Class * GetClass() const ;
    static m3d::Object * CreateObject();
    virtual int GameDataSetup();
    void UpdateLabels();
    virtual m3d::Object * Clone();
    virtual int OnWndNotify(m3d::ui::Wnd *,unsigned int,unsigned int,m3d::AIParam const &);
    int GetDragAmount();
    int GetOriginalAmount();
    virtual ~DevideDlg();
    static m3d::Class * GetBaseClass();
    void SetAmount(int);
protected:
    DevideDlg(DevideDlg const &);
    DevideDlg();

public:
    RT_CLASS_INLINE_DECLARE(DevideDlg);

private:
    int m_maxAmount;
    m3d::ui::Wnd *m_lblOriginalAmount;
    m3d::ui::Wnd *m_lblDragAmount;
    m3d::ui::SliderWnd *m_slider;
    m3d::ui::ButtonWnd *m_btnPlus;
    m3d::ui::ButtonWnd *m_btnMinus;
    AuxInfo m_aif;
    bool m_isInited;
};
