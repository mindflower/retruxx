#pragma once
#include <ui/wnd.h>

namespace m3d
{
    namespace ui
    {
        class ButtonWnd;
        class SliderWnd;
    }
}  // namespace m3d

class DevideDlg : public m3d::ui::ModalWnd
{
public:
    void SetAmount(int amount);
    int GetDragAmount();
    int GetOriginalAmount();

    struct AuxInfo
    {
        /* 0x0000 */ CStr m_lblOriginalAmountName;
        /* 0x000c */ CStr m_lblDragAmountName;
        /* 0x0018 */ CStr m_sliderName;
        /* 0x0024 */ CStr m_btnPlusName;
        /* 0x0030 */ CStr m_btnMinusName;
        AuxInfo(DevideDlg::AuxInfo const&);
        AuxInfo();
    }; /* size: 0x003c */

protected:
    DevideDlg();
    DevideDlg(DevideDlg const& rhs);

public:
    virtual ~DevideDlg() override /* 0x00 */;
    virtual m3d::Object* Clone() override /* 0x00 */;
    static m3d::Object* __fastcall CreateObject();
    static m3d::Class* __fastcall GetBaseClass();
    virtual m3d::Class* GetClass() const override /* 0x00 */;
    RT_CLASS_DECLARE(DevideDlg);
    virtual int GameDataSetup() override /* 0x00 */;
    virtual int OnWndNotify(m3d::ui::Wnd* from, unsigned int idFrom, unsigned int message, m3d::AIParam const& data)
        override /* 0x00 */;
    void UpdateLabels();
    /* 0x0224 */ int m_maxAmount;
    /* 0x0228 */ m3d::ui::Wnd* m_lblOriginalAmount;
    /* 0x022c */ m3d::ui::Wnd* m_lblDragAmount;
    /* 0x0230 */ m3d::ui::SliderWnd* m_slider;
    /* 0x0234 */ m3d::ui::ButtonWnd* m_btnPlus;
    /* 0x0238 */ m3d::ui::ButtonWnd* m_btnMinus;
    /* 0x023c */ DevideDlg::AuxInfo m_aif;
    /* 0x0278 */ bool m_isInited;
}; /* size: 0x027c */
